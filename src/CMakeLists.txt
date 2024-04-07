add_executable(main)

target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(main PUBLIC
	main.cxx
	mesh.hxx mesh.cxx
	model.hxx model.cxx
	resource.hxx resource.cxx
)
