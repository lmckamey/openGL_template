#pragma once
#include "stdafx.h"
#include "scene.h"
#include "renderable.h"
#include "mesh.h"
class Light :
	public Renderable
{
public:
	Light(const std::string& name, Scene* scene) : Renderable(name, scene) 
	{
		m_mesh.Load("..\\Resources\\Meshes\\cube.obj");
		m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
		m_transform.m_scale = glm::vec3(0.2f);

		m_shader.CompileShader("..\\Resources\\Shaders\\basic.vert", GL_VERTEX_SHADER);
		m_shader.CompileShader("..\\Resources\\Shaders\\basic.frag", GL_FRAGMENT_SHADER);
		m_shader.Link();
		m_shader.Use();

	};
	~Light();

	void Update();
	void Render();

public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
protected:
	Mesh m_mesh;
};

