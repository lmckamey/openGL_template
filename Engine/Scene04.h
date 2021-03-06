#pragma once
#include "stdafx.h"
#include "scene.h"
#include "material.h"
#include "camera.h"
#include "shader.h"

class Scene04 :
	public Scene
{
public:
	Scene04(Engine* e) : Scene(e) {}
	~Scene04();

	bool Initalize();
	void Update();
	void Render();
	void Shutdown();


private:


	struct object
	{
		// id / handles
		GLuint shaderProgram;
		GLuint vaoHandle;

		// transforms
		GLint mxModelViewUniform;
		GLint mxMVPUniform;
		GLint mxNormalUniform;

		// material
		GLint ambientMaterialUniform;
		GLint diffuseMaterialUniform;
		GLint specularMaterialUniform;
	};

	struct light
	{
		GLint positionUniform;
		GLint colorUniform;
	};

private:

	object m_cube;
	Material m_material;

	Shader m_shader;

	float m_rotation;

};
