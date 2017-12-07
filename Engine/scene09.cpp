#include "glm\vec3.hpp"
#include "glm\matrix.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\random.hpp"
#include "stb_image.h"
#include "tiny_obj_loader.h"
#include "Scene09.h"
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

Scene09::~Scene09()
{
}

bool Scene09::Initalize()
{

	GLint bpp;
	GLint height;
	GLint width;

	//seeds Random
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed);

	//Adding input Buttons
	m_engine->Get<Input>()->AddButton("escape", Input::eButtonType::KEYBOARD, GLFW_KEY_ESCAPE);

	Camera* camera = new Camera("camera", this);
	Camera::Data data;
	data.type = Camera::eType::ORBIT;
	camera->Initialize(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), data);
	AddObject(camera);

	camera = new Camera("camera_rtt", this);
	data.type = Camera::eType::LOOK_AT;
	camera->Initialize(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), data);
	AddObject(camera);
	

	Light* light = new Light("light", this);
	light->m_transform.m_position = glm::vec3(3.0f, 2.0f, 1.0f);
	glm::vec3 color = glm::rgbColor(glm::vec3(glm::linearRand(0.0f, 360.0f), 1.0f, 0.8f));
	light->ambient = glm::vec3(1.0f);
	light->diffuse = color;


	AddObject(light);

	auto model = new Model("model", this);
	model->m_transform.m_scale = glm::vec3(1.0f);
	model->m_transform.m_position = glm::vec3(0.0f);
	model->m_cameraID = "camera_rtt";

	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong.vert.shader", GL_VERTEX_SHADER);
	model->m_shader.CompileShader("..\\Resources\\Shaders\\phong.frag.shader", GL_FRAGMENT_SHADER);

	model->m_shader.Link();
	model->m_shader.Use();
	model->m_shader.PrintActiveAttribs();
	model->m_shader.PrintActiveUniforms();

	model->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model->m_material.m_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	model->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model->m_material.m_shininess = 0.4f * 12.0f;

	model->m_shader.SetUniform("material.ambient", model->m_material.m_ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.m_diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.m_specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.m_shininess);

	model->m_shader.SetUniform("light.ambient", light->ambient);
	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);

	model->m_mesh.Load("..\\Resources\\Meshes\\suzanne.obj");

	model->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

	AddObject(model);

	auto model2 = new Model("cube", this);
	model2->m_transform.m_scale = glm::vec3(1.0f);
	model2->m_transform.m_position = glm::vec3(0.0f);
	model2->m_cameraID = "camera";

		

	model2->m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.vert.shader", GL_VERTEX_SHADER);
	model2->m_shader.CompileShader("..\\Resources\\Shaders\\texture_phong.frag.shader", GL_FRAGMENT_SHADER);
		
	model2->m_shader.Link();
	model2->m_shader.Use();
	model2->m_shader.PrintActiveAttribs();
	model2->m_shader.PrintActiveUniforms();
		
	model2->m_material.m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	model2->m_material.m_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	model2->m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	model2->m_material.m_shininess = 0.4f * 12.0f;
		
	model2->m_shader.SetUniform("material.ambient", model2->m_material.m_ambient);
	model2->m_shader.SetUniform("material.diffuse", model2->m_material.m_diffuse);
	model2->m_shader.SetUniform("material.specular", model2->m_material.m_specular);
	model2->m_shader.SetUniform("material.shininess", model2->m_material.m_shininess);
	
	model2->m_mesh.Load("..\\Resources\\Meshes\\cube.obj");
		
	model2->m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
	model2->m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
	model2->m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

	GLuint texture = Material::CreateTexture(512, 512);
	model2->m_material.AddTexture(texture, GL_TEXTURE0);

	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(result == GL_FRAMEBUFFER_COMPLETE);

	AddObject(model2);


	return true;
}

void Scene09::Update()
{
	float dt = m_engine->Get<Timer>()->FrameTime();
	Camera* camera2 = GetObject<Camera>("camera");
	Light* light = GetObject<Light>("light");


	Model* model = GetObject<Model>("model");
	m_rotation = m_rotation + 1.0f * dt;
	glm::quat rotation = glm::angleAxis(m_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	model->m_transform.m_rotation = rotation;

	Camera* camera = GetObject<Camera>("camera_rtt");
	glm::vec4 position = camera->GetView() * glm::vec4(light->m_transform.m_position, 1.0f);
	model->m_shader.Use();
	model->m_shader.SetUniform("light.position", position);

	position = camera2->GetView() * glm::vec4(light->m_transform.m_position, 1.0f);

	model = GetObject<Model>("cube");
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

void Scene09::Render()
{
	// render code

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, m_numOfVerticies);

	glViewport(0, 0, 512, 512);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto renderable = GetObject<Renderable>("model");
	renderable->Render();

	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_engine->Get<Renderer>()->m_width, m_engine->Get<Renderer>()->m_height);
	glClearColor(0.8f, 0.8f, 0.8f, 0.8f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderable = GetObject<Renderable>("cube");
	renderable->Render();
	renderable = GetObject<Renderable>("light");
	renderable->Render();

	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);


}

void Scene09::Shutdown()
{
}
