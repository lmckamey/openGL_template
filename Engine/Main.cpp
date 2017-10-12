#include <iostream>
#include "stdafx.h"
#include "engine.h"
#include "input.h"
#include "renderer.h"
#include "glm\vec3.hpp"

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

int main()
{
	std::shared_ptr<Engine> engine(new Engine);
	if (!engine->Initialize())
	{
		engine->Shutdown();
		exit(EXIT_FAILURE);
	}

	GLuint program = engine->Get<Renderer>()->CreateShaderProgram("..\\Resources\\Shaders\\basic.vert", "..\\Resources\\Shaders\\basic.frag");

	GLuint vboHandles[3];
	glGenBuffers(3, vboHandles);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[COLOR]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

	GLuint vaoHandle;
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

	while (!glfwWindowShouldClose(engine->Get<Renderer>()->m_window))
	{
		engine->Update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render code
		glBindVertexArray(vaoHandle);
		glDrawArrays(GL_LINE_STRIP, 0, 4);
		glBindVertexArray(0);

		glfwSwapBuffers(engine->Get<Renderer>()->m_window);
	}
	engine->Shutdown();


	return 0;
}