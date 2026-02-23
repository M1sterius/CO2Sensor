cmake_minimum_required(VERSION 3.29)
project(CO2Server)

set(CMAKE_CXX_STANDARD 20)
set(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/Dependencies)

add_executable(CO2Server
    Source/main.cpp
    Server.cpp
)

target_include_directories(CO2Server PRIVATE
    Source
    ../Protocol
    ${DEPENDENCIES_DIR}/boost_asio
    ${DEPENDENCIES_DIR}/fmt/include
)

add_subdirectory(${DEPENDENCIES_DIR}/fmt ${CMAKE_CURRENT_BINARY_DIR}/fmt)
target_link_libraries(CO2Server PRIVATE fmt::fmt)
