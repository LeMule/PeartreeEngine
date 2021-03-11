//peartree
#include "Pear.h"
#include "Logging.h"

//std
#include <random>
#include <chrono>
#include <sstream>

//thirdparty
#include <glad.h>
#include <GLFW/glfw3.h>

Pear::Pear(std::vector<float>& vertices, std::vector<int>& indices, std::vector<unsigned int>& textureIDs, const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
	: m_shader(new Shader(vertexShaderFile, fragmentShaderFile)),
	m_textureIDs(textureIDs)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	InitVertices(vertices);
	InitIndices(indices);
	InitShaderAttributes();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Pear::~Pear()
{
	if (!Destroy())
	{
		std::stringstream ss;
		ss << "Something went wrong Destroying >> " << this;
		LOG(ss.rdbuf());
	}
}

bool Pear::Draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
	//Do the Shader business first
	m_shader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureIDs.at(0));

	unsigned int posLoc = glGetUniformLocation(m_shader->ID, "_texture1");
	glUniform1i(posLoc, m_textureIDs.at(0));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textureIDs.at(1));

	posLoc = glGetUniformLocation(m_shader->ID, "_texture2");
	glUniform1i(posLoc, m_textureIDs.at(1));

	//Bind our VAO for rendering.
	glBindVertexArray(VAO);

	//Position First.
	posLoc = glGetUniformLocation(m_shader->ID, "x");
	glUniform1f(posLoc, m_pos.x);

	posLoc = glGetUniformLocation(m_shader->ID, "y");
	glUniform1f(posLoc, m_pos.y);

	posLoc = glGetUniformLocation(m_shader->ID, "z");
	glUniform1f(posLoc, m_pos.z);

	//Transform.
	float timeValue = glfwGetTime();
	glm::mat4 trans = glm::mat4(1.0f);
	/*trans = glm::rotate(trans, glm::radians(sin(timeValue) + m_rotation), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(1.0, 1.0, 1.0));*/
	posLoc = glGetUniformLocation(m_shader->ID, "transform");
	glUniformMatrix4fv(posLoc, 1, GL_FALSE, glm::value_ptr(trans));

	//Apply Space Matrices.
	posLoc = glGetUniformLocation(m_shader->ID, "model");
	glUniformMatrix4fv(posLoc, 1, GL_FALSE, glm::value_ptr(model));
	
	posLoc = glGetUniformLocation(m_shader->ID, "view");
	glUniformMatrix4fv(posLoc, 1, GL_FALSE, glm::value_ptr(view));

	posLoc = glGetUniformLocation(m_shader->ID, "projection");
	glUniformMatrix4fv(posLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//Finally, Draw
	if(m_indices.empty())
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	else
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

	//Unbind so others can render as well
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool Pear::InitVertices(std::vector<float>& vertices)
{
	if (vertices.empty())
		return false;

	m_vertices = vertices;
	GLuint sizeInBytes = m_vertices.size() * sizeof(float);

	//Vertex Buffer Object.
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeInBytes, &m_vertices.front(), GL_STATIC_DRAW);
	return true;
}

bool Pear::InitIndices(std::vector<int>& indices)
{
	if (indices.empty())
		return false;

	m_indices = indices;
	GLuint sizeInBytes = m_indices.size() * sizeof(int);

	//Element Buffer Object.
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, &m_indices.front(), GL_STATIC_DRAW);
	return true;
}

bool Pear::InitShaderAttributes()
{
	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	return true;
}

bool Pear::Destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glBindVertexArray(0);
	delete m_shader;
	m_shader = nullptr;
	return true;
}

void Pear::SetXPos(float x)
{
	m_pos.x = x;
}

void Pear::SetYPos(float y)
{
	m_pos.y = y;
}

void Pear::SetZPos(float z)
{
	m_pos.z = z;
}