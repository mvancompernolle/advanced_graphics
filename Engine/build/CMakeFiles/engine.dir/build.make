# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/matthew/Desktop/adv_graphics/Engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/matthew/Desktop/adv_graphics/Engine/build

# Include any dependencies generated for this target.
include CMakeFiles/engine.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/engine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/engine.dir/flags.make

CMakeFiles/engine.dir/src/Engine.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/Engine.cpp.o: ../src/Engine.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/Engine.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/Engine.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/Engine.cpp

CMakeFiles/engine.dir/src/Engine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/Engine.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/Engine.cpp > CMakeFiles/engine.dir/src/Engine.cpp.i

CMakeFiles/engine.dir/src/Engine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/Engine.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/Engine.cpp -o CMakeFiles/engine.dir/src/Engine.cpp.s

CMakeFiles/engine.dir/src/Engine.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/Engine.cpp.o.requires

CMakeFiles/engine.dir/src/Engine.cpp.o.provides: CMakeFiles/engine.dir/src/Engine.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/Engine.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/Engine.cpp.o.provides

CMakeFiles/engine.dir/src/Engine.cpp.o.provides.build: CMakeFiles/engine.dir/src/Engine.cpp.o

CMakeFiles/engine.dir/src/Camera.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/Camera.cpp.o: ../src/Camera.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/Camera.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/Camera.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/Camera.cpp

CMakeFiles/engine.dir/src/Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/Camera.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/Camera.cpp > CMakeFiles/engine.dir/src/Camera.cpp.i

CMakeFiles/engine.dir/src/Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/Camera.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/Camera.cpp -o CMakeFiles/engine.dir/src/Camera.cpp.s

CMakeFiles/engine.dir/src/Camera.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/Camera.cpp.o.requires

CMakeFiles/engine.dir/src/Camera.cpp.o.provides: CMakeFiles/engine.dir/src/Camera.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/Camera.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/Camera.cpp.o.provides

CMakeFiles/engine.dir/src/Camera.cpp.o.provides.build: CMakeFiles/engine.dir/src/Camera.cpp.o

CMakeFiles/engine.dir/src/Cube.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/Cube.cpp.o: ../src/Cube.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/Cube.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/Cube.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/Cube.cpp

CMakeFiles/engine.dir/src/Cube.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/Cube.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/Cube.cpp > CMakeFiles/engine.dir/src/Cube.cpp.i

CMakeFiles/engine.dir/src/Cube.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/Cube.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/Cube.cpp -o CMakeFiles/engine.dir/src/Cube.cpp.s

CMakeFiles/engine.dir/src/Cube.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/Cube.cpp.o.requires

CMakeFiles/engine.dir/src/Cube.cpp.o.provides: CMakeFiles/engine.dir/src/Cube.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/Cube.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/Cube.cpp.o.provides

CMakeFiles/engine.dir/src/Cube.cpp.o.provides.build: CMakeFiles/engine.dir/src/Cube.cpp.o

CMakeFiles/engine.dir/src/ShaderProgram.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/ShaderProgram.cpp.o: ../src/ShaderProgram.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/ShaderProgram.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/ShaderProgram.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/ShaderProgram.cpp

CMakeFiles/engine.dir/src/ShaderProgram.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/ShaderProgram.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/ShaderProgram.cpp > CMakeFiles/engine.dir/src/ShaderProgram.cpp.i

CMakeFiles/engine.dir/src/ShaderProgram.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/ShaderProgram.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/ShaderProgram.cpp -o CMakeFiles/engine.dir/src/ShaderProgram.cpp.s

CMakeFiles/engine.dir/src/ShaderProgram.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/ShaderProgram.cpp.o.requires

CMakeFiles/engine.dir/src/ShaderProgram.cpp.o.provides: CMakeFiles/engine.dir/src/ShaderProgram.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/ShaderProgram.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/ShaderProgram.cpp.o.provides

CMakeFiles/engine.dir/src/ShaderProgram.cpp.o.provides.build: CMakeFiles/engine.dir/src/ShaderProgram.cpp.o

CMakeFiles/engine.dir/src/main.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/main.cpp.o: ../src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/main.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/main.cpp

CMakeFiles/engine.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/main.cpp > CMakeFiles/engine.dir/src/main.cpp.i

CMakeFiles/engine.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/main.cpp -o CMakeFiles/engine.dir/src/main.cpp.s

CMakeFiles/engine.dir/src/main.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/main.cpp.o.requires

CMakeFiles/engine.dir/src/main.cpp.o.provides: CMakeFiles/engine.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/main.cpp.o.provides

CMakeFiles/engine.dir/src/main.cpp.o.provides.build: CMakeFiles/engine.dir/src/main.cpp.o

CMakeFiles/engine.dir/src/DefualtProgram.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/DefualtProgram.cpp.o: ../src/DefualtProgram.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/DefualtProgram.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/DefualtProgram.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/DefualtProgram.cpp

CMakeFiles/engine.dir/src/DefualtProgram.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/DefualtProgram.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/DefualtProgram.cpp > CMakeFiles/engine.dir/src/DefualtProgram.cpp.i

CMakeFiles/engine.dir/src/DefualtProgram.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/DefualtProgram.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/DefualtProgram.cpp -o CMakeFiles/engine.dir/src/DefualtProgram.cpp.s

CMakeFiles/engine.dir/src/DefualtProgram.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/DefualtProgram.cpp.o.requires

CMakeFiles/engine.dir/src/DefualtProgram.cpp.o.provides: CMakeFiles/engine.dir/src/DefualtProgram.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/DefualtProgram.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/DefualtProgram.cpp.o.provides

CMakeFiles/engine.dir/src/DefualtProgram.cpp.o.provides.build: CMakeFiles/engine.dir/src/DefualtProgram.cpp.o

CMakeFiles/engine.dir/src/Terrain.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/Terrain.cpp.o: ../src/Terrain.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/Terrain.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/Terrain.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/Terrain.cpp

CMakeFiles/engine.dir/src/Terrain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/Terrain.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/Terrain.cpp > CMakeFiles/engine.dir/src/Terrain.cpp.i

CMakeFiles/engine.dir/src/Terrain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/Terrain.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/Terrain.cpp -o CMakeFiles/engine.dir/src/Terrain.cpp.s

CMakeFiles/engine.dir/src/Terrain.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/Terrain.cpp.o.requires

CMakeFiles/engine.dir/src/Terrain.cpp.o.provides: CMakeFiles/engine.dir/src/Terrain.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/Terrain.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/Terrain.cpp.o.provides

CMakeFiles/engine.dir/src/Terrain.cpp.o.provides.build: CMakeFiles/engine.dir/src/Terrain.cpp.o

CMakeFiles/engine.dir/src/Input.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/Input.cpp.o: ../src/Input.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/Input.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/Input.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/Input.cpp

CMakeFiles/engine.dir/src/Input.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/Input.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/Input.cpp > CMakeFiles/engine.dir/src/Input.cpp.i

CMakeFiles/engine.dir/src/Input.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/Input.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/Input.cpp -o CMakeFiles/engine.dir/src/Input.cpp.s

CMakeFiles/engine.dir/src/Input.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/Input.cpp.o.requires

CMakeFiles/engine.dir/src/Input.cpp.o.provides: CMakeFiles/engine.dir/src/Input.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/Input.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/Input.cpp.o.provides

CMakeFiles/engine.dir/src/Input.cpp.o.provides.build: CMakeFiles/engine.dir/src/Input.cpp.o

CMakeFiles/engine.dir/src/Entity.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/Entity.cpp.o: ../src/Entity.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/Entity.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/Entity.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/Entity.cpp

CMakeFiles/engine.dir/src/Entity.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/Entity.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/Entity.cpp > CMakeFiles/engine.dir/src/Entity.cpp.i

CMakeFiles/engine.dir/src/Entity.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/Entity.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/Entity.cpp -o CMakeFiles/engine.dir/src/Entity.cpp.s

CMakeFiles/engine.dir/src/Entity.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/Entity.cpp.o.requires

CMakeFiles/engine.dir/src/Entity.cpp.o.provides: CMakeFiles/engine.dir/src/Entity.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/Entity.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/Entity.cpp.o.provides

CMakeFiles/engine.dir/src/Entity.cpp.o.provides.build: CMakeFiles/engine.dir/src/Entity.cpp.o

CMakeFiles/engine.dir/src/Texture.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/Texture.cpp.o: ../src/Texture.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/Texture.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/Texture.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/Texture.cpp

CMakeFiles/engine.dir/src/Texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/Texture.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/Texture.cpp > CMakeFiles/engine.dir/src/Texture.cpp.i

CMakeFiles/engine.dir/src/Texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/Texture.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/Texture.cpp -o CMakeFiles/engine.dir/src/Texture.cpp.s

CMakeFiles/engine.dir/src/Texture.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/Texture.cpp.o.requires

CMakeFiles/engine.dir/src/Texture.cpp.o.provides: CMakeFiles/engine.dir/src/Texture.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/Texture.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/Texture.cpp.o.provides

CMakeFiles/engine.dir/src/Texture.cpp.o.provides.build: CMakeFiles/engine.dir/src/Texture.cpp.o

CMakeFiles/engine.dir/src/TerrainProgram.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/TerrainProgram.cpp.o: ../src/TerrainProgram.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/TerrainProgram.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/TerrainProgram.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/TerrainProgram.cpp

CMakeFiles/engine.dir/src/TerrainProgram.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/TerrainProgram.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/TerrainProgram.cpp > CMakeFiles/engine.dir/src/TerrainProgram.cpp.i

CMakeFiles/engine.dir/src/TerrainProgram.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/TerrainProgram.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/TerrainProgram.cpp -o CMakeFiles/engine.dir/src/TerrainProgram.cpp.s

CMakeFiles/engine.dir/src/TerrainProgram.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/TerrainProgram.cpp.o.requires

CMakeFiles/engine.dir/src/TerrainProgram.cpp.o.provides: CMakeFiles/engine.dir/src/TerrainProgram.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/TerrainProgram.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/TerrainProgram.cpp.o.provides

CMakeFiles/engine.dir/src/TerrainProgram.cpp.o.provides.build: CMakeFiles/engine.dir/src/TerrainProgram.cpp.o

CMakeFiles/engine.dir/src/Graphics.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/Graphics.cpp.o: ../src/Graphics.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/Graphics.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/Graphics.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/Graphics.cpp

CMakeFiles/engine.dir/src/Graphics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/Graphics.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/Graphics.cpp > CMakeFiles/engine.dir/src/Graphics.cpp.i

CMakeFiles/engine.dir/src/Graphics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/Graphics.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/Graphics.cpp -o CMakeFiles/engine.dir/src/Graphics.cpp.s

CMakeFiles/engine.dir/src/Graphics.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/Graphics.cpp.o.requires

CMakeFiles/engine.dir/src/Graphics.cpp.o.provides: CMakeFiles/engine.dir/src/Graphics.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/Graphics.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/Graphics.cpp.o.provides

CMakeFiles/engine.dir/src/Graphics.cpp.o.provides.build: CMakeFiles/engine.dir/src/Graphics.cpp.o

CMakeFiles/engine.dir/src/EntityManager.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/EntityManager.cpp.o: ../src/EntityManager.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_13)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/EntityManager.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/EntityManager.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/EntityManager.cpp

CMakeFiles/engine.dir/src/EntityManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/EntityManager.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/EntityManager.cpp > CMakeFiles/engine.dir/src/EntityManager.cpp.i

CMakeFiles/engine.dir/src/EntityManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/EntityManager.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/EntityManager.cpp -o CMakeFiles/engine.dir/src/EntityManager.cpp.s

CMakeFiles/engine.dir/src/EntityManager.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/EntityManager.cpp.o.requires

CMakeFiles/engine.dir/src/EntityManager.cpp.o.provides: CMakeFiles/engine.dir/src/EntityManager.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/EntityManager.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/EntityManager.cpp.o.provides

CMakeFiles/engine.dir/src/EntityManager.cpp.o.provides.build: CMakeFiles/engine.dir/src/EntityManager.cpp.o

CMakeFiles/engine.dir/src/Clock.cpp.o: CMakeFiles/engine.dir/flags.make
CMakeFiles/engine.dir/src/Clock.cpp.o: ../src/Clock.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles $(CMAKE_PROGRESS_14)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/engine.dir/src/Clock.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/engine.dir/src/Clock.cpp.o -c /home/matthew/Desktop/adv_graphics/Engine/src/Clock.cpp

CMakeFiles/engine.dir/src/Clock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/engine.dir/src/Clock.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/matthew/Desktop/adv_graphics/Engine/src/Clock.cpp > CMakeFiles/engine.dir/src/Clock.cpp.i

CMakeFiles/engine.dir/src/Clock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/engine.dir/src/Clock.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/matthew/Desktop/adv_graphics/Engine/src/Clock.cpp -o CMakeFiles/engine.dir/src/Clock.cpp.s

CMakeFiles/engine.dir/src/Clock.cpp.o.requires:
.PHONY : CMakeFiles/engine.dir/src/Clock.cpp.o.requires

CMakeFiles/engine.dir/src/Clock.cpp.o.provides: CMakeFiles/engine.dir/src/Clock.cpp.o.requires
	$(MAKE) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/Clock.cpp.o.provides.build
.PHONY : CMakeFiles/engine.dir/src/Clock.cpp.o.provides

CMakeFiles/engine.dir/src/Clock.cpp.o.provides.build: CMakeFiles/engine.dir/src/Clock.cpp.o

# Object files for target engine
engine_OBJECTS = \
"CMakeFiles/engine.dir/src/Engine.cpp.o" \
"CMakeFiles/engine.dir/src/Camera.cpp.o" \
"CMakeFiles/engine.dir/src/Cube.cpp.o" \
"CMakeFiles/engine.dir/src/ShaderProgram.cpp.o" \
"CMakeFiles/engine.dir/src/main.cpp.o" \
"CMakeFiles/engine.dir/src/DefualtProgram.cpp.o" \
"CMakeFiles/engine.dir/src/Terrain.cpp.o" \
"CMakeFiles/engine.dir/src/Input.cpp.o" \
"CMakeFiles/engine.dir/src/Entity.cpp.o" \
"CMakeFiles/engine.dir/src/Texture.cpp.o" \
"CMakeFiles/engine.dir/src/TerrainProgram.cpp.o" \
"CMakeFiles/engine.dir/src/Graphics.cpp.o" \
"CMakeFiles/engine.dir/src/EntityManager.cpp.o" \
"CMakeFiles/engine.dir/src/Clock.cpp.o"

# External object files for target engine
engine_EXTERNAL_OBJECTS =

engine: CMakeFiles/engine.dir/src/Engine.cpp.o
engine: CMakeFiles/engine.dir/src/Camera.cpp.o
engine: CMakeFiles/engine.dir/src/Cube.cpp.o
engine: CMakeFiles/engine.dir/src/ShaderProgram.cpp.o
engine: CMakeFiles/engine.dir/src/main.cpp.o
engine: CMakeFiles/engine.dir/src/DefualtProgram.cpp.o
engine: CMakeFiles/engine.dir/src/Terrain.cpp.o
engine: CMakeFiles/engine.dir/src/Input.cpp.o
engine: CMakeFiles/engine.dir/src/Entity.cpp.o
engine: CMakeFiles/engine.dir/src/Texture.cpp.o
engine: CMakeFiles/engine.dir/src/TerrainProgram.cpp.o
engine: CMakeFiles/engine.dir/src/Graphics.cpp.o
engine: CMakeFiles/engine.dir/src/EntityManager.cpp.o
engine: CMakeFiles/engine.dir/src/Clock.cpp.o
engine: CMakeFiles/engine.dir/build.make
engine: CMakeFiles/engine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable engine"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/engine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/engine.dir/build: engine
.PHONY : CMakeFiles/engine.dir/build

CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/Engine.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/Camera.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/Cube.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/ShaderProgram.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/main.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/DefualtProgram.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/Terrain.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/Input.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/Entity.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/Texture.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/TerrainProgram.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/Graphics.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/EntityManager.cpp.o.requires
CMakeFiles/engine.dir/requires: CMakeFiles/engine.dir/src/Clock.cpp.o.requires
.PHONY : CMakeFiles/engine.dir/requires

CMakeFiles/engine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/engine.dir/cmake_clean.cmake
.PHONY : CMakeFiles/engine.dir/clean

CMakeFiles/engine.dir/depend:
	cd /home/matthew/Desktop/adv_graphics/Engine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/matthew/Desktop/adv_graphics/Engine /home/matthew/Desktop/adv_graphics/Engine /home/matthew/Desktop/adv_graphics/Engine/build /home/matthew/Desktop/adv_graphics/Engine/build /home/matthew/Desktop/adv_graphics/Engine/build/CMakeFiles/engine.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/engine.dir/depend

