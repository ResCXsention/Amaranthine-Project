cmake_minimum_required(VERSION 3.28)
project("The Amaranthine Project" VERSION 0.1 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory("lib/midnight")
add_subdirectory("lib/glad")
add_subdirectory("src")

find_package(glfw3 3.3 REQUIRED)

target_link_libraries(main PUBLIC midnight loader glfw)
target_sources(main PUBLIC lib/glad/gl.c)
target_include_directories(main PUBLIC "lib")
