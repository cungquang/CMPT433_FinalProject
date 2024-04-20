# Beagle Burnout Brigade
- Team Members:
  - Adam Atbi
  - Daven Chohan
  - Hong Quang Cung

![Team_Image](https://github.com/cungquang/CMPT433_FinalProject/blob/main/2024_04_12%4012_00_02-0809.jpg)

## Project Description

The project aims to develop a robotic car utilizing a camera to detect human faces, calculate
their position, and autonomously navigate towards them, stopping precisely at their feet with the
aid of an ultrasonic sensor. The Beagle Bone will communicate with a Python server for image
analysis in order to accurately identify the target in the image.

Video Demo:

[Project_Demo_Video](https://www.youtube.com/watch?v=vF6NraIldHc)

## General Server File Sturcture

- `ai
- `hal/`: Contains all low-level hardware abstraction layer (HAL) modules
- `app/`: Contains all application-specific code. Broken into modules and a main file
- `build/`: Generated by CMake; stores all temporary build files (may be deleted to clean)

```
  .
  ├── ai_server
  │   └── human_detection_server.py
  │   └── human_detection_test.py
  │   └── test_image.jpg
  ├── app
  │   ├── include
  │   │   └── <file_name>.h
  │   ├── src
  │   │   ├── <file_name>.c
  │   │   └── main.c
  │   └── CMakeLists.txt           # Sub CMake file, just for app/
  ├── hal
  │   ├── include
  │   │   └── hal
  │   │       └── <hardware_filename>.h
  │   ├── src
  │   │   └── <hardware_filename>.c
  │   └── CMakeLists.txt           # Sub CMake file, just for hal/
  ├── CMakeLists.txt               # Main CMake file for the project
  └── README.md
```  

## Usage

- Install CMake: `sudo apt update` and `sudo apt install cmake`
- When you first open the project, click the "Build" button in the status bar for CMake to generate the `build\` folder and recreate the makefiles.
  - When you edit and save a CMakeLists.txt file, VS Code will automatically update this folder.
- When you add a new file (.h or .c) to the project, you'll need to rerun CMake's build
  (Either click "Build" or resave `/CMakeLists.txt` to trigger VS Code re-running CMake)
- Cross-compile using VS Code's CMake addon:
  - The "kit" defines which compilers and tools will be run.
  - Change the kit via the menu: Help > Show All Commands, type "CMake: Select a kit".
    - Kit "GCC 10.2.1 arm-linux-gnueabi" builds for target.
    - Kit "Unspecified" builds for host (using default `gcc`).
  - Most CMake options for the project can be found in VS Code's CMake view (very left-hand side).
- Build the project using Ctrl+Shift+B, or by the menu: Terminal > Run Build Task...
  - If you try to build but get an error about "build is not a directory", the re-run CMake's build as mentioned above.

## Address Sanitizer

- The address sanitizer built into gcc/clang is very good at catching memory access errors.
- Enable it by uncomment the `fsanitize=address` lines in the root CMakeFile.txt.
- For this to run on the BeagleBone, you must run:
  `sudo apt install libasan6`
  - Without this installed, you'll get an error:   
    "error while loading shared libraries: libasan.so.6: cannot open shared object file: No such file or directory"

## Manually Running CMake

To manually run CMake from the command line use:

```shell
  # Regenerate build/ folder and makefiles:
  rm -rf build/         # Wipes temporary build folder
  cmake -S . -B build   # Generate makefiles in build\

  # Build (compile & link) the project
  cmake --build build
```
