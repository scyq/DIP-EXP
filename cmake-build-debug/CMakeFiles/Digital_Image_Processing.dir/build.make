# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/yuqingchen/Documents/GitHub/Digital Image Processing"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/yuqingchen/Documents/GitHub/Digital Image Processing/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Digital_Image_Processing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Digital_Image_Processing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Digital_Image_Processing.dir/flags.make

CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.o: CMakeFiles/Digital_Image_Processing.dir/flags.make
CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.o: ../EXP0/exp0.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/yuqingchen/Documents/GitHub/Digital Image Processing/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.o -c "/Users/yuqingchen/Documents/GitHub/Digital Image Processing/EXP0/exp0.cpp"

CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/yuqingchen/Documents/GitHub/Digital Image Processing/EXP0/exp0.cpp" > CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.i

CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/yuqingchen/Documents/GitHub/Digital Image Processing/EXP0/exp0.cpp" -o CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.s

# Object files for target Digital_Image_Processing
Digital_Image_Processing_OBJECTS = \
"CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.o"

# External object files for target Digital_Image_Processing
Digital_Image_Processing_EXTERNAL_OBJECTS =

Digital_Image_Processing: CMakeFiles/Digital_Image_Processing.dir/EXP0/exp0.cpp.o
Digital_Image_Processing: CMakeFiles/Digital_Image_Processing.dir/build.make
Digital_Image_Processing: CMakeFiles/Digital_Image_Processing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/yuqingchen/Documents/GitHub/Digital Image Processing/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Digital_Image_Processing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Digital_Image_Processing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Digital_Image_Processing.dir/build: Digital_Image_Processing

.PHONY : CMakeFiles/Digital_Image_Processing.dir/build

CMakeFiles/Digital_Image_Processing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Digital_Image_Processing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Digital_Image_Processing.dir/clean

CMakeFiles/Digital_Image_Processing.dir/depend:
	cd "/Users/yuqingchen/Documents/GitHub/Digital Image Processing/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/yuqingchen/Documents/GitHub/Digital Image Processing" "/Users/yuqingchen/Documents/GitHub/Digital Image Processing" "/Users/yuqingchen/Documents/GitHub/Digital Image Processing/cmake-build-debug" "/Users/yuqingchen/Documents/GitHub/Digital Image Processing/cmake-build-debug" "/Users/yuqingchen/Documents/GitHub/Digital Image Processing/cmake-build-debug/CMakeFiles/Digital_Image_Processing.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Digital_Image_Processing.dir/depend
