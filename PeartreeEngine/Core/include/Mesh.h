#ifndef MESH_H
#define MESH_H

#include <glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture 
{
	uint32_t id;
	uint32_t type_hash;
	std::string path;
};

class Mesh 
{
public:
	// mesh Data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;
	unsigned int VAO;

	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		std::hash<std::string> hashfunc;
		texture_types[0] = (uint32_t)hashfunc("texture_diffuse");
		texture_types[1] = (uint32_t)hashfunc("texture_specular");
		texture_types[2] = (uint32_t)hashfunc("texture_normal");
		texture_types[3] = (uint32_t)hashfunc("texture_height");

		SetupMesh();
	}

	// render the mesh
	void Draw(Shader& shader)
	{
		uint32_t diffuseNr = 1;
		uint32_t specularNr = 1;
		uint32_t normalNr = 1;
		uint32_t heightNr = 1;

		uint32_t typeHash = 0;
		std::string number = "";
		std::string name = "";

		for (uint32_t i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			typeHash = textures[i].type_hash;
			if (typeHash == texture_types[0])
			{
				name = "texture_diffuse";
				number = std::to_string(diffuseNr++);
			}
			else if (typeHash == texture_types[1])
			{
				name = "texture_specular";
				number = std::to_string(specularNr++);
			}
			else if (typeHash == texture_types[2])
			{
				name = "texture_normal";
				number = std::to_string(normalNr++);
			}
			else if (typeHash == texture_types[3])
			{
				name = "texture_height";
				number = std::to_string(heightNr++);
			}

			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

private:
	unsigned int VBO, EBO;
	uint32_t texture_types[4];

	void SetupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		
		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);
	}
};
#endif

