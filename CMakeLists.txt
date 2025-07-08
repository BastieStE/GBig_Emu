cmake_minimum_required(VERSION 3.10)
project(GBig_emu C)

# Set the C standard
set(CMAKE_C_STANDARD 99)


# Add strict warning flags
add_compile_options(-Wall -Wextra -Wpedantic -Werror)
# Optional: even stricter checks
# add_compile_options(-Wconversion -Wsign-conversion -Wstrict-prototypes)

# Find the SDL2 package (make sure SDL2 is installed on your system)
find_package(SDL2 REQUIRED)

# Set the include directories
include_directories(include ${SDL2_INCLUDE_DIRS})

# Add all source files in the lib directory
file(GLOB SOURCES "lib/*.c")

# Create the executable
add_executable(GB_emu ${SOURCES})

# Link SDL2 to your executable
target_link_libraries(GB_emu ${SDL2_LIBRARIES})

# Optional: If you want to organize your build outputs, you can specify a different directory for the executable
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
