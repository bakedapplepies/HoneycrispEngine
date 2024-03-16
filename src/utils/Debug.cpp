#include "Debug.h"
#include "src/pch/pch.h"


HNCRSP_NAMESPACE_START

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, unsigned int line)
{
    while (GLenum error = glGetError()) {
        std::string errorType;
        switch (error)
        {
            case GL_INVALID_ENUM:                  errorType = "INVALID ENUM"; break;
            case GL_INVALID_VALUE:                 errorType = "INVALID VALUE"; break;
            case GL_INVALID_OPERATION:             errorType = "INVALID OPERATION"; break;
            case GL_STACK_OVERFLOW:                errorType = "STACK OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               errorType = "STACK UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 errorType = "OUT OF MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: errorType = "INVALID FRAMEBUFFER OPERATION"; break;
            case GL_CONTEXT_LOST:                  errorType = "CONTEXT LOST"; break;
            default:                               errorType = "OPENGL ERROR ENUM UNRECOGNIZED"; break;
        }
        std::cout << fmt::format("[OPENGL ERROR: {} | {}]\n    At: {} Ln: {}, File: {}\n",
            error, errorType, function, line, file
        );
        return false;
    }
    return true;
}

HNCRSP_NAMESPACE_END