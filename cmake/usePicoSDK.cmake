## Include this file if you want to use the RF24Mesh library
## in YOUR (Pico) project.

cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Define the RF24Mesh library
add_library(RF24Mesh INTERFACE)

target_sources(RF24Mesh INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/../RF24Mesh.cpp
)

target_include_directories(RF24Mesh INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/../
)
