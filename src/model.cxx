#include "model.hxx"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glbinding/gl/gl.h>

namespace res
{
	void Model::index_asset(const std::string path)
	{
		static Assimp::Importer importer;
		const aiScene *mesh_scene{importer.ReadFile(path, aiProcess_Triangulate)};
		if (mesh_scene == nullptr) std::cout << importer.GetErrorString() << std::endl;

		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<unsigned int> indices;
		process_ainode(mesh_scene->mRootNode, mesh_scene);
		meshes.push_back(Mesh(vertices, normals, indices));
	}

	void Model::draw()
	{
		for (auto m{meshes.begin()}; m != meshes.end(); ++m) {
			gl::glBindVertexArray(m->get_vao());
			gl::glDrawElements(gl::GL_TRIANGLES, m->get_index_count(), gl::GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
			gl::glBindVertexArray(0);
		}
	}

	void Model::process_ainode(aiNode *node, const aiScene *scene)
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
			process_ainode(node->mChildren[i], scene);
		}
	}
}
