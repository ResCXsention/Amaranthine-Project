cmake_minimum_required(VERSION 3.28)
project("The Amaranthine Project" VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory("libs/midnight")
add_subdirectory("src")

target_link_libraries(main PUBLIC midnight)
target_include_directories(main PUBLIC "libs/midnight")
