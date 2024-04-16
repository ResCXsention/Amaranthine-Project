#ifndef RES_MODEL_RESOURCE
#define RES_MODEL_RESOURCE

#include <vector>
#include <assimp/scene.h>
#include <resource.hxx>

namespace res
{
	class ModelResource final : public Resource
	{
	public:
		class Mesh final
		{
		public:
			explicit Mesh(
					const std::vector<float> vertices,
					const std::vector<float> normals,
					const std::vector<unsigned int> indices
					);

			unsigned int get_vao() const;
			unsigned int get_index_count() const;

		private:
			std::vector<float> vertices;
			std::vector<float> normals;
			std::vector<unsigned int> indices;

			unsigned int vao, vbo, ebo, nbo;
		};

		virtual void load(const std::string path) override;
		virtual void unload() override;

		const std::vector<Mesh> get_meshes() const;

	private:
		std::vector<Mesh> meshes;

		void load_ainode(aiNode *node, const aiScene *scene);
	};

#ifndef NDEBUG
	ModelResource::Mesh spherical_mesh(
		const float radius,
		const unsigned int longitudes,
		const unsigned int latitudes
		);
#endif
}

#endif
