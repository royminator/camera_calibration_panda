# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/marolab/camera_calibration_panda

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marolab/camera_calibration_panda/build

# Include any dependencies generated for this target.
include CMakeFiles/calib_manual.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/calib_manual.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/calib_manual.dir/flags.make

CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o: CMakeFiles/calib_manual.dir/flags.make
CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o: ../src/calib_manual.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marolab/camera_calibration_panda/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o -c /home/marolab/camera_calibration_panda/src/calib_manual.cpp

CMakeFiles/calib_manual.dir/src/calib_manual.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/calib_manual.dir/src/calib_manual.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marolab/camera_calibration_panda/src/calib_manual.cpp > CMakeFiles/calib_manual.dir/src/calib_manual.cpp.i

CMakeFiles/calib_manual.dir/src/calib_manual.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/calib_manual.dir/src/calib_manual.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marolab/camera_calibration_panda/src/calib_manual.cpp -o CMakeFiles/calib_manual.dir/src/calib_manual.cpp.s

CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o.requires:

.PHONY : CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o.requires

CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o.provides: CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o.requires
	$(MAKE) -f CMakeFiles/calib_manual.dir/build.make CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o.provides.build
.PHONY : CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o.provides

CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o.provides.build: CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o


# Object files for target calib_manual
calib_manual_OBJECTS = \
"CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o"

# External object files for target calib_manual
calib_manual_EXTERNAL_OBJECTS =

calib_manual: CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o
calib_manual: CMakeFiles/calib_manual.dir/build.make
calib_manual: libutil.a
calib_manual: libcamera.a
calib_manual: libcalibration.a
calib_manual: libutil.a
calib_manual: libcamera.a
calib_manual: /usr/local/lib/libopencv_aruco.so.3.3.1
calib_manual: /usr/local/lib/libopencv_calib3d.so.3.3.1
calib_manual: /usr/local/lib/libopencv_features2d.so.3.3.1
calib_manual: /usr/local/lib/libopencv_highgui.so.3.3.1
calib_manual: /usr/local/lib/libopencv_videoio.so.3.3.1
calib_manual: /usr/local/lib/libopencv_imgcodecs.so.3.3.1
calib_manual: /usr/local/lib/libopencv_imgproc.so.3.3.1
calib_manual: /usr/local/lib/libopencv_flann.so.3.3.1
calib_manual: /usr/local/lib/libopencv_core.so.3.3.1
calib_manual: CMakeFiles/calib_manual.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/marolab/camera_calibration_panda/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable calib_manual"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/calib_manual.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/calib_manual.dir/build: calib_manual

.PHONY : CMakeFiles/calib_manual.dir/build

CMakeFiles/calib_manual.dir/requires: CMakeFiles/calib_manual.dir/src/calib_manual.cpp.o.requires

.PHONY : CMakeFiles/calib_manual.dir/requires

CMakeFiles/calib_manual.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/calib_manual.dir/cmake_clean.cmake
.PHONY : CMakeFiles/calib_manual.dir/clean

CMakeFiles/calib_manual.dir/depend:
	cd /home/marolab/camera_calibration_panda/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marolab/camera_calibration_panda /home/marolab/camera_calibration_panda /home/marolab/camera_calibration_panda/build /home/marolab/camera_calibration_panda/build /home/marolab/camera_calibration_panda/build/CMakeFiles/calib_manual.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/calib_manual.dir/depend
