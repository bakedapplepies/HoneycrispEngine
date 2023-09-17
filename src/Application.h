#pragma once

#include "pch/pch.h"
#include "Window.h"


class Application
{
private:
	Application() = default;
	~Application();

private:
	static Application* ContextWrap;
	Window* window;

public:
	Application(const Application&) = delete;
	Application operator=(const Application&) = delete;

	static Application* Get();
	void OnUpdate();
	void OnImGUI();
	int Run();
};