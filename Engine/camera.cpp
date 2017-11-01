#include "stdafx.h"

#include "camera.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(std::string id, Scene* scene) : Object(id, scene)
{
	//
}


Camera::~Camera()
{
	//
}

void Camera::Initialize(const glm::vec3& position, const glm::vec3& target, const Camera::Data& data)
{
	m_transform.m_position = position;
	m_target = target;
	m_data = data;

	m_mxView = glm::lookAt(m_transform.m_position, m_target, glm::vec3(0.0f, 1.0f, 0.0f));
	m_transform.m_rotation = glm::quat_cast(m_mxView);

	float aspect = (float)Renderer::m_width / (float)Renderer::m_height;
	m_mxProjection =  glm::perspective(m_data.fov, aspect, m_data.nearClip, m_data.farClip);
	
	// set input
	m_scene->m_engine->Get<Input>()->AddButton("camera rotateA", Input::eButtonType::KEYBOARD, GLFW_KEY_LEFT_ALT);
	m_scene->m_engine->Get<Input>()->AddButton("camera rotateAB", Input::eButtonType::MOUSE, 0);
	m_scene->m_engine->Get<Input>()->AddButton("camera rotateC", Input::eButtonType::MOUSE, 1);
	m_scene->m_engine->Get<Input>()->AddButton("camera translate", Input::eButtonType::MOUSE, 2);

	m_scene->m_engine->Get<Input>()->AddAnalog("camera x", Input::eAnalogType::MOUSE_X, 0);
	m_scene->m_engine->Get<Input>()->AddAnalog("camera y", Input::eAnalogType::MOUSE_Y, 0);
	m_scene->m_engine->Get<Input>()->AddAnalog("camera z", Input::eAnalogType::MOUSE_Z, 0);
}

void Camera::Update()
{
	glm::vec3 translate(0.0f);
	glm::vec3 rotate(0.0f);
	UpdateInput(translate, rotate);
	
	if (m_data.type == eType::EDITOR) UpdateTransformEditor(translate, rotate);
	else if (m_data.type == eType::ORBIT) UpdateTransformOrbit(translate, rotate);
}

void Camera::UpdateInput(glm::vec3& translate, glm::vec3& rotate)
{
	bool translateAction = m_scene->m_engine->Get<Input>()->GetButton("camera translate") == Input::eButtonState::HELD;

	float x = m_scene->m_engine->Get<Input>()->GetAnalogRelative("camera x");
	float y = m_scene->m_engine->Get<Input>()->GetAnalogRelative("camera y");
	float z = m_scene->m_engine->Get<Input>()->GetAnalogRelative("camera z");

	float rate = 0.01f;
	if (translateAction)
	{
		translate.x += -x * rate;
		translate.y +=  y * rate;
	}

	translate.z += -z * 0.8f;

	bool rotateAction = (m_scene->m_engine->Get<Input>()->GetButton("camera rotateC") == Input::eButtonState::HELD || 
						(m_scene->m_engine->Get<Input>()->GetButton("camera rotateA") == Input::eButtonState::HELD && m_scene->m_engine->Get<Input>()->GetButton("camera rotateAB") == Input::eButtonState::HELD));
	if (rotateAction)
	{
		rotate.x += y * rate;
		rotate.y += x * rate;
	}
}

void Camera::UpdateTransformEditor(const glm::vec3& translate, const glm::vec3& rotate)
{
	glm::quat qpitch = glm::angleAxis(rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat qyaw = glm::angleAxis(rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_transform.m_rotation = qpitch * m_transform.m_rotation * qyaw;
	m_transform.m_rotation = glm::normalize(m_transform.m_rotation);

	m_transform.m_position += (translate * m_transform.m_rotation);

	glm::mat4 mxTranslation = glm::translate(glm::mat4(1.0), -m_transform.m_position);
	glm::mat4 mxRotation = glm::mat4_cast(m_transform.m_rotation);

	m_mxView = mxRotation * mxTranslation;
}

void Camera::UpdateTransformOrbit(const glm::vec3& translate, const glm::vec3& rotate)
{
	glm::quat qpitch = glm::angleAxis(-rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat qyaw = glm::angleAxis(-rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_transform.m_rotation = m_transform.m_rotation * qyaw * qpitch;
	m_transform.m_rotation = glm::normalize(m_transform.m_rotation);

	m_data.distance = m_data.distance + translate.z;
	m_transform.m_position = (m_transform.m_rotation * glm::vec3(0.0f, 0.0f, m_data.distance)) + m_target;
	glm::vec3 up = m_transform.m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	m_mxView = glm::lookAt(m_transform.m_position, m_target, up);
}

void Camera::SetTarget(const glm::vec3& target)
{
	m_target = target;

	m_mxView = glm::lookAt(m_transform.m_position, m_target, glm::vec3(0.0f, 1.0f, 0.0f));
	m_transform.m_rotation = glm::quat_cast(m_mxView);
}
