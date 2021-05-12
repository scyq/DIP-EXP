#pragma once
#include <vector>
#include "Pixel.h"

unsigned int max_color = 256;
unsigned int current_color = 0;
// 用于取出颜色某一通道的某一位
const unsigned char mask[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

class octree_node {
public:
    bool is_leaf; // 是否是叶子节点
    bool can_reduce; // 是否可以归约
    unsigned int count; // 该节点记录了多少个像素的颜色
    unsigned int level; // 层数，level = 7 即为叶子结点

    unsigned int red_sum; // 红色分量总和
    unsigned int green_sum; // 绿色分量总和
    unsigned int blue_sum; // 蓝色分量总和

    unsigned int map_to; // 映射到vector方便根据索引查询
    unsigned int child_num; // 子节点数量
    octree_node* child_ptr[8]{};  // 子节点指针

    explicit octree_node(unsigned int lv);
};

class octree {
public:
    std::vector<octree_node*> leaf_nodes;
    // 横向记录每一层的节点
    std::vector<int> layer_nodes[8];
    void add_color(octree_node* &node, unsigned int red, unsigned int green, unsigned int blue, bool is_new);
    void reduce_color();
    void recursive_reduce(unsigned int idx);

    // 计算层数对应RGB位数的二进制组合
    static unsigned int mask_calculate(unsigned int level, unsigned int red, unsigned int green, unsigned int blue);

    // 查询某颜色归并结束后对应的颜色的索引
    unsigned int query_result_index(octree_node* node, unsigned int red, unsigned int green, unsigned int blue);

    // 存储 八叉树索引 - 调色版索引的数组
    // 数组下标是八叉树中的索引
    unsigned int *nidx_pidx_pair;
};

unsigned int octree::mask_calculate(unsigned int level, unsigned int red, unsigned int green, unsigned int blue){
    unsigned int shift = 7 - level;
    return (((red & mask[level]) >> shift) << 2) |
    (((green & mask[level]) >> shift) << 1) | ((blue & mask[level]) >> shift);
}

void octree::add_color(octree_node *&node, unsigned int red, unsigned int green, unsigned int blue, bool is_new) {
    // 将该节点的索引放进所在层中，方便之后搜索
    if (node->level < 8 && is_new) {
        layer_nodes[node->level].emplace_back(node->map_to);
    }
    node->red_sum += red;
    node->green_sum += green;
    node->blue_sum += blue;
    node->count ++;
    if (node->is_leaf)
        return;

    if (node->level == 8) {
        node->is_leaf = true;
        node->can_reduce = false;
        // 这是一个新颜色，需要新建一个叶子节点
        if (is_new) {
            current_color++;
            if (current_color > max_color) {
                reduce_color();
            }
        }
    } else {
        unsigned int idx = mask_calculate(node->level, red, green, blue);
        // 不存在对应的子节点
        if (!node->child_ptr[idx]) {
            node->child_ptr[idx] = new octree_node(node->level + 1);
            leaf_nodes.emplace_back(node->child_ptr[idx]);
            node->child_ptr[idx]->map_to = leaf_nodes.size() - 1;
            node->child_num++;
            add_color(node->child_ptr[idx], red, green, blue, true);
        } else {
            add_color(node->child_ptr[idx], red, green, blue, false);
        }
    }
}

void octree::reduce_color() {
    // 拥有最少子节点的节点索引
    int min_idx = -1;
    for (int i = 7; i >=0 ; i--)
    {
        // 找到最少的子节点归并，使得调色版尽量保留更多的颜色，把少数颜色变成一个颜色
        unsigned int min_count = INT_MAX;
        // 对每一层的节点横向开始归并
        for (int idx : layer_nodes[i])
        {
            if (leaf_nodes[idx]->can_reduce && leaf_nodes[idx]->child_num >= 2 && min_count > leaf_nodes[idx]->child_num)
            {
                min_count = leaf_nodes[idx]->child_num;
                min_idx = idx;
            }
        }
        if (min_idx != -1)
            break;
    }
    recursive_reduce(min_idx);
    leaf_nodes[min_idx]->is_leaf = true;
    current_color -= (leaf_nodes[min_idx]->child_num - 1);
}

// reduce的递归调用
// 只是把所有子节点标记为不可归并
// 不需要再做其他操作是因为，八叉树每个节点都记录了某个像素的颜色的信息
void octree::recursive_reduce(unsigned int idx) {
    if (!leaf_nodes[idx]->can_reduce)
        return;
    leaf_nodes[idx]->can_reduce = false;
    for (auto & child : leaf_nodes[idx]->child_ptr)
    {
        if (child)
            recursive_reduce(child->map_to);
    }
}

// 该函数查找到的一定是保留在调色版内的颜色
unsigned int octree::query_result_index(octree_node *node, unsigned int red, unsigned int green, unsigned int blue) {
    if(node->is_leaf) {
        return node->map_to;
    }
    auto idx = mask_calculate(node->level, red, green, blue);
    return query_result_index(node->child_ptr[idx], red, green, blue);
}

octree_node::octree_node(unsigned int lv) {
    level = lv;
    red_sum = green_sum = blue_sum = count = child_num = 0;
    map_to = -1;
    for (auto & i : child_ptr) {
        i = nullptr;
    }
    is_leaf = false;
    can_reduce = true;
}
