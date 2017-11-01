#pragma once
#include"glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"
class Transform
{
public:
	Transform();
	~Transform();

	glm::mat4 GetMatrix44();

public:
	
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::quat m_rotation;
};

