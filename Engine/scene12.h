#pragma once
#include "stdafx.h"
#include "scene.h"
#include "material.h"
#include "camera.h"
#include "shader.h"

class Scene12 :
	public Scene
{
public:
	Scene12(Engine* e) : Scene(e) {}
	~Scene12();

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

	GLint m_numOfVerticies;
	GLuint m_framebuffer;

	float m_rotation;

	GLuint m_depthBuffer = 0;
	Shader* m_depthShader = nullptr;

};
