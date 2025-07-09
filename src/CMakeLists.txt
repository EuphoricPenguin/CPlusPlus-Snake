cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

# Set source files with full paths
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(SOURCES 
    "main.cpp"
    "Game.cpp"
)
set(HEADERS 
    "${SOURCE_DIR}/Game.h"
)

# Verify source files exist
foreach(SRC IN LISTS SOURCES HEADERS)
    if(NOT EXISTS "${SRC}")
        message(FATAL_ERROR "Missing required file: ${SRC}")
    endif()
endforeach()

set(CMAKE_CXX_STANDARD 17)

# Static build configuration
set(BUILD_SHARED_LIBS OFF)
set(SDL_STATIC ON)
set(SDL2_STATIC ON)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)

# Static linking flags
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()

# Set explicit library paths for MSYS2
set(SDL2_PATH "C:/msys64/mingw64")
set(SDL2_LIBRARY "${SDL2_PATH}/lib/libSDL2.a")
set(SDL2_MIXER_LIBRARY "${SDL2_PATH}/lib/libSDL2_mixer.a")
set(XMP_LIBRARY "${SDL2_PATH}/lib/libxmp.a")
set(FLAC_LIBRARY "${SDL2_PATH}/lib/libFLAC.a")
set(MPG123_LIBRARY "${SDL2_PATH}/lib/libmpg123.a")
set(VORBIS_LIBRARY "${SDL2_PATH}/lib/libvorbis.a")
set(VORBISFILE_LIBRARY "${SDL2_PATH}/lib/libvorbisfile.a")
set(OGG_LIBRARY "${SDL2_PATH}/lib/libogg.a")
set(OPUS_LIBRARY "${SDL2_PATH}/lib/libopus.a")
set(OPUSFILE_LIBRARY "${SDL2_PATH}/lib/libopusfile.a")
set(WAVPACK_LIBRARY "${SDL2_PATH}/lib/libwavpack.a")
set(SETUPAPI_LIBRARY "${SDL2_PATH}/lib/libsetupapi.a")

# Find dependencies
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
include_directories(
    ${SDL2_INCLUDE_DIRS} 
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_PATH}/include/SDL2
    ${CMAKE_SOURCE_DIR}/src
)

# Create executable (WIN32 for Windows subsystem)
add_executable(SnakeGame WIN32 ${SOURCES} ${HEADERS})

# Link libraries in proper dependency order
target_link_libraries(SnakeGame PRIVATE
    ${SDL2_LIBRARY}
    ${SDL2_MIXER_LIBRARY}
    ${XMP_LIBRARY}
    ${FLAC_LIBRARY}
    ${MPG123_LIBRARY}
    ${VORBISFILE_LIBRARY}
    ${VORBIS_LIBRARY}
    ${OGG_LIBRARY}
    ${OPUSFILE_LIBRARY}
    ${OPUS_LIBRARY}
    ${WAVPACK_LIBRARY}
    ${SETUPAPI_LIBRARY}
    mingw32
    SDL2main
    opengl32
    imm32
    winmm
    version
    shell32
    shlwapi
    dwmapi
    gdi32
)

# Windows subsystem configuration
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
endif()

# Copy sound files
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/sfx)
add_custom_command(TARGET SnakeGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/sfx/chomp.wav
        ${CMAKE_BINARY_DIR}/sfx/
    COMMENT "Copying sound files..."
)

message(STATUS "SDL2 Library: ${SDL2_LIBRARY}")
message(STATUS "SDL2_mixer Library: ${SDL2_MIXER_LIBRARY}")
message(STATUS "XMP Library: ${XMP_LIBRARY}")
