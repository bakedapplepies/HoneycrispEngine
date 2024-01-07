#include "Debug.h"


static bool err = false;
void createBreak()
{
    err = true;
}

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec2)
{
    std::cout << fmt::format("[{}, {}]", vec2.x, vec2.y);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec3)
{
    std::cout << fmt::format("[{}, {}, {}]", vec3.x, vec3.y, vec3.z);
    return stream;
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

void terminate(const char* msg, const char* file, unsigned int line)
{
    glfwTerminate();
    std::cout << fmt::format("[TERMINATE at {}, Ln: {}]: {}\n", file, line, msg);
    assert(false);
}