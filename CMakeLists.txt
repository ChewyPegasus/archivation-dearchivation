cmake_minimum_required(VERSION 3.10)
project(arch_dearch VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/archiver/archiver.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
