add_library(midnight STATIC)
set_property(TARGET midnight PROPERTY CXX_STANDARD 23)
set_property(TARGET midnight PROPERTY LINKER_LANGUAGE CXX)
target_include_directories(midnight PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(midnight PUBLIC
	midnight.hxx
	util.hxx util.cxx
	matrix.hxx matrix.cxx
	polar.hxx polar.cxx
)
