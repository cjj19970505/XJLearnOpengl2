#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

#include <vector>
#include <iostream>
#include "Mesh.h"
#include "Material.h"

#include "Texture.h"
using namespace std;
namespace xjg
{
	class Model
	{
	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		vector<Mesh> meshes;
		vector<Material> materials;
		string directory;
		string path;
		bool gammaCorrection;
		
		Model() :position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1) {}
		Model(string const &path, bool gamma = false) :Model()
		{
			this->path = path;
			this->gammaCorrection = gamma;
		}
		glm::mat4 getModelMatrix()
		{
			glm::mat4 model(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1));
			model = glm::translate(model, position);
			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, scale);
			return model;
		}
		void loadModel()
		{
			if (meshes.size() > 0)
			{
				cout << "WARRING::MODEL " << path << " HAS BEEN LOADED" << endl;
				return;
			}
			// read file via ASSIMP
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
			// check for errors
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
				return;
			}
			// retrieve the directory path of the filepath
			//if(path.find_last_of('/')!=NULL)
			directory = path.substr(0, path.find_last_of('/'));
			//cout << "DIRECTORY::::" << path.find_last_of('/') << endl;
			// process ASSIMP's root node recursively
			processNode(scene->mRootNode, scene);
			cout << materials.size() << endl;
		}
	private:
		void processNode(aiNode *node, const aiScene *scene)
		{
			// process each mesh located at the current node
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				// the node object only contains indices to index the actual objects in the scene. 
				// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				Material mat;
				meshes.push_back(processMesh(mesh, scene));
				materials.push_back(mat);

			}
			// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene);
			}
		}
		Mesh processMesh(aiMesh *mesh, const aiScene *scene)
		{
			// data to fill
			vector<Vertex> vertices;
			vector<unsigned int> indices;

			// Walk through each of the mesh's vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
								  // positions
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.position = vector;
				// normals
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normal = vector;
				// texture coordinates
				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					glm::vec2 vec;
					// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.texCoords = vec;
				}
				else
					vertex.texCoords = glm::vec2(0.0f, 0.0f);
				// tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.tangent = vector;
				// bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.bitangent = vector;
				vertices.push_back(vertex);
			}
			// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}


			// return a mesh object created from the extracted mesh data
			return Mesh(vertices, indices);
		}

	};
}