#include "system.hxx"

#include <node.hxx>

namespace res
{
	System::System()
	{
		root = new Node;
	}

	Node *System::get_root()
	{
		return root;
	}

	void System::cycle()
	{
		root->cycle();
	}
}
