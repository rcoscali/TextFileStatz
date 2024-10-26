cmake_minimum_required(VERSION 3.29)
project(TextFileStatz C)

set(CMAKE_C_STANDARD 11)

# Add include dirs for libs
include_directories(.)
include_directories(./libs/libfcm/include)
include_directories(./libs/libStNRGen/include)

# Add build of the libraries
add_subdirectory(libs/libfcm libs/libfcm/cmake-build-debug/)
add_subdirectory(libs/libStNRGen)

# Add executable target
add_executable(TextFileStatz
        src/main.c)
# Add libs for target
target_link_libraries(TextFileStatz
        fcm
        StNRGen)
