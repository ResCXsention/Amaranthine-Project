add_library(midnight)
set_property(TARGET midnight PROPERTY CXX_STANDARD 23)

target_sources(midnight PUBLIC
	FILE_SET midnight_modules
	TYPE CXX_MODULES 
	FILES midnight.cxx matrix.cxx utils.cxx polar.cxx quaternion.cxx
)
