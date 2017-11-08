#pragma once
#include "stdafx.h"
#include "transform.h"
#include "scene.h"


class Object
{
public:
	Object(){}
	Object(std::string name, Scene* scene):m_scene(scene), m_name(name){}
	virtual ~Object() {}

	virtual void Update();

public:
	std::string m_name;
	Transform m_transform;

protected:

	Scene* m_scene;
};

