add_executable(main)

target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} res)
target_sources(main PUBLIC
	main.cxx
	mesh.hxx mesh.cxx
	res/resource.hxx
	res/text_resource.hxx res/text_resource.cxx
	res/model_resource.hxx res/model_resource.cxx
)
