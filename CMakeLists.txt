cmake_minimum_required(VERSION 3.10.0)
project(MiniStudio VERSION 1.0.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/app/lib)

set(SFML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SFML/include")
set(SFML_LIB_DIR "${CMAKE_SOURCE_DIR}/SFML/lib")
set(SFML_BIN_DIR "${CMAKE_SOURCE_DIR}/SFML/bin")

include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIB_DIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(mylib)
add_subdirectory(mainFile)