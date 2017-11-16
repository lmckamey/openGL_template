#include "glm\vec3.hpp"
#include "glm\matrix.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\random.hpp"
#include "tiny_obj_loader.h"
#include "Scene08.h"
#include "timer.h"
#include "renderer.h"
#include "stdafx.h"
#include "input.h"
#include "light.h"
#include "model.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>


#define NUM_LIGHTS 5

Scene08::~Scene08()
{
}

bool Scene08::Initalize()
{

	GLint bpp;
	GLint height;
	GLint width;

	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed);

	Camera* camera = new Camera("camera", this);
	Camera::Data data;
	data.type = Camera::eType::EDITOR;
	camera->Initialize(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0), data);
	AddObject(camera);

	m_engine->Get<Input>()->AddButton("escape", Input::eButtonType::KEYBOARD, GLFW_KEY_ESCAPE);
	m_engine->Get<Input>()->AddButton("mode", Input::eButtonType::KEYBOARD, GLFW_KEY_SPACE);

	for (int i = 0; i < NUM_LIGHTS; i++)
	{

	glm::vec3 position = glm::sphericalRand(4.0f);
	Light* light = new Light("light", this);

	light->ambient = glm::vec3(1);
	light->diffuse = glm::vec3(1);
	light->specular = glm::vec3(1);
	glm::vec3 color = glm::rgbColor(glm::vec3(glm::linearRand(0.0f, 360.0f), 1.0f, 1.0f));

	light->diffuse = color;
	light->m_transform.m_position = position;

	AddObject(light);

	}

	auto model = new Model("model", this);
	model->m_transform.m_scale = glm::vec3(1.0f);
	model->m_transform.m_position = glm::vec3(0.0f);

	auto model2 = new Model("plane", this);
	model2->m_transform.m_scale = glm::vec3(5.0f);
	model2->m_transform.m_position = glm::vec3(0.0f, -1.0f, 0.0f);

	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_multiLight.vert.shader", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_multiLight.frag.shader", GL_FRAGMENT_SHADER);

	model2->m_shader.CompileShader("..\\Resources\\Shaders\\phong_multiLight.vert.shader", GL_VERTEX_SHADER);
	model2->m_shader.CompileShader("..\\Resources\\Shaders\\phong_multiLight.frag.shader", GL_FRAGMENT_SHADER);


	auto lights = GetObjects<Light>();


	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.0f, 0.75f, 0.75f);
	model->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model->m_material.m_shininess = 0.4f * 12.0f;

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	for (int i = 0; i < lights.size(); i++)
	{
		char uniformName[32];

		sprintf_s(uniformName, "lights[%d].diffuse", i);
		model->m_shader.SetUniform(uniformName, lights[i]->diffuse);

		sprintf_s(uniformName, "lights[%d].specular", i);
		model->m_shader.SetUniform(uniformName, lights[i]->specular);
	}

	model->m_mesh.Load("..\\Resources\\Meshes\\suzanne.obj");

	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

	AddObject(model);

	model2->m_shader.Link();
	model2->m_shader.Use();
	model2->m_shader.PrintActiveAttribs();
	model2->m_shader.PrintActiveUniforms();

	model2->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model2->m_material.m_diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
	model2->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model2->m_material.m_shininess = 0.4f * 64.0f;
	model2->m_material.LoadTexture2D("..\\Resources\\Textures\\crate.bmp", GL_TEXTURE0);
	model2->m_material.LoadTexture2D("..\\Resources\\Textures\\crate_specular.bmp", GL_TEXTURE1);

	model2->m_shader.SetUniform("material.ambient", model2->m_material.m_ambient);
	model2->m_shader.SetUniform("material.diffuse", model2->m_material.m_diffuse);
	model2->m_shader.SetUniform("material.specular", model2->m_material.m_specular);
	model2->m_shader.SetUniform("material.shininess", model2->m_material.m_shininess);

	for (int i = 0; i < lights.size(); i++)
	{
		char uniformName[32];

		sprintf_s(uniformName, "lights[%d].diffuse", i);
		model2->m_shader.SetUniform(uniformName, lights[i]->diffuse);

		sprintf_s(uniformName, "lights[%d].specular", i);
		model2->m_shader.SetUniform(uniformName, lights[i]->specular);
	}

	model2->m_mesh.Load("..\\Resources\\Meshes\\plane.obj");


	model2->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model2->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model2->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

	AddObject(model2);
	return true;
}

void Scene08::Update()
{
	//memberDeclorations
	float dt = m_engine->Get<Timer>()->FrameTime();
	//std::cout << m_engine->Get<Timer>()->FramesPerSecond() << std::endl;
	Camera* camera = GetObject<Camera>("camera");


	//LightLogic

	auto lights = GetObjects<Light>();
	for (size_t i = 0; i < lights.size(); i++)
	{
		// calculate light position position = view * light[i]
		float w = (m_pointLightMode) ? 1.0f : 0.0f;

		m_rotation = m_rotation + 1.0f * dt;
		glm::vec4 tempPosition = glm::vec4(lights[i]->m_transform.m_position, 1);
		glm::mat4 math = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f * dt), glm::vec3(0, 1, 0));
		auto temp = glm::vec3(math * tempPosition);
		lights[i]->m_transform.m_position = temp;
		glm::vec4 position = camera->GetView() * glm::vec4(temp, w);

		//m_rotation = m_rotation + 1.0f * dt;
		//glm::quat rotation = glm::angleAxis(m_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		//light->m_transform.m_position = rotation * glm::vec3(0.0f, 1.0f, 1.0f);
		// = camera->GetView() * glm::vec4(light->m_transform.m_position, w);


		auto models = GetObjects<Model>();

		for (auto model : models)
		{
			// set uniform for model shader
			// make sure you call Use() on the model->shader so that it is set
			model->m_shader.Use();
			// create name of uniform like above sprintf_s but it’ll be position
		
			char uniformName[32]; 
			sprintf_s(uniformName, "lights[%d].position", i);
			// set model->shader->SetUniform
			model->m_shader.SetUniform(uniformName, position);

		}
	}



	//ModelLogic


	auto objects = GetObjects<Object>();
	for (auto object : objects)
	{
		object->Update();
	}

	//Input
	if (m_engine->Get<Input>()->GetButton("mode") == Input::eButtonState::DOWN)
	{
		m_pointLightMode = !m_pointLightMode;
	}
	if (m_engine->Get<Input>()->GetButton("escape") == Input::eButtonState::DOWN)
	{
		glfwSetWindowShouldClose(m_engine->Get<Renderer>()->m_window, GLFW_TRUE);
	}

}

void Scene08::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::vector<Renderable*> renderables = GetObjects<Renderable>();
	for (auto renderable : renderables)
	{
		renderable->Render();
	}
	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);
}

void Scene08::Shutdown()
{
}
