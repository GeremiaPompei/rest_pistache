cmake_minimum_required(VERSION 3.4)

project(rest_pistache)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)

find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(${PROJECT_NAME} main.cpp element.cpp server.cpp client.cpp)

target_compile_definitions(${PROJECT_NAME} PUBLIC PISTACHE_USE_SSL)

target_include_directories(${PROJECT_NAME} PRIVATE ${OPENSSL_INCLUDE_DIR})
    
target_link_libraries(${PROJECT_NAME} PkgConfig::Pistache OpenSSL::SSL OpenSSL::Crypto)
