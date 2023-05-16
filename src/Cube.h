#pragma once

#include "pch/pch.h"

#include "Texture.h"
#include "Mesh.h"


extern Texture grassTextureMap;
class Cube : public Mesh
{
private:
    const std::vector<float> vertices = {
        // front
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        // right
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,

        // back
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,

        // left
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,

        // top
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // bottom
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
    };

    std::vector<float> normals = {
        // front
         0.0f,  0.0f, -1.0f, 
         0.0f,  0.0f, -1.0f, 
         0.0f,  0.0f, -1.0f, 
         0.0f,  0.0f, -1.0f, 
        
        // right
         1.0f,  0.0f,  0.0f, 
         1.0f,  0.0f,  0.0f, 
         1.0f,  0.0f,  0.0f, 
         1.0f,  0.0f,  0.0f, 

        // back
         0.0f,  0.0f,  1.0f, 
         0.0f,  0.0f,  1.0f, 
         0.0f,  0.0f,  1.0f, 
         0.0f,  0.0f,  1.0f, 

        // right
        -1.0f,  0.0f,  0.0f, 
        -1.0f,  0.0f,  0.0f, 
        -1.0f,  0.0f,  0.0f, 
        -1.0f,  0.0f,  0.0f, 

        // top
         0.0f,  1.0f,  0.0f, 
         0.0f,  1.0f,  0.0f, 
         0.0f,  1.0f,  0.0f, 
         0.0f,  1.0f,  0.0f, 

        // bottom
         0.0f, -1.0f,  0.0f, 
         0.0f, -1.0f,  0.0f, 
         0.0f, -1.0f,  0.0f, 
         0.0f, -1.0f,  0.0f, 
    };

    TextureCoords grassSideCoords = grassTextureMap.GetTextureCoords(Textures::GRASS_SIDE);
    TextureCoords grassTopCoords = grassTextureMap.GetTextureCoords(Textures::GRASS_TOP);
    TextureCoords dirtCoords = grassTextureMap.GetTextureCoords(Textures::DIRT);

    std::vector<float> uv = {
        // top
        grassSideCoords.tl.x, grassSideCoords.tl.y,
        grassSideCoords.tr.x, grassSideCoords.tr.y,
        grassSideCoords.br.x, grassSideCoords.br.y,
        grassSideCoords.bl.x, grassSideCoords.bl.y,

        // right
        grassSideCoords.tl.x, grassSideCoords.tl.y,
        grassSideCoords.tr.x, grassSideCoords.tr.y,
        grassSideCoords.br.x, grassSideCoords.br.y,
        grassSideCoords.bl.x, grassSideCoords.bl.y,

        // back
        grassSideCoords.tl.x, grassSideCoords.tl.y,
        grassSideCoords.tr.x, grassSideCoords.tr.y,
        grassSideCoords.br.x, grassSideCoords.br.y,
        grassSideCoords.bl.x, grassSideCoords.bl.y,

        // left
        grassSideCoords.tl.x, grassSideCoords.tl.y,
        grassSideCoords.tr.x, grassSideCoords.tr.y,
        grassSideCoords.br.x, grassSideCoords.br.y,
        grassSideCoords.bl.x, grassSideCoords.bl.y,

        // top
        grassTopCoords.tl.x, grassTopCoords.tl.y,
        grassTopCoords.tr.x, grassTopCoords.tr.y,
        grassTopCoords.br.x, grassTopCoords.br.y,
        grassTopCoords.bl.x, grassTopCoords.bl.y,

        // bottom
        dirtCoords.tl.x, dirtCoords.tl.y,
        dirtCoords.tr.x, dirtCoords.tr.y,
        dirtCoords.br.x, dirtCoords.br.y,
        dirtCoords.bl.x, dirtCoords.bl.y,
    };

    std::vector<unsigned int> indices = {
        // top
        0, 1, 2,
        0, 2, 3,

        // right
        4, 5, 6,
        4, 6, 7,

        // back
        8, 9, 10,
        8, 10, 11,

        // left
        12, 13, 14,
        12, 14, 15,

        // top
        16, 17, 18,
        16, 18, 19,

        // back
        20, 21, 22,
        20, 22, 23
    };

public:
    Cube() = default;
    Cube(const glm::vec3& position);
    Cube(const Cube& other) = delete;
    Cube(Cube&& other) noexcept;
    Cube& operator=(const Cube& other) = delete;
    Cube& operator=(Cube&& other) noexcept;
    ~Cube() override;
};