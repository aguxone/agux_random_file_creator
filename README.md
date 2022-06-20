# Agux's Random File Creator

## Description
Creates a random text file file containing the current date every "x" seconds in the specified paths; app has an integrated GUI made with Dear ImGUI and it's easy and straightforward to use.
File paths (the ones where the random files will be created) need to be specificied in the "directorios.ini" file separated by a new line.
Application is intended to be used in Windows, although it could be compiled for any platform without it's "minimize to notification area" functionality.
A windows release is available on this repository.

<!-- ![screen-gif](./prueba_rando_creator.gif) -->
<img src="https://github.com/aguxone/agux_random_file_creator/blob/gif_storage/760x530_15fps.gif?raw=true" alt="agxu_rfc_gif" width="60%" height="40%">


## Libraries used
- ImGui 1.88 Docking (for the GUI, OpenGL3 as backend)
- SDL 2.0.20

## Compiling from source:
The code was made to be compiled with MinGW compiler for windows (you can change a single line from g++ to Clang in the Makefile to compile with Clang if you prefer, not tested), although Imgui and SDL are cross-platform libraries and you can theoretically compile this code for windows, linux, macos and even for Android and Ios with proper compilation parameters and dependencies.
- For windows: Have a version of the MinGW compiler (i used version 11.2.0 compiled by Brecht Sanders https://www.winlibs.com/), and on a command prompt go to the root directory of the app and simply use the following command (in order to use the provided Makefile)

    mingw32-make -f Makefile





