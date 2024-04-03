cmake_minimum_required(VERSION 3.28)
project("The Amaranthine Project" VERSION 0.1 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER "clang++-17")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory("lib")
add_subdirectory("src")

find_package(glfw3 3.3 REQUIRED)
find_package(glbinding REQUIRED)
find_package(assimp REQUIRED)

target_link_libraries(main PUBLIC midnight glfw glbinding::glbinding assimp)

