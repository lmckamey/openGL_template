#include "glm\vec3.hpp"
#include "glm\matrix.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Scene04.h"
#include "timer.h"
#include "renderer.h"
#include "stdafx.h"
#include "image.h"
//#define PHONG


namespace
{
	float vertexData[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f

	};



	enum vboID
	{
		POSITION,
		COLOR,
		VERTEX
	};

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

Scene04::~Scene04()
{
}

bool Scene04::Initalize()
{

#ifdef PHONG
	m_cube.shaderProgram = m_engine->Get<Renderer>()->CreateShaderProgram("..\\Resources\\Shaders\\phong.vert.shader", "..\\Resources\\Shaders\\phong.frag.shader");
#else
	m_cube.shaderProgram = m_engine->Get<Renderer>()->CreateShaderProgram("..\\Resources\\Shaders\\texture_phong.vert.shader", "..\\Resources\\Shaders\\texture_phong.frag");
#endif // PHONG
	GLint bpp;
	GLint height;
	GLint width;
	const unsigned char* data = Image::LoadBMP("/../Resources/Textures/crate.bmp", width, height, bpp);

	GLuint textures[2];
	
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1,textures);
	
	glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);
	if (bpp == 32)
	{
		glTextureStorage2D(GL_TEXTURE0,0, GL_RGBA, height, width);
		glTexImage2D(GL_TEXTURE0,0,GL_RGBA,width,height,0,GL_UNSIGNED_BYTE,GL_RGBA, data);
		//glTextureSubImage2D(GL_TEXTURE_2D, GL_TEXTURE0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_INT, data);
		glGenerateMipmap(GL_TEXTURE0);
		glGetUniformLocation(m_cube.shaderProgram, "IDK");
		delete data;
	}
	else if (bpp == 24)
	{
		glTextureStorage2D(GL_TEXTURE0, 1, GL_RGB, height, width);
		glTexImage2D(GL_TEXTURE0,0,GL_RGB,width,height,0,GL_UNSIGNED_BYTE,GL_RGB, data);
		//glTextureSubImage2D(GL_TEXTURE_2D, GL_TEXTURE0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_INT, data);
		glGenerateMipmap(GL_TEXTURE0);
		glGetUniformLocation(m_cube.shaderProgram, "IDK");
		delete data;
	}

	GLuint vboHandles[3];
	glGenBuffers(3, vboHandles);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);


	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexBuffer(0, vboHandles[VERTEX], 0, sizeof(glm::vec3) * 2 + sizeof(float) * 2);
	glBindVertexBuffer(1, vboHandles[VERTEX], sizeof(glm::vec3), sizeof(glm::vec3) * 2 + sizeof(float) * 2);
	glBindVertexBuffer(2, vboHandles[VERTEX], sizeof(glm::vec3)*2, sizeof(glm::vec3) * 2 + sizeof(float) *2);

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);

	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);

	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);

	m_cube.mxModelViewUniform = glGetUniformLocation(m_cube.shaderProgram, "mxModelView");
	m_cube.mxMVPUniform = glGetUniformLocation(m_cube.shaderProgram, "mxMVP");
	m_cube.mxNormalUniform = glGetUniformLocation(m_cube.shaderProgram, "mxNormal");

	m_cube.ambientMaterialUniform = glGetUniformLocation(m_cube.shaderProgram, "ambientMaterial");
	m_cube.diffuseMaterialUniform = glGetUniformLocation(m_cube.shaderProgram, "diffuseMaterial");
	m_cube.specularMaterialUniform = glGetUniformLocation(m_cube.shaderProgram, "specularMaterial");

	m_light.positionUniform = glGetUniformLocation(m_cube.shaderProgram, "lightPosition");
	m_light.colorUniform = glGetUniformLocation(m_cube.shaderProgram, "lightColor");



	return true;
}

void Scene04::Update()
{
	m_rotation = m_rotation + m_engine->Get<Timer>()->FrameTime();
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	rotate = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(1.0f, 0.0f, 0.0f));
	mxModel = translate * rotate;
	mxView = glm::lookAt(glm::vec3(0.0f, 1.0f, 1.5f), glm::vec3(0), glm::vec3(0, 1, 0));
	mxProjection = glm::perspective(90.0f, (float)m_engine->Get<Renderer>()->m_width / (float)m_engine->Get<Renderer>()->m_height, 0.1f, 1000.0f);

	mxModelView = mxView * mxModel;

	MVP = mxProjection * mxView * mxModel;
	glUniformMatrix4fv(m_cube.mxMVPUniform, 1, GL_FALSE, &MVP[0][0]);

	mxNormal = glm::mat3(mxModelView);
	glm::vec3 ambientMaterial = glm::vec3(0.2f, 0.2f, 0.2f);
	glUniform3fv(m_cube.ambientMaterialUniform, 1, &ambientMaterial[0]);

	glUniformMatrix4fv(m_cube.mxModelViewUniform, 1, GL_FALSE, &mxModelView[0][0]);

	glm::mat3 mxNormal = glm::mat3(mxModelView);
	mxNormal = glm::inverse(mxNormal);
	mxNormal = glm::transpose(mxNormal);
	glUniformMatrix3fv(m_cube.mxNormalUniform, 1, GL_FALSE, &mxNormal[0][0]);

	glm::vec3 lightPosition = mxView * glm::vec4(2.0f, 10.0f, 10.0f, 1.0f);
	glUniform4fv(m_light.positionUniform, 1, &lightPosition[0]);

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(m_light.colorUniform, 1, &lightColor[0]);

	glm::vec3 diffuseMaterial = glm::vec3(0.0f, 0.0f, 1.0f);
	glUniform3fv(m_cube.diffuseMaterialUniform, 1, &diffuseMaterial[0]);

	glm::vec3 specularMaterial = glm::vec3(1.0f, 0.0f, 0.0f);
	glUniform3fv(m_cube.specularMaterialUniform, 1, &specularMaterial[0]);

}

void Scene04::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render code
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Scene04::Shutdown()
{
}
