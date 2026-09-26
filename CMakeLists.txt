cmake_minimum_required(VERSION 3.12)
project(IoT_VirtualGreenhouse VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Route compiler to seek your header files inside include/
include_directories(include)

find_package(Threads REQUIRED)

# Build Target 1: The Gateway Server
add_executable(CentralServer src/CentralServer.cpp)
target_link_libraries(CentralServer PRIVATE Threads::Threads)

# Build Target 2: The Hardware Simulation Client
add_executable(EdgeDeviceClient src/EdgeDeviceClient.cpp)
target_link_libraries(EdgeDeviceClient PRIVATE Threads::Threads)
