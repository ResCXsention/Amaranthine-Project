add_library(loader STATIC)
set_property(TARGET loader PROPERTY LINKER_LANGUAGE CXX)
target_include_directories(loader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(loader PUBLIC gl.h gl.c)
