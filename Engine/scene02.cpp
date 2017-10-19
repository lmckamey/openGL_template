#include "glm\vec3.hpp"
#include "glm\matrix.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Scene02.h"
#include "renderer.h"
#include "stdafx.h"
#include "input.h"

namespace
{
	float positionData[] =
	{
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f,  0.8f, 0.0f,
		-0.8f, -0.8f, 0.0f
	};

	float colorData[] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	glm::vec3 vPositionData[] =
	{
		glm::vec3(-0.8f, -0.8f, 0.0f),
		glm::vec3(0.8f, -0.8f, 0.0f),
		glm::vec3(0.0f,  0.8f, 0.0f),
	};

	glm::vec3 vColorData[] =
	{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
	};

	glm::vec3 vertexData[] =
	{
		glm::vec3(-0.8f, -0.8f, 0.0f),
		glm::vec3(1.0f,  0.0f, 0.0f),
		glm::vec3(0.8f, -0.8f, 0.0f),
		glm::vec3(0.0f,  1.0f, 0.0f),
		glm::vec3(0.0f,  0.8f, 0.0f),
		glm::vec3(0.0f,  0.0f, 1.0f),
		glm::vec3(-0.8f, -0.8f, 0.0f),
		glm::vec3(1.0f,  0.0f, 0.0f),
	};

	enum vboID
	{
		POSITION,
		COLOR,
		VERTEX
	};
	float m_x;
	float m_y;
	float m_cursorSize;

	GLuint vaoHandle;
	GLuint m_shaderProgram;

	glm::mat4 mxTranslate;
	glm::mat4 mxScale;

}

Scene02::~Scene02()
{
}

bool Scene02::Initalize()
{

	m_cursorSize = 0.2f;

	mxScale = glm::scale(glm::mat4(1.0f), glm::vec3(m_cursorSize));

	m_shaderProgram = m_engine->Get<Renderer>()->CreateShaderProgram("..\\Resources\\Shaders\\transform.vert", "..\\Resources\\Shaders\\basic.frag");

	GLuint vboHandles[3];
	glGenBuffers(3, vboHandles);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[COLOR]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);


	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexBuffer(0, vboHandles[POSITION], 0, sizeof(glm::vec3));
	glBindVertexBuffer(1, vboHandles[COLOR], 0, sizeof(glm::vec3));

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);

	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);

	m_engine->Get<Input>()->AddAnalog("x-axis", Input::eAnalogType::MOUSE_X,0);
	m_engine->Get<Input>()->AddAnalog("y-axis", Input::eAnalogType::MOUSE_Y,0);
	m_engine->Get<Input>()->AddButton("click", Input::eButtonType::MOUSE, 0);
	Input::eButtonState state = m_engine->Get<Input>()->GetButton("click");

	return true;
}

void Scene02::Update()
{
	float x = m_engine->Get<Input>()->GetAnalogAbsolute("x-axis");
	float y = m_engine->Get<Input>()->GetAnalogAbsolute("y-axis");

	m_x = ((x / m_engine->Get <Renderer>()->m_width) * 2.0f) - 1.0f;
	m_y = ((y / m_engine->Get <Renderer>()->m_height)*2.0f) - 1.0f;
	mxTranslate = (glm::translate(glm::mat4(1.0f),glm::vec3(m_x,-m_y,0.0f))*mxScale);

	GLuint matrixUniform = glGetUniformLocation(m_shaderProgram, "modelMatrix");
	glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &mxTranslate[0][0]);
	glfwSetInputMode(m_engine->Get<Renderer>()->m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Scene02::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render code
	glBindVertexArray(vaoHandle);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Scene02::Shutdown()
{
}
