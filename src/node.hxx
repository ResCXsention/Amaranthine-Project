#ifndef RES_NODE
#define RES_NODE

#include <vector>
#include <unordered_map>
#include <iostream>
#include <midnight.hxx>
#include <component.hxx>

namespace res
{
	class Scene;
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
		void operator=(const Node &other) = delete;

		Node *add_child();
		midnight::Matrix4x4 get_transform() const;
		void set_main_transform(const midnight::Matrix4x4 transform);
		midnight::Matrix4x4 get_main_transform() const;
		void set_priority_transform(const midnight::Matrix4x4 transform);
		midnight::Matrix4x4 get_priority_transform() const;
		void transform_main(const midnight::Matrix4x4 transform);
		void transform_priority(const midnight::Matrix4x4 transform);
		Scene *get_owning_scene() const;

		template<ComponentType T>
		void add_component();
		template<ComponentType T>
		T *get_component();
		void cycle(const midnight::Matrix4x4 current_transform = midnight::matrixIdentity<4>());

	private:
		Scene *owning_scene{nullptr};
		midnight::Matrix4x4 main_transform{midnight::matrixIdentity<4>()};
		midnight::Matrix4x4 priority_transform{midnight::matrixIdentity<4>()};
		std::vector<Node*> children;
		std::unordered_map<unsigned int, Component*> components;

		Node() = default;
		Node(const Node &other) = delete;

		friend class Scene;
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
		if (!components.contains(component_id<T>())) {
			std::cerr << "Node, attempted to get a component that doesn't exist.\n";
		}
		return dynamic_cast<T*>(components.at(component_id<T>()));
	}
}

#endif
