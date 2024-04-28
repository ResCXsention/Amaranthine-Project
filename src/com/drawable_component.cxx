#include "drawable_component.hxx"

#include <iostream>
#include <vector>
#include <glbinding/gl/gl.h>
#include <midnight.hxx>
#include <node.hxx>

namespace res
{
	void Drawable::set_shader(const unsigned int shader)
	{
		this->shader = shader;
	}

	void Drawable::set_model(std::weak_ptr<res::ModelResource> model)
	{
		this->model = model;
	}

	void Drawable::cycle()
	{
		if (std::shared_ptr<res::ModelResource> locked_model{model.lock()}) {
			if (shader != 0) {
				std::vector<res::ModelResource::Mesh> meshes{locked_model->get_meshes()};
				for (auto &mesh : meshes) {
					gl::glUseProgram(shader);

					const int model_loc{gl::glGetUniformLocation(shader, "u_model")};
					const int view_loc{gl::glGetUniformLocation(shader, "u_view")};
					const int projection_loc{gl::glGetUniformLocation(shader, "u_projection")};
					static midnight::Matrix4x4 p{midnight::matrixPerspective(0.57, 800 / 600, 0.001F, 2000)};
					gl::glUniformMatrix4fv(model_loc, 1, false, owning_node->get_transform().dataPtr());
					gl::glUniformMatrix4fv(view_loc, 1, false, midnight::matrixIdentity<4>().dataPtr());
					gl::glUniformMatrix4fv(gl::glGetUniformLocation(shader, "u_projection"), 1, false, p.dataPtr());

					gl::glBindVertexArray(mesh.get_vao());
					gl::glDrawElements(
							gl::GL_TRIANGLES,
							mesh.get_index_count(),
							gl::GL_UNSIGNED_INT,
							reinterpret_cast<void*>(0)
							);
					gl::glBindVertexArray(0);
				}
			}
		} else {
			std::cerr << "Node, model or shader unset.\n";
		}
	}

}
