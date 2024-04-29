#include "node.hxx"

#include <iostream>
#include <glbinding/gl/gl.h>
#include <component.hxx>
#include <scene.hxx>

namespace res
{
	Node *Node::add_child()
	{
		children.push_back(new Node);
		children.back()->owning_scene = owning_scene;
		return children.back();
	}

	midnight::Matrix4x4 Node::get_transform() const
	{
		return priority_transform * main_transform;
	}

	void Node::set_main_transform(const midnight::Matrix4x4 transform)
	{
		this->main_transform = transform;
	}

	midnight::Matrix4x4 Node::get_main_transform() const
	{
		return main_transform;
	}

	void Node::set_priority_transform(const midnight::Matrix4x4 transform)
	{
		this->priority_transform = transform;
	}

	midnight::Matrix4x4 Node::get_priority_transform() const
	{
		return priority_transform;
	}

	void Node::transform_main(const midnight::Matrix4x4 transform)
	{
		this->main_transform *= transform;
	}

	void Node::transform_priority(const midnight::Matrix4x4 transform)
	{
		this->priority_transform *= transform;
	}

	Scene *Node::get_owning_scene() const
	{
		return owning_scene;
	}

	void Node::cycle(const midnight::Matrix4x4 current_transform)
	{
		for (auto component : components) {
			component.second->cycle(current_transform);
		}
		for (auto child : children) {
			child->cycle(main_transform * current_transform * priority_transform);
		}
	}
}
