#include "stdafx.h"
#include "engine.h"
#include "renderer.h"
#include "scene03.h"

int main()
{
	std::shared_ptr<Engine> engine(new Engine);
	if (!engine->Initialize())
	{
		engine->Shutdown();
		exit(EXIT_FAILURE);
	}
	std::shared_ptr<Scene> scene(new Scene03(engine.get()));
	if (!scene->Initalize())
	{
		scene->Shutdown();
		engine->Shutdown();
		exit(EXIT_FAILURE);
	}

	while (!glfwWindowShouldClose(engine->Get<Renderer>()->m_window))
	{
		engine->Update();
		scene->Update();
		scene->Render();

		glfwSwapBuffers(engine->Get<Renderer>()->m_window);
	}
	scene->Shutdown();
	engine->Shutdown();


	return 0;
}