#pragma once
#include "stdafx.h"
#include"glm\glm.hpp"


class Material
{

	struct TextureInfo
	{
		GLuint activeTexture;
		GLuint texture;
		GLenum type;
	};

public:
	Material();
	~Material();


	void AddTexture(GLuint texture, GLuint activeTexture);
	void SetMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	bool LoadTextureCube(const std::string& basename, const std::vector<std::string>& suffixes, const std::string& type, GLuint activeTexture);
	bool LoadTexture2D(const std::string& filename, GLuint activeTexture);
	void SetTextures();

public:

	static GLuint CreateTexture(GLuint width, GLuint height);
	static GLuint CreateDepthTexture(GLuint width, GLuint height);
	static GLuint CreateDepthbuffer(GLuint texture, GLuint width, GLuint height);

public:
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	float m_shininess;

	std::vector<TextureInfo> m_textures;

};

