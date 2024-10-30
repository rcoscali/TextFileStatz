cmake_minimum_required(VERSION 3.29)
project(TextFileStatz C)

set(CMAKE_C_STANDARD 11)
set(TOP_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Add include dirs for libs
include_directories(.)
include_directories(./libs/libfcm/include)
include_directories(./libs/libStNRGen/include)
include_directories(./libs/CUnit-2.1-3/cunit/Headers)
include_directories(./libs/CUnit-2.1-3/cunit/Headers)

# Add build of the libraries
add_subdirectory(libs/libfcm libs/libfcm/cmake-build-debug/)
add_subdirectory(libs/libStNRGen libs/libStNRGen/cmake-build-debug/)
add_subdirectory(libs/CUnit-2.1-3 libs/CUnit-2.1-3/cmake-build-debug/)

# Add executable target
add_executable(TextFileStatz
        src/main.c
)
# Add libs for target
target_link_libraries(TextFileStatz
        fcm
        StNRGen
)

# Add test executable
add_executable(maintest test/maintest.c)
# Add libs for maintest
target_link_libraries(maintest
        fcm
        StNRGen
        cunitbasic
        cunittestfmk
)
# Add include directories for test executable
target_include_directories(maintest
        PUBLIC libs/CUnit-2.1-3/CUnit/Headers
)
# Add tests
enable_testing()
