cmake_minimum_required("VERSION" 3.12)
project(IoT_VirtualGreenhouse VERSION 1.0.0 LANGUAGES CXX)

# Enforce standard modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point CMake to your header folders
include_directories(include)

# Define the final executable binary and its dependencies
add_executable(IoT_Greenhouse src/main.cpp)

# Enable multi-threading library support (POSIX Threads)
find_package(Threads REQUIRED)
target_link_libraries(IoT_Greenhouse PRIVATE Threads::Threads)
