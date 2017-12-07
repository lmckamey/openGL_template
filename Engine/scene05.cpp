#include "glm\vec3.hpp"
#include "glm\matrix.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "stb_image.h"
#include "tiny_obj_loader.h"
#include "Scene05.h"
#include "timer.h"
#include "renderer.h"
#include "stdafx.h"
#include "meshLoader.h"
#include "input.h"
#include "light.h"
#define TEXTURES


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

Scene05::~Scene05()
{
}

bool Scene05::Initalize()
{

	GLint bpp;
	GLint height;
	GLint width;

	m_engine->Get<Input>()->AddButton("escape", Input::eButtonType::KEYBOARD, GLFW_KEY_ESCAPE);

	Camera* camera = new Camera("camera", this);
	camera->Initialize(glm::vec3(0.0f, 1.0f, 1.5f), glm::vec3(0));
	AddObject(camera);

	Light* light = new Light("light", this);
	light->ambient = glm::vec3(1);
	light->diffuse = glm::vec3(1);
	light->specular = glm::vec3(1);
	light->m_transform.m_position = glm::vec3(0);

	AddObject(light);


#ifdef PHONG
	m_shader.CompileShader("..\\Resources\\Shaders\\phong.vert.shader", GL_VERTEX_SHADER);
	m_shader.CompileShader("..\\Resources\\Shaders\\phong.frag.shader", GL_FRAGMENT_SHADER);
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

	m_shader.Link();
	m_shader.Use();

	m_material.SetMaterial(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 100.0f);
	m_material.LoadTexture2D("..\\Resources\\Textures\\crate.bmp", GL_TEXTURE0);
	m_material.LoadTexture2D("..\\Resources\\Textures\\crate_specular.bmp", GL_TEXTURE1);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	MeshLoader::LoadMesh("..\\Resources\\Meshes\\cube.obj", vertices, normals, uvs);


	m_numOfVerticies = vertices.size();


	GLuint vboHandles[3];
	glGenBuffers(3, vboHandles);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data() , GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);


	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	
	glBindVertexBuffer(0, vboHandles[0], 0, sizeof(glm::vec3));
	glBindVertexBuffer(1, vboHandles[1], 0, sizeof(glm::vec3));
	glBindVertexBuffer(2, vboHandles[2], 0, sizeof(glm::vec2));


	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);

	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);

	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);


	return true;
}

void Scene05::Update()
{
	m_rotation = m_rotation + m_engine->Get<Timer>()->FrameTime();
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	rotate = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(1.0f, 0.0f, 0.0f));
	mxModel = translate * rotate;

	Camera* camera = GetObject<Camera>("camera");
	mxView = camera->GetView();
	mxProjection = camera->GetProjection();

	MVP = mxProjection * mxView * mxModel;
	m_shader.SetUniform("mxMVP", MVP);

	mxModelView = mxView * mxModel;
	m_shader.SetUniform("mxModelView", mxModelView);

	mxNormal = glm::mat3(mxModelView);
	mxNormal = glm::inverse(mxNormal);
	mxNormal = glm::transpose(mxNormal);
	m_shader.SetUniform("mxNormal", mxNormal);


	Light* light = GetObject<Light>("light");
	glm::vec3 lightPosition = mxView * glm::vec4(light->m_transform.m_position, 1.0f);
	m_shader.SetUniform("lightPosition", lightPosition);

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_shader.SetUniform("lightColor", lightColor);

	m_shader.SetUniform("material.ambient", m_material.m_ambient);
	m_shader.SetUniform("material.diffuse", m_material.m_diffuse);
	m_shader.SetUniform("material.specular", m_material.m_specular);

	if (m_engine->Get<Input>()->GetButton("escape") == Input::eButtonState::DOWN)
	{
		glfwSetWindowShouldClose(m_engine->Get<Renderer>()->m_window, GLFW_TRUE);
	}

	auto objects = GetObjects<Object>();
	for (auto object : objects)
	{
		object->Update();
	}

}

void Scene05::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render code

	m_material.SetTextures();
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, m_numOfVerticies);

	glfwSwapBuffers(m_engine->Get<Renderer>()->m_window);


}

void Scene05::Shutdown()
{
}
