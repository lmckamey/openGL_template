#include "glm\vec3.hpp"
#include "glm\matrix.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "stb_image.h"
#include "tiny_obj_loader.h"
#include "Scene06.h"
#include "timer.h"
#include "renderer.h"
#include "stdafx.h"
#include "input.h"
#include "light.h"
#include "model.h"
#define PHONG


namespace
{
	
}

Scene06::~Scene06()
{
}

bool Scene06::Initalize()
{

	GLint bpp;
	GLint height;
	GLint width;

	m_engine->Get<Input>()->AddButton("escape", Input::eButtonType::KEYBOARD, GLFW_KEY_ESCAPE);


	Light* light = new Light("light", this);
	light->ambient = glm::vec3(1);
	light->diffuse = glm::vec3(1);
	light->specular = glm::vec3(1);
	light->m_transform.m_position = glm::vec3(0.0f, 0.0f, 1.0f);

	AddObject(light);

	auto model = new Model("model", this);
	model->m_transform.m_scale = glm::vec3(1.0f);
	model->m_transform.m_position = glm::vec3(0.0f);


#ifdef PHONG
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong.vert.shader", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong.frag.shader", GL_FRAGMENT_SHADER);
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
	model->m_material.m_shininess = 0.4f * 128.0f;
	model->m_material.LoadTexture2D("..\\Resources\\Textures\\crate.bmp", GL_TEXTURE0);
	model->m_material.LoadTexture2D("..\\Resources\\Textures\\crate_specular.bmp", GL_TEXTURE1);

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);


	model->m_shader.SetUniform("fog.distanceMin", 3.0f);
	model->m_shader.SetUniform("fog.distanceMax", 20.0f);
	model->m_shader.SetUniform("fog.color", glm::vec3(0.5f, 0.5f, 0.5f));

	model->m_mesh.Load("..\\Resources\\Meshes\\Dargon.obj");

	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

	AddObject(model);

	Camera* camera = new Camera("camera", this);	
	Camera::Data data;
	data.type = Camera::eType::ORBIT;
	camera->Initialize(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0),data);
	AddObject(camera);
	return true;
}

void Scene06::Update()
{
	Model* model = GetObject<Model>("model");
	Camera* camera = GetObject<Camera>("camera");
	Light* light = GetObject<Light>("light");
	

	glm::vec4 position = camera->GetView() * glm::vec4(light->m_transform.m_position, 1.0f);

	model->m_shader.Use();

	model->m_shader.SetUniform("light.position", position);

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

void Scene06::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::vector<Renderable*> renderables = GetObjects<Renderable>();
	for (auto renderable : renderables)
	{
		renderable->Render();
	}

	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);
}

void Scene06::Shutdown()
{
}
