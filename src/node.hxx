#ifndef RES_NODE
#define RES_NODE

#include <vector>
#include <map>
#include <iostream>
#include <midnight.hxx>
#include <component.hxx>

namespace res
{
	class System;
	class Component;

	template<class T>
	concept ComponentType = std::is_base_of<Component, T>::value;
	static inline unsigned int unique_component_id()
	{
		static unsigned int id{0};
		return id++;
	}
	template<ComponentType T>
	static unsigned int component_id()
	{
		static unsigned int id{unique_component_id()};
		return id;
	}

	class Node final
	{
	public:
		Node() = default;
		// Node(const Node &other);

		void operator=(const Node &other) = delete;

		Node *add_child();
		void set_transform(const midnight::Matrix4x4 transform);
		midnight::Matrix4x4 get_transform() const;

		template<ComponentType T>
		void add_component();
		template<ComponentType T>
		T *get_component();
		void cycle(const midnight::Matrix4x4 current_transform = midnight::matrixIdentity<4>());

	private:
		System *owning_system;
		midnight::Matrix4x4 transform{midnight::matrixIdentity<4>()};
		std::vector<Node*> children;
		std::map<unsigned int, Component*> components;


		friend class System;
	};

	template<ComponentType T>
	void Node::add_component()
	{
		if (components.contains(component_id<T>())) {
			std::cerr << "Node, attempted to add a component that already exists.\n";
		}
		components[component_id<T>()] = new T();
		components[component_id<T>()]->owning_node = this;
	}

	template<ComponentType T>
	T *Node::get_component()
	{
		return dynamic_cast<T*>(components[component_id<T>()]);
	}
}

#endif
