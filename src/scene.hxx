#ifndef RES_Scene
#define RES_Scene

#include <matrix.hxx>

namespace res
{
	class Node;
	class Camera;

	class Scene final
	{
	public:
		Scene();

		Node *get_root();
		Camera const *get_active_camera() const;
		void cycle();
	
	private:
		Node *root{nullptr};
		Camera *active_camera{nullptr};
		midnight::Matrix4x4 view_matrix;
		midnight::Matrix4x4 projection_matrix;

		friend class Camera;
		friend class Drawable;
	};
}

#endif
