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
		explicit Model(const std::string path);
		void draw();

	private:
		std::vector<Mesh> meshes;
		
		void process_node(aiNode *node, const aiScene *scene);
	};
}

#endif
