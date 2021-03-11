#pragma once

#include "Shader.h"
#include "Texture.h"

#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

struct Position
{
	float x;
	float y;
	float z;
};

class Pear
{
public:
	Pear(std::vector<float>&, std::vector<int>&, std::vector<unsigned int>&, const std::string&, const std::string&);
	~Pear();

	virtual bool Draw(glm::mat4&, glm::mat4&, glm::mat4&);

	virtual void SetXPos(float x);
	virtual void SetYPos(float y);
	virtual void SetZPos(float z);

private:
	bool InitVertices(std::vector<float>&);
	bool InitIndices(std::vector<int>&);
	bool InitShaderAttributes();
	bool Destroy();

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	
	Shader* m_shader;
	std::vector<unsigned int> m_textureIDs;
	std::vector<float> m_vertices;
	std::vector<int> m_indices;

	uint32_t m_color;
	Position m_pos;
};

