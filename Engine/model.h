#pragma once
#include "renderable.h"
#include "mesh.h"
#include "shader.h"
#include "material.h"

class Model :
	public Renderable
{
public:
	Model(std::string name, Scene * scene) :Renderable(name, scene) {};
	~Model();

	void Update();
	void Render();

public:
	Mesh m_mesh;
	Shader m_shader;
	Material m_material;
	std::string m_cameraID;
};

