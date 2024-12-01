cmake_minimum_required(VERSION 3.10)
project(YourProjectName C)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Set the include directories
include_directories(include)

# Add all source files in the lib directory
file(GLOB SOURCES "lib/*.c")

# Create the executable
add_executable(YourExecutable ${SOURCES})

# Optional: If you want to organize your build outputs, you can specify a different directory for the executable
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# You can also link any required libraries (if any)
# target_link_libraries(YourExecutable <library>)
