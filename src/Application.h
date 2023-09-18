#pragma once

#include "pch/pch.h"
#include "Window.h"


class Application
{
private:
	Application() = default;
	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application operator=(const Application&) = delete;
	Application operator=(Application&&) = delete;

private:
	static std::shared_ptr<Application> ContextWrap;
	Window* window;

public:
	~Application();

	static std::shared_ptr<Application> Get();
	void OnUpdate();
	void OnImGUI();
	void Run();
};