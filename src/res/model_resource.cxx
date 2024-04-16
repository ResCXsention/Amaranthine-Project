#include "model_resource.hxx"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glbinding/gl/gl.h>

namespace res
{
	ModelResource::Mesh::Mesh(
			const std::vector<float> vertices,
			const std::vector<float> normals,
			const std::vector<unsigned int> indices
			)
		:vertices{vertices}, normals{normals}, indices{indices}
	{
		gl::glGenVertexArrays(1, &vao);	
		gl::glGenBuffers(1, &vbo);
		gl::glGenBuffers(1, &ebo);
		gl::glGenBuffers(1, &nbo);

		gl::glBindVertexArray(vao);

		gl::glBindBuffer(gl::GL_ARRAY_BUFFER, vbo);
		gl::glBufferData(gl::GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), gl::GL_STATIC_DRAW);
		gl::glEnableVertexAttribArray(0);
		gl::glVertexAttribPointer(0, 3, gl::GL_FLOAT, false, 0, reinterpret_cast<void*>(0));

		gl::glBindBuffer(gl::GL_ARRAY_BUFFER, nbo);
		gl::glBufferData(gl::GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), gl::GL_STATIC_DRAW);
		gl::glEnableVertexAttribArray(1);
		gl::glVertexAttribPointer(1, 3, gl::GL_FLOAT, false, 0, reinterpret_cast<void*>(0));

		gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, ebo);
		gl::glBufferData(gl::GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), gl::GL_STATIC_DRAW);

		gl::glBindBuffer(gl::GL_ARRAY_BUFFER, 0);
		gl::glBindVertexArray(0);
		gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int ModelResource::Mesh::get_vao() const
	{
		return vao;
	}

	unsigned int ModelResource::Mesh::get_index_count() const
	{
		return indices.size();
	}

	void ModelResource::load(const std::string path)
	{
		static Assimp::Importer importer;
		const aiScene *scene{importer.ReadFile(path, aiProcess_Triangulate)};
		if (scene == nullptr) std::cout << importer.GetErrorString() << std::endl;

		load_ainode(scene->mRootNode, scene);
	}

	void ModelResource::unload()
	{
	}

	const std::vector<ModelResource::Mesh> ModelResource::get_meshes() const
	{
		return meshes;
	}

	void ModelResource::load_ainode(aiNode *node, const aiScene *scene)
	{
		for (unsigned int i{0}; i < node->mNumMeshes; ++i) {
			aiMesh *mesh{scene->mMeshes[node->mMeshes[i]]};

			std::vector<float> vertices, normals;
			std::vector<unsigned int> indices;
			for (unsigned int i{0}; i < mesh->mNumVertices; ++i) {
				vertices.push_back(mesh->mVertices[i].x);
				vertices.push_back(mesh->mVertices[i].y);
				vertices.push_back(mesh->mVertices[i].z);
				normals.push_back(mesh->mNormals[i].x);
				normals.push_back(mesh->mNormals[i].y);
				normals.push_back(mesh->mNormals[i].z);
			}
			for (unsigned int i{0}; i < mesh->mNumFaces; ++i) {
				aiFace face{mesh->mFaces[i]};
				for (unsigned int j{0}; j < face.mNumIndices; ++j) {
					indices.push_back(face.mIndices[j]);
				}
			}
			meshes.push_back(Mesh(vertices, normals, indices));
		}
		for (unsigned int i{0}; i < node->mNumChildren; ++i) {
			load_ainode(node->mChildren[i], scene);
		}
	}
}
