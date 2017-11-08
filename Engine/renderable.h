#pragma once
#include "object.h"
#include "shader.h"
#include "material.h"
class Renderable :
	public Object
{
public:
	Renderable();
	~Renderable();

	void Render();
protected:
	Shader m_shader;
	Material m_material;

};

