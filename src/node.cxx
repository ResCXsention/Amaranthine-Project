#include "node.hxx"

#include <iostream>
#include <glbinding/gl/gl.h>

namespace res
{
	void Node::add_child(Node *node)
	{
		children.push_back(node);
	}

	void Node::set_transform(const midnight::Matrix4x4 transform)
	{
		node_transform = transform;
	}

	midnight::Matrix4x4 Node::get_transform() const
	{
		return node_transform;
	}

	void Node::set_shader_program(const unsigned int shader_program)
	{
		this->shader_program = shader_program;
	}

	void Node::set_model(std::weak_ptr<res::ModelResource> model)
	{
		this->model = model;
	}

	void Node::set_uniform_preset_function(std::function<preset_uniforms_signature> function)
	{
		preset_uniforms = function;
	}

	void Node::update_and_render(const midnight::Matrix4x4 current_transform)
	{
		if (std::shared_ptr<res::ModelResource> locked_model{model.lock()}) {
			if (shader_program != 0) {
				std::vector<res::ModelResource::Mesh> meshes{locked_model->get_meshes()};
				for (auto &mesh : meshes) {
					gl::glUseProgram(shader_program);
					preset_uniforms(shader_program, current_transform * node_transform);
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

		for (auto child : children) {
			child->update_and_render(node_transform * current_transform);
		}
	}

	void default_preset_uniforms(
			const int program,
			const midnight::Matrix4x4 node_transform
			)
	{
		const int model_loc{gl::glGetUniformLocation(program, "u_model")};
		const int view_loc{gl::glGetUniformLocation(program, "u_view")};
		const int projection_loc{gl::glGetUniformLocation(program, "u_projection")};
		static midnight::Matrix4x4 p{midnight::matrixPerspective(0.57, 800 / 600, 0.001F, 2000)};
		gl::glUniformMatrix4fv(model_loc, 1, false, node_transform.dataPtr());
		gl::glUniformMatrix4fv(view_loc, 1, false, midnight::matrixIdentity<4>().dataPtr());
		gl::glUniformMatrix4fv(gl::glGetUniformLocation(program, "u_projection"), 1, false, p.dataPtr());
	}
}
