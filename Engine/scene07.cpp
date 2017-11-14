#include "glm\vec3.hpp"
#include "glm\matrix.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "stb_image.h"
#include "tiny_obj_loader.h"
#include "Scene07.h"
#include "timer.h"
#include "renderer.h"
#include "stdafx.h"
#include "input.h"
#include "light.h"
#include "model.h"
#define SPOTLIGHT
//#define SPOTLIGHT

namespace
{

}

Scene07::~Scene07()
{
}

bool Scene07::Initalize()
{

	GLint bpp;
	GLint height;
	GLint width;

	m_engine->Get<Input>()->AddButton("escape", Input::eButtonType::KEYBOARD, GLFW_KEY_ESCAPE);
	m_engine->Get<Input>()->AddButton("mode", Input::eButtonType::KEYBOARD, GLFW_KEY_SPACE);

	Light* light = new Light("light", this);
	light->ambient = glm::vec3(1);
	light->diffuse = glm::vec3(1);
	light->specular = glm::vec3(1);

	AddObject(light);

	auto model = new Model("model", this);
	model->m_transform.m_scale = glm::vec3(1.0f);
	model->m_transform.m_position = glm::vec3(0.0f);

	auto model2 = new Model("plane", this);
	model2->m_transform.m_scale = glm::vec3(5.0f);
	model2->m_transform.m_position = glm::vec3(0.0f, -1.0f, 0.0f);


#ifdef PHONG
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_directional.vert.shader", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_directional.frag.shader", GL_FRAGMENT_SHADER);

	model2->m_shader.CompileShader("..\\Resources\\Shaders\\phong_directional.vert.shader", GL_VERTEX_SHADER);
	model2->m_shader.CompileShader("..\\Resources\\Shaders\\phong_directional.frag.shader", GL_FRAGMENT_SHADER);

#elif defined(TEXTURES)
	m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.vert.shader", GL_VERTEX_SHADER);
	m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.frag.shader", GL_FRAGMENT_SHADER);
#elif defined(MIX_TEXTURES)
	m_shader.CompileShader("..\\Resources\\Shaders\\multi_texture_phong.vert.shader", GL_VERTEX_SHADER);
	m_shader.CompileShader("..\\Resources\\Shaders\\multi_texture_phong.frag.shader", GL_FRAGMENT_SHADER);
	const unsigned char* data2 = Image::LoadBMP("..\\Resources\\Textures\\grass.bmp", width, height, bpp);
#elif defined(SPECULAR_TEXTURES)
	m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong_specular.vert.shader", GL_VERTEX_SHADER);
	m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong_specular.frag.shader", GL_FRAGMENT_SHADER);
	const unsigned char* data2 = Image::LoadBMP("..\\Resources\\Textures\\crate_specular.bmp", width, height, bpp);
#elif defined(SPOTLIGHT)
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_directional.vert.shader", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong_spotlight.frag.shader", GL_FRAGMENT_SHADER);

	model2->m_shader.CompileShader("..\\Resources\\Shaders\\phong_directional.vert.shader", GL_VERTEX_SHADER);
	model2->m_shader.CompileShader("..\\Resources\\Shaders\\phong_spotlight.frag.shader", GL_FRAGMENT_SHADER);

#else
	m_cube.shaderProgram = m_engine->Get<Renderer>()->CreateShaderProgram("..\\Resources\\Shaders\\gouraud.vert.shader", "..\\Resources\\Shaders\\gouraud.frag.shader");

#endif // PHONG

	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.0f, 0.75f, 0.75f);
	model->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model->m_material.m_shininess = 0.4f * 12.0f;
	model->m_material.LoadTexture2D("..\\Resources\\Textures\\crate.bmp", GL_TEXTURE0);
	model->m_material.LoadTexture2D("..\\Resources\\Textures\\crate_specular.bmp", GL_TEXTURE1);

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);
#ifdef SPOTLIGHT
	model->m_shader.SetUniform("light.cutoff", glm::radians(45.0f));
	model->m_shader.SetUniform("light.exponent", 30.0f);
#endif // SPOTLIGHT






	//model->m_shader.SetUniform("fog.distanceMin", 20.0f);
	//model->m_shader.SetUniform("fog.distanceMax", 50.0f);
	//model->m_shader.SetUniform("fog.color", glm::vec3(0.5f, 0.5f, 0.5f));

	model->m_mesh.Load("..\\Resources\\Meshes\\suzanne.obj");

	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

	AddObject(model);

	model2->m_shader.Link();
	model2->m_shader.Use();
	model2->m_shader.PrintActiveAttribs();
	model2->m_shader.PrintActiveUniforms();
		
	model2->m_material.m_ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	model2->m_material.m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	model2->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model2->m_material.m_shininess = 0.4f * 64.0f;
	model2->m_material.LoadTexture2D("..\\Resources\\Textures\\crate.bmp", GL_TEXTURE0);
	model2->m_material.LoadTexture2D("..\\Resources\\Textures\\crate_specular.bmp", GL_TEXTURE1);
		
	model2->m_shader.SetUniform("material.ambient", model2->m_material.m_ambient);
	model2->m_shader.SetUniform("material.diffuse", model2->m_material.m_diffuse);
	model2->m_shader.SetUniform("material.specular", model2->m_material.m_specular);
	model2->m_shader.SetUniform("material.shininess", model2->m_material.m_shininess);
		
	model2->m_shader.SetUniform("light.diffuse", light->diffuse);
	model2->m_shader.SetUniform("light.specular", light->specular);
#ifdef SPOTLIGHT
	model2->m_shader.SetUniform("light.cutoff", glm::radians(45.0f));
	model2->m_shader.SetUniform("light.exponent", 30.0f);
#endif // SPOTLIGHT



	model2->m_mesh.Load("..\\Resources\\Meshes\\plane.obj");


	model2->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model2->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model2->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);
		
	AddObject(model2);

	Camera* camera = new Camera("camera", this);
	Camera::Data data;
	data.type = Camera::eType::EDITOR;
	camera->Initialize(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0), data);
	AddObject(camera);
	return true;
}

void Scene07::Update()
{
	float dt = m_engine->Get<Timer>()->FrameTime();
	//std::cout << m_engine->Get<Timer>()->FramesPerSecond() << std::endl;
	Camera* camera = GetObject<Camera>("camera");
	Light* light = GetObject<Light>("light");

	if (m_engine->Get<Input>()->GetButton("mode") == Input::eButtonState::DOWN)
	{
		m_pointLightMode = !m_pointLightMode;
	}



	float w = (m_pointLightMode) ? 1.0f : 0.0f;

	m_rotation = m_rotation + 1.0f * dt;
	glm::quat rotation = glm::angleAxis(m_rotation, glm::vec3(0.0f, 0.2f, 1.0f));
	light->m_transform.m_position = rotation * glm::vec3(0.0f, 1.0f, 2.0f);
	glm::vec4 position = camera->GetView() * glm::vec4(light->m_transform.m_position, w);
	
	std::vector<Model*> models = GetObjects<Model>();

	for (auto model : models)
	{
		model->m_shader.Use();
		model->m_shader.SetUniform("light.position", position);
		glm::mat3 viewDirectionMatrix = glm::mat3(camera->GetView());
		glm::vec3 direction = viewDirectionMatrix * glm::vec4(glm::vec3(0.0f, -1.0f, 0.0f), 0.0f);
		model->m_shader.SetUniform("light.direction", direction);
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

void Scene07::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::vector<Renderable*> renderables = GetObjects<Renderable>();
	for (auto renderable : renderables)
	{
		renderable->Render();
	}

	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);
}

void Scene07::Shutdown()
{
}
