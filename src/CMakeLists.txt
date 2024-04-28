add_executable(main)

target_include_directories(main PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		res
		com
		)
target_sources(main PUBLIC
		main.cxx
		node.hxx node.cxx
		system.hxx system.cxx
		res/resource.hxx
		res/text_resource.hxx res/text_resource.cxx
		res/model_resource.hxx res/model_resource.cxx
		com/component.hxx com/component.cxx
		com/drawable_component.hxx com/drawable_component.cxx
		)
