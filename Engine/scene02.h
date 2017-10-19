#pragma once
#include "scene.h"
class Scene02 :
	public Scene
{
public:
	Scene02(Engine* e) : Scene(e) {}
	~Scene02();

	bool Initalize();
	void Update();
	void Render();
	void Shutdown();

};

