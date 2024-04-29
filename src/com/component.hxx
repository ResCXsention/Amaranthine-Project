#ifndef RES_COMPONENT
#define RES_COMPONENT

#include <type_traits>
#include <midnight.hxx>

namespace res
{
	class Node;

	class Component
	{
	public:
		Component() = default;
		Component(const Component &other) = delete;

		void operator=(Component &other) = delete;

		virtual void cycle(midnight::Matrix4x4 current_transform) {};

	protected:
		Node *owning_node;

		friend class Node;
	};
}

#endif
