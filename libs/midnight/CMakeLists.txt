add_library(midnight STATIC)
set_property(TARGET midnight PROPERTY CXX_STANDARD 23)
target_sources(midnight PUBLIC
	midnight.hxx
	util.hxx util.cxx
	matrix.hxx matrix.cxx
	polar.hxx polar.cxx
)
