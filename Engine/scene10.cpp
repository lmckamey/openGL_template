#include "glm\vec3.hpp"
#include "glm\matrix.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\random.hpp"
#include "stb_image.h"
#include "tiny_obj_loader.h"
#include "Scene10.h"
#include "timer.h"
#include "renderer.h"
#include "stdafx.h"
#include "meshLoader.h"
#include "input.h"
#include "light.h"
#include "model.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>


namespace
{

	GLuint vaoHandle;

	glm::mat4 mxModel;
	glm::mat4 mxView;
	glm::mat4 mxProjection;
	glm::mat4 mxModelView;
	glm::mat3 mxNormal;

	glm::mat4 translate;
	glm::mat4 rotate;
	glm::mat4 MVP;


}

Scene10::~Scene10()
{
}

bool Scene10::Initalize()
{


	//Adding input Buttons
	m_engine->Get<Input>()->AddButton("escape", Input::eButtonType::KEYBOARD, GLFW_KEY_ESCAPE);


	Camera* camera = new Camera("camera", this);
	Camera::Data data;
	data.type = Camera::eType::EDITOR;
	camera->Initialize(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), data);
	AddObject(camera);


	auto skybox = new Model("skybox", this);
	skybox->m_transform.m_scale = glm::vec3(40.0f);
	skybox->m_cameraID = "camera";

	skybox->m_shader.CompileShader("..\\Resources\\Shaders\\reflection.vert.shader", GL_VERTEX_SHADER);
	skybox->m_shader.CompileShader("..\\Resources\\Shaders\\reflection.frag.shader", GL_FRAGMENT_SHADER);

	skybox->m_shader.Link();
	skybox->m_shader.Use();
	skybox->m_shader.PrintActiveAttribs();
	skybox->m_shader.PrintActiveUniforms();

	std::vector<std::string> suffixes = { "_posx", "_negx", "_posy", "_negy", "_posz", "_negz" };
	skybox->m_material.LoadTextureCube("..\\Resources\\Textures\\SkyBox\\borg", suffixes, "jpg", GL_TEXTURE0);
	
	skybox->m_mesh.Load("..\\Resources\\Meshes\\cube.obj");
	skybox->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);

	GLuint skyboxIndex = glGetSubroutineIndex(skybox->m_shader.GetHandle(), GL_VERTEX_SHADER, "skybox");
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &skyboxIndex);


	AddObject(skybox);

	auto model = new Model("model", this);
	//model->m_transform.m_scale = glm::vec3(0.15f);
	model->m_transform.m_scale = glm::vec3(1.0f);
	model->m_transform.m_position = glm::vec3(0.0f,1.0f,0.0f);
	model->m_cameraID = "camera";



	model->m_shader.CompileShader("..\\Resources\\Shaders\\reflection.vert.shader", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\reflection.frag.shader", GL_FRAGMENT_SHADER);

	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_mesh.Load("..\\Resources\\Meshes\\AssaultRifle.obj");

	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);

	GLuint modelIndex = glGetSubroutineIndex(model->m_shader.GetHandle(), GL_VERTEX_SHADER, "reflection");
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &modelIndex);

	AddObject(model);

	return true;
}

void Scene10::Update()
{
	float dt = m_engine->Get<Timer>()->FrameTime();
	Camera* camera = GetObject<Camera>("camera");


	Model* model = GetObject<Model>("model");
	m_rotation = m_rotation + 1.0f * dt;
	glm::quat rotation = glm::angleAxis(m_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	model->m_transform.m_rotation = rotation;

	auto models = GetObjects<Model>();
	for (auto model : models)
	{
		glm::mat4 mxModel = model->m_transform.GetMatrix44();
		model->m_shader.Use();
		model->m_shader.SetUniform("mxModel", mxModel);
		model->m_shader.SetUniform("cameraWorldPosition", camera->m_transform.m_position);
	}

	auto objects = GetObjects<Object>();
	for (auto object : objects)
	{
		object->Update();
	}


	if (m_engine->Get<Input>()->GetButton("escape") == Input::eButtonState::DOWN)
	{
		glfwSetWindowShouldClose(m_engine->Get<Renderer>()->m_window, GLFW_TRUE);
	}

}

void Scene10::Render()
{
	// render code

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::vector<Renderable*> renderables = GetObjects<Renderable>();
	for (auto renderable : renderables)
	{
		renderable->Render();
	}
	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);

}

void Scene10::Shutdown()
{
}
