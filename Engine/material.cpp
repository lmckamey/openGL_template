#include "material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static GLuint texture = 0;

Material::Material()
{
}


Material::~Material()
{
}

void Material::SetMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_shininess = shininess;
}

bool Material::LoadTexture2D(const std::string & filename, GLuint activeTexture)
{
	bool success = false; 

	int width;
	int height;
	int n;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &n, 0);

	glGenTextures(0, &activeTexture);
	glBindTexture(GL_TEXTURE_2D, texture);

	TextureInfo textureInfo = { activeTexture, texture };
	m_textures.push_back(textureInfo);

	GLenum storageFormat = (n == 4) ? GL_RGBA8 : GL_RGB8;
	GLenum imageFormat = (n == 4) ? GL_RGBA : GL_RGB;

	if (data)
	{
		//glTextureStorage2D(GL_TEXTURE_2D, 0, storageFormat, height, width);
		glTexImage2D(GL_TEXTURE_2D, 0, 0, width, height, 0, GL_UNSIGNED_BYTE, imageFormat, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		delete data;
		success = true;
		texture++;
	}


	return success;
}

void Material::SetTextures()
{
	for (TextureInfo textureInfo : m_textures)
	{
		glActiveTexture(textureInfo.activeTexture);
		glBindTexture(GL_TEXTURE_2D, textureInfo.texture);
	}
}
