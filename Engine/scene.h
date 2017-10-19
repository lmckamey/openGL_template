#pragma once
#include "engine.h"
class Scene
{
public:
	 Scene(Engine * engine) : m_engine(engine) {};
	 ~Scene() {};

	bool virtual Initalize() = 0;
	void virtual Update() = 0;
	void virtual Render() = 0;
	void virtual Shutdown() = 0;

protected:
	Engine * m_engine;
};
