cmake_minimum_required(VERSION 3.4)

project(rest_pistache)

set(CMAKE_CXX_STANDARD 14)

find_package(Pistache 0.0.2 REQUIRED)

find_package(RapidJSON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} pistache_shared)
