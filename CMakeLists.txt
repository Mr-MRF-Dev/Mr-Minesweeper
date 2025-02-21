cmake_minimum_required(VERSION 3.10)

# Project name
project(MrMinesweeper C)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Add the executable
add_executable(MrMinesweeper src/Minesweeper.c)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src)

# Platform-specific settings
if(WIN32)
    target_link_libraries(MrMinesweeper ws2_32)
endif()