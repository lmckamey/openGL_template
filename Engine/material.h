#pragma once
#include "stdafx.h"
#include"glm\glm.hpp"


class Material
{

	struct TextureInfo
	{
		GLuint activeTexture;
		GLuint texture;
	};

public:
	Material();
	~Material();

	void SetMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	bool LoadTexture2D(const std::string& filename, GLuint activeTexture);
	void SetTextures();


public:
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	float m_shininess;

	std::vector<TextureInfo> m_textures;

};

