#pragma once
#include <vector>
#include "engine.h"

class Object;
class Scene
{
public:
	 Scene(Engine * engine) : m_engine(engine) {};
	 virtual ~Scene() 
	 {
		 for (auto object : m_objects)
		 {
			 delete object;
		 }
	 };

	bool virtual Initalize() = 0;
	void virtual Update() = 0;
	void virtual Render() = 0;
	void virtual Shutdown() = 0;

	void AddObject(Object* object)
	{
		m_objects.push_back(object);
	}

	template<typename T>
	T* GetObject(const std::string& name);

	template<typename T>
	std::vector<T*> GetObjects();

public:
	Engine * m_engine;


protected:
	std::vector<Object*> m_objects;
};

template<typename T>
inline T * Scene::GetObject(const std::string & name)
{
	for (auto object : m_objects)
	{
		if (object->m_name == name)
		{
			return dynamic_cast<T*>(object);
		}

	}
	return nullptr;
}

template<typename T>
inline std::vector<T*> Scene::GetObjects()
{
	std::vector<T*> objects;
	for (auto object : m_objects)
	{
		auto tempObj = dynamic_cast<T*>(object);
		if (tempObj != nullptr)
		{
			objects.push_back(tempObj);
		}
	}
	return objects;
}
