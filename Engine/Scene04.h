#pragma once
#include "scene.h"
#include "stdafx.h"

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
	object m_cube;
	light m_light;

	float m_rotation;
};
