# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\Desktop\GPU

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\Desktop\GPU\cmake-build-debug

# Include any dependencies generated for this target.
include oganyan_lab2\CMakeFiles\oganyan_lab2.dir\depend.make
# Include the progress variables for this target.
include oganyan_lab2\CMakeFiles\oganyan_lab2.dir\progress.make

# Include the compile flags for this target's objects.
include oganyan_lab2\CMakeFiles\oganyan_lab2.dir\flags.make

oganyan_lab2\CMakeFiles\oganyan_lab2.dir\main.cpp.obj: oganyan_lab2\CMakeFiles\oganyan_lab2.dir\flags.make
oganyan_lab2\CMakeFiles\oganyan_lab2.dir\main.cpp.obj: ..\oganyan_lab2\main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Desktop\GPU\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object oganyan_lab2/CMakeFiles/oganyan_lab2.dir/main.cpp.obj"
	cd E:\Desktop\GPU\cmake-build-debug\oganyan_lab2
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\oganyan_lab2.dir\main.cpp.obj /FdCMakeFiles\oganyan_lab2.dir\ /FS -c E:\Desktop\GPU\oganyan_lab2\main.cpp
<<
	cd E:\Desktop\GPU\cmake-build-debug

oganyan_lab2\CMakeFiles\oganyan_lab2.dir\main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/oganyan_lab2.dir/main.cpp.i"
	cd E:\Desktop\GPU\cmake-build-debug\oganyan_lab2
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe > CMakeFiles\oganyan_lab2.dir\main.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\Desktop\GPU\oganyan_lab2\main.cpp
<<
	cd E:\Desktop\GPU\cmake-build-debug

oganyan_lab2\CMakeFiles\oganyan_lab2.dir\main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/oganyan_lab2.dir/main.cpp.s"
	cd E:\Desktop\GPU\cmake-build-debug\oganyan_lab2
	C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\oganyan_lab2.dir\main.cpp.s /c E:\Desktop\GPU\oganyan_lab2\main.cpp
<<
	cd E:\Desktop\GPU\cmake-build-debug

# Object files for target oganyan_lab2
oganyan_lab2_OBJECTS = \
"CMakeFiles\oganyan_lab2.dir\main.cpp.obj"

# External object files for target oganyan_lab2
oganyan_lab2_EXTERNAL_OBJECTS =

oganyan_lab2\oganyan_lab2.exe: oganyan_lab2\CMakeFiles\oganyan_lab2.dir\main.cpp.obj
oganyan_lab2\oganyan_lab2.exe: oganyan_lab2\CMakeFiles\oganyan_lab2.dir\build.make
oganyan_lab2\oganyan_lab2.exe: "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.4\lib\x64\OpenCL.lib"
oganyan_lab2\oganyan_lab2.exe: oganyan_lab2\CMakeFiles\oganyan_lab2.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\Desktop\GPU\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable oganyan_lab2.exe"
	cd E:\Desktop\GPU\cmake-build-debug\oganyan_lab2
	"C:\Program Files\JetBrains\CLion 2021.2\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\oganyan_lab2.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x64\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x64\mt.exe --manifests -- C:\PROGRA~2\MICROS~4\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx64\x64\link.exe /nologo @CMakeFiles\oganyan_lab2.dir\objects1.rsp @<<
 /out:oganyan_lab2.exe /implib:oganyan_lab2.lib /pdb:E:\Desktop\GPU\cmake-build-debug\oganyan_lab2\oganyan_lab2.pdb /version:0.0 /machine:x64 /debug /INCREMENTAL /subsystem:console  "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.4\lib\x64\OpenCL.lib" kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<
	cd E:\Desktop\GPU\cmake-build-debug

# Rule to build all files generated by this target.
oganyan_lab2\CMakeFiles\oganyan_lab2.dir\build: oganyan_lab2\oganyan_lab2.exe
.PHONY : oganyan_lab2\CMakeFiles\oganyan_lab2.dir\build

oganyan_lab2\CMakeFiles\oganyan_lab2.dir\clean:
	cd E:\Desktop\GPU\cmake-build-debug\oganyan_lab2
	$(CMAKE_COMMAND) -P CMakeFiles\oganyan_lab2.dir\cmake_clean.cmake
	cd E:\Desktop\GPU\cmake-build-debug
.PHONY : oganyan_lab2\CMakeFiles\oganyan_lab2.dir\clean

oganyan_lab2\CMakeFiles\oganyan_lab2.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" E:\Desktop\GPU E:\Desktop\GPU\oganyan_lab2 E:\Desktop\GPU\cmake-build-debug E:\Desktop\GPU\cmake-build-debug\oganyan_lab2 E:\Desktop\GPU\cmake-build-debug\oganyan_lab2\CMakeFiles\oganyan_lab2.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : oganyan_lab2\CMakeFiles\oganyan_lab2.dir\depend
