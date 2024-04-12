#ifndef RES_MODEL
#define RES_MODEL

#include <string>
#include <vector>
#include <assimp/scene.h>
#include <mesh.hxx>

namespace res
{
	class Model final
	{
	public:
		Model() = default;
		void index_asset(const std::string path);
		void draw();

	private:
		std::vector<Mesh> meshes;
		
		void process_ainode(aiNode *node, const aiScene *scene);
	};
}

#endif
