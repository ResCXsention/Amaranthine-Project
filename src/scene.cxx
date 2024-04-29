#include "scene.hxx"

#include <node.hxx>
#include <camera_component.hxx>

namespace res
{
	Scene::Scene()
	{
		root = new Node;
		root->owning_scene = this;
		view_matrix = midnight::matrixIdentity<4>();
		const midnight::Matrix4x4 default_p{midnight::matrixPerspective(0.57, 800 / 600, 0.001F, 2000)};
		projection_matrix = default_p;
	}

	Node *Scene::get_root()
	{
		return root;
	}

	Camera const *Scene::get_active_camera() const
	{
		return active_camera;
	}

	void Scene::cycle()
	{
		root->cycle();
	}
}
