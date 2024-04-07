#ifndef RES_MESH
#define RES_MESH

#include <vector>

namespace res
{
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

#ifndef NDEBUG
	Mesh spherical_mesh(
		const float radius,
		const unsigned int longitudes,
		const unsigned int latitudes
		);
#endif
}

#endif
