#pragma once

#include "system.h"
#include <map>

class Renderer;

class Input : public System
{
public:
	enum class eButtonState
	{
		INVALID,
		UP,
		DOWN,
		HELD,
		RELEASED,
	};

	enum class eButtonType
	{
		INVALID,
		KEYBOARD,
		MOUSE,
		JOYSTICK,
	};

	enum class eAnalogType
	{
		INVALID,
		MOUSE_X,
		MOUSE_Y,
		MOUSE_Z,
		JOYSTICK_X,
		JOYSTICK_Y,
	};

	typedef struct tButtonInfo
	{
		int id;
		eButtonType type;
		eButtonState state;
	} ButtonInfo;

	typedef struct tAnalogInfo
	{
		int id;
		eAnalogType type;
		float valueAbsolute;
		float valueRelative;
	} AnalogInfo;
	
public:
	Input(Engine* engine);
	~Input();

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	bool Initialize();
	void Shutdown();
	void Update();
	
	void AddButton(const std::string name, eButtonType type, int id);
	eButtonState GetButton(const std::string name);

	void AddAnalog(const std::string name, eAnalogType type, int id);
	float GetAnalogAbsolute(const std::string name);
	float GetAnalogRelative(const std::string name);


public:
	static float s_scrollX;
	static float s_scrollY;


private:
	std::map<std::string, ButtonInfo> m_buttons;
	std::map<std::string, AnalogInfo> m_analogs;
};

