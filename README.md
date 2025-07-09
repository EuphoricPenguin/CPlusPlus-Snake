# C++ Snake Game

A small experiment in creating and building a C++ application using MSYS2 and Void Agent/DeepSeek.

What did I learn? Not only is "vibe compiling" possible, but once the dev environment is in order, iterating is fairly smooth.
The biggest gripe I have with C++, or at least the setup I'm using, is that small feature additions result in a lot of
back-and-forth with Void Agent/DeepSeek to resolve minor dependency discrepancies. You will have the best experience if
you prototype a version of each feature you want (GUI/Sound/etc.) at first and get that working before expanding out the
program's internal functionality. This is basically the same version of snake you can get working in 10 minutes with
any other language; it's the annoying build process that slows down "vibe coding" with C++.

## Prerequisites

### 1. Install MSYS2 and MinGW-w64
1. Download MSYS2 from [https://www.msys2.org/](https://www.msys2.org/)
2. Follow the installation instructions
3. Update packages:
   ```shell
   pacman -Syu
   ```
4. Install MinGW-w64 toolchain:
   ```shell
   pacman -S --needed base-devel mingw-w64-x86_64-toolchain
   ```

### 2. Install Required Libraries
Run these commands in MSYS2 MinGW64 terminal:
```shell
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_mixer \
           mingw-w64-x86_64-libogg mingw-w64-x86_64-libvorbis \
           mingw-w64-x86_64-flac mingw-w64-x86_64-mpg123 \
           mingw-w64-x86_64-opus mingw-w64-x86_64-opusfile \
           mingw-w64-x86_64-wavpack mingw-w64-x86_64-cmake
```

## Building the Game

1. Open MSYS2 MinGW64 terminal
2. Navigate to your project directory
3. Create and enter build directory:
   ```shell
   mkdir build
   cd build
   ```
4. Configure the project with CMake (pointing to src directory):
   ```shell
   cmake ../src -G "MinGW Makefiles"
   ```
5. Build the project:
   ```shell
   cmake --build . --config Release
   ```

The executable will be created in the `build/Release/` directory.

## Running the Game

1. Navigate to the build directory:
   ```shell
   cd build/Release
   ```
2. Run the executable:
   ```shell
   ./SnakeGame.exe
   ```

## Distribution

Since this is a static build, the executable should run standalone without requiring additional DLLs.
Even so, I've had issues getting Void Agent/DeepSeek to properly integrate SFX into the binary, so the
copied SFX folder is mandatory for the program to work.

## Troubleshooting

- If you get missing library errors, verify all required packages are installed
- Make sure you're using the MinGW64 terminal, not the MSYS2 terminal
- For static linking issues, check that all `.a` library files exist in `/mingw64/lib/`
- If CMake can't find the source files, verify you're pointing to the correct path (`../src`)

## License

[CC0*](LICENSE)

\*The code is licensed under CC0, as it's entirely generated, but the license of the executable you compile will depend
on the licenses of the libraries.
