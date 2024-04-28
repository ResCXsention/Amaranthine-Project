#ifndef RES_COMPONENT
#define RES_COMPONENT

#include <type_traits>

namespace res
{
	class Node;

	class Component
	{
	public:
		Component() = default;
		Component(const Component &other) = delete;

		void operator=(Component &other) = delete;

		virtual void cycle() {};

	protected:
		Node *owning_node;

		friend class Node;
	};
}

#endif
