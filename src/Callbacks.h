#pragma once

#include <iostream>
#include <string>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void error_callback(int error, const char *msg)
{
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    std::cerr << s << '\n';
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Exit window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Toggle wireframes
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        GLint front_back_mode[2];
        glGetIntegerv(GL_POLYGON_MODE, front_back_mode);
        if (front_back_mode[0] == GL_FILL && front_back_mode[1] == GL_FILL)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}