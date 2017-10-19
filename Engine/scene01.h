#pragma once
#include "scene.h"
class Scene01 :
	public Scene
{
public:
	Scene01(Engine* e) : Scene(e) {}
	~Scene01();

	bool Initalize();
	void Update();
	void Render();
	void Shutdown();

};

