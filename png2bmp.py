from PIL import Image

target_name = input("请输入目标文件名，不带后缀")

# Open image
image = Image.open('qiuqiu.png')

# Quantize to 256 colours using fast octree method
result = image.quantize(colors=256, method=2)
result.save(target_name + ".bmp")

print("已经生成目标图片为8位256色图片")