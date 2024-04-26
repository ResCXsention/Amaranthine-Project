#ifndef RES_NODE
#define RES_NODE

#include <vector>
#include <memory>
#include <functional>
#include <midnight.hxx>
#include <model_resource.hxx>

namespace res
{
	using preset_uniforms_signature = void(const int, const midnight::Matrix4x4);
	void default_preset_uniforms(
			const int program,
			const midnight::Matrix4x4 node_matrix
			);

	class Node final
	{
	public:
		Node() = default;
		Node(const Node &other) = delete;

		void operator=(const Node &other) = delete;

		void add_child(Node *node);
		void set_transform(const midnight::Matrix4x4 transform);
		midnight::Matrix4x4 get_transform() const;
		void set_shader_program(const unsigned int shader_program);
		void set_model(std::weak_ptr<res::ModelResource> model);
		void set_uniform_preset_function(std::function<preset_uniforms_signature> function);
		void update_and_render(const midnight::Matrix4x4 current_transform = midnight::matrixIdentity<4>());

	private:
		midnight::Matrix4x4 node_transform;
		std::vector<Node*> children;
		std::function<preset_uniforms_signature> preset_uniforms{default_preset_uniforms};
		unsigned int shader_program{0};
		std::weak_ptr<res::ModelResource> model;

	};
}

#endif
