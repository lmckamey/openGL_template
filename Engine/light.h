#pragma once
#include "stdafx.h"
#include "scene.h"
#include "object.h"
class Light :
	public Object
{
public:
	Light(const std::string& name, Scene* scene) : Object(name, scene) {};
	~Light();

	void Update();

public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

