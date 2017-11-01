#include "transform.h"
#include "glm\gtc\matrix_transform.hpp"


Transform::Transform()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rotation = glm::quat(glm::vec3(0.0f));
	m_scale = glm::vec3(1.0f);
}



Transform::~Transform()
{
}

glm::mat4 Transform::GetMatrix44()
{
	glm::mat4 mxt = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 mxr = glm::mat4_cast(m_rotation);
	glm::mat4 mxs = glm::scale(glm::mat4(1.0f), m_scale);

	return mxt * mxr * mxs;
}

