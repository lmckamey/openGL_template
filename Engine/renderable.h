#pragma once
#include "object.h"
#include "shader.h"
#include "material.h"
class Renderable :
	public Object
{
public:
	Renderable(std::string name, Scene * scene):Object(name,scene) {};
	virtual ~Renderable() {};

	virtual void Render() = 0;
	virtual void Update() = 0;
protected:
	Shader m_shader;
	Material m_material;

};

