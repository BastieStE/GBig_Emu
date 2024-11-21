# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(GBig_emuC LANGUAGES C)

# Specify C standard (optional)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include the header files directory
include_directories(include)

# Add source files
set(SOURCES
    lib/bus.c
    lib/cart.c
    lib/cpu.c
    lib/orchestr.c
    lib/ppu.c
    lib/timer.c
    lib/instruction.c
)

# Add the executable
add_executable(GBig_emuC ${SOURCES})

# Specify where the executable should be built
set_target_properties(GBig_emuC PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build
)
