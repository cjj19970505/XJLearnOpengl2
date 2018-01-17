#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glad\glad.h>
#include <vector>
using namespace std;
namespace xjg
{
	struct Vertex
	{
		glm::vec3 position;
		// normal
		glm::vec3 normal;
		// texCoords
		glm::vec2 texCoords;
		// tangent
		glm::vec3 tangent;
		// bitangent
		glm::vec3 bitangent;

		Vertex()
		{
		}
		Vertex(float x, float y, float z)
		{
			position = glm::vec3(x, y, z);
		}
	};
	class Mesh
	{
	private:
		unsigned int VBO, EBO;
	public:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		unsigned int VAO;
		
		Mesh():VBO(0), EBO(0), VAO(0){}
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices):Mesh()
		{
			this->vertices = vertices;
			this->indices = indices;
		}

		void setup()
		{
			if (VAO != 0)
			{
				cout << "WARRING::THIS MESH HAS ALREADY BEEN SETUP" << endl;
				return;
			}
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			// load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// A great thing about structs is that their memory layout is sequential for all its items.
			// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
			// again translates to 3/2 floats which translates to a byte array.
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
			// set the vertex attribute pointers
			// vertex Positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
			// vertex tangent
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
			// vertex bitangent
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

			glBindVertexArray(0);
		}

	};
}