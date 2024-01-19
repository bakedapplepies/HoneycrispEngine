#pragma once

#include "src/pch/pch.h"
#include "Window.h"


HNCRSP_NAMESPACE_START

class Application
{
private:
	Application() = default;
	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application operator=(const Application&) = delete;
	Application operator=(Application&&) = delete;

private:
	static Application* m_ContextWrap;
	std::unique_ptr<Window> window;

public:
	~Application();

	static Application& Get();
	static void DeleteInstance();
	void Run();
};

HNCRSP_NAMESPACE_END