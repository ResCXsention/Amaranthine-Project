#include "node.hxx"

#include <iostream>
#include <glbinding/gl/gl.h>
#include <component.hxx>

namespace res
{
	Node *Node::add_child()
	{
		children.push_back(new Node);
		return children.back();
	}

	void Node::set_transform(const midnight::Matrix4x4 transform)
	{
		this->transform = transform;
	}

	midnight::Matrix4x4 Node::get_transform() const
	{
		return transform;
	}

	void Node::cycle(const midnight::Matrix4x4 current_transform)
	{
		for (auto component : components) {
			component.second->cycle();
		}
		for (auto child : children) {
			child->cycle(transform * current_transform);
		}
	}
}
