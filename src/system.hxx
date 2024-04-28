#ifndef RES_SYSTEM
#define RES_SYSTEM

#include <matrix.hxx>

namespace res
{
	class Node;

	class System final
	{
	public:
		System();

		Node *get_root();
		void cycle();
	
	private:
		Node *root;
		midnight::Matrix4x4 view_matrix;
		midnight::Matrix4x4 projection_matrix;
	};
}

#endif
