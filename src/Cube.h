#pragma once

#include "pch/pch.h"

#include "Object.h"
#include "VertexArray.h"
#include "Texture.h"


class Cube : virtual public Object
{
private:
    float m_vertices[288] = {
        -0.5f,  0.5f,  0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // front top left - 0
         0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 1.0f,  // front top right - 1
         0.5f, -0.5f,  0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // front bottom right - 2
         0.5f, -0.5f,  0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // front bottom right - 2
        -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 0.0f,  // front bottom left - 3
        -0.5f,  0.5f,  0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // front top left - 0

         0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  0.0f, 1.0f,  // front top right - 1
         0.5f,  0.5f, -0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 1.0f,  // back top right - 5
         0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // back bottom right - 6
         0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // back bottom right - 6
         0.5f, -0.5f,  0.5f,  0.6f,  0.8f,  1.0f,  0.0f, 0.0f,  // front bottom right - 2
         0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  0.0f, 1.0f,  // front top right - 1

         0.5f,  0.5f, -0.5f,  0.0f,  0.7f,  0.7f,  0.0f, 1.0f,  // back top right - 5
        -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  1.0f, 1.0f,  // back top left - 4
        -0.5f, -0.5f, -0.5f,  0.7f,  0.2f,  0.4f,  1.0f, 0.0f,  // back bottom left - 7
        -0.5f, -0.5f, -0.5f,  0.7f,  0.2f,  0.4f,  1.0f, 0.0f,  // back bottom left - 7
         0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  0.0f, 0.0f,  // back bottom right - 6
         0.5f,  0.5f, -0.5f,  0.0f,  0.7f,  0.7f,  0.0f, 1.0f,  // back top right - 5

        -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // back top left - 4
        -0.5f,  0.5f,  0.5f,  0.7f,  0.9f,  0.5f,  1.0f, 1.0f,  // front top left - 0
        -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  1.0f, 0.0f,  // front bottom left - 3
        -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  1.0f, 0.0f,  // front bottom left - 3
        -0.5f, -0.5f, -0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 0.0f,  // back bottom left - 7
        -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // back top left - 4

        -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // back top left - 4
         0.5f,  0.5f, -0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 1.0f,  // back top right - 5
         0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 0.0f,  // front top right - 1
         0.5f,  0.5f,  0.5f,  0.0f,  0.7f,  0.7f,  1.0f, 0.0f,  // front top right - 1
        -0.5f,  0.5f,  0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 0.0f,  // front top left - 0
        -0.5f,  0.5f, -0.5f,  0.7f,  0.9f,  0.5f,  0.0f, 1.0f,  // back top left - 4

        -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 1.0f,  // front bottom left - 3
         0.5f, -0.5f,  0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 1.0f,  // front bottom right - 2
         0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // back bottom right - 6
         0.5f, -0.5f, -0.5f,  0.6f,  0.8f,  1.0f,  1.0f, 0.0f,  // back bottom right - 6
        -0.5f, -0.5f, -0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 0.0f,  // back bottom left - 7
        -0.5f, -0.5f,  0.5f,  0.7f,  0.2f,  0.4f,  0.0f, 1.0f,  // front bottom left - 3
    };

    unsigned int m_indicies[36] = {
        0, 1, 2,  // front
        2, 3, 0,
        1, 5, 6, // right
        6, 2, 1,
        5, 4, 7,  // back
        7, 6, 5,
        4, 0, 3,  // left
        3, 7, 4,
        4, 5, 1,  // top
        1, 0, 4,
        3, 2, 6,  // bottom
        6, 7, 3,
    };

    VertexArray m_VAO;
    Texture m_texture;

// public:
    glm::vec3 position;

public:
    Cube(int x, int y, int z);
    // ~Cube();

    void Draw() const override;
    glm::vec3 GetPosition() const;
};