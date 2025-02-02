cmake_minimum_required(VERSION 3.20)
project(nes VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# enable_testing()
# add_subdirectory(src)
# add_subdirectory(tests)

if(APPLE)
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif()

# MacPorts /opt/local if needed
include_directories("/opt/local/include")
link_directories("/opt/local/lib")

# Include entire include/ folder
include_directories(${PROJECT_SOURCE_DIR}/include)

# Recursively gather all headers & sources
file(GLOB_RECURSE PROJECT_HEADERS "${PROJECT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})
