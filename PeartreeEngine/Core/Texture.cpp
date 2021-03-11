//#include "Logging.h"
//#include "Texture.h"
//#include "stb_image.h"
//#include "glad/glad.h"
//
//using namespace PearTree;
//
//Texture::Texture(const std::string& filename, bool withAlpha)
//{
//	stbi_set_flip_vertically_on_load(true);
//	glGenTextures(1, &ID);
//	// set the texture wrapping/filtering options (on the currently bound texture object)
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load and generate the texture
//	int width, height, nrChannels;
//	unsigned char* data;
//	if(!withAlpha)
//		data = stbi_load(filename.c_str(), &width, &height, &nrChannels, STBI_rgb);
//	else
//		data = stbi_load(filename.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
//
//	if (data)
//	{
//		if(!withAlpha)
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//		else
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		LOG("Failed to load texture");
//	}
//	stbi_image_free(data);
//}
//
//Texture::~Texture() {}
//Texture::Texture() {}