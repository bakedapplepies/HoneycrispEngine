#include "pch/pch.h"
#include "Window.h"


class OGLWrapper
{
private:
	OGLWrapper() = default;
	~OGLWrapper();

private:
	static OGLWrapper* ContextWrap;
	Window* window;

public:
	OGLWrapper(const OGLWrapper&) = delete;
	OGLWrapper operator=(const OGLWrapper&) = delete;

	static OGLWrapper* Get();
	int Run();
};