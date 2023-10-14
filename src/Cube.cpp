#include "Debug.h"
#include "Cube.h"
#include "core/Texture.h"


Cube::Cube()
{
    InitializeAttributeData();
    ConstructMesh();
}

// Cube::Cube(Cube&& other) noexcept
//     : Mesh(std::move(other))
// {
    
// }

// Cube& Cube::operator=(Cube&& other) noexcept
// {
//     positions = std::move(other.positions);
//     this->GetVAO() = std::move(other.GetVAO());

//     return *this;
// }

void Cube::InitializeAttributeData()
{
    _verticesPos = std::vector<glm::vec3> {
        // front
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),

        // right
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f,  0.5f),

        // back
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),

        // left
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),

        // top
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),

        // bottom
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
    };

    _colors = std::vector<glm::vec3> {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),

        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),

        glm::vec3(0.369f, 0.616f, 0.204f),
        glm::vec3(0.369f, 0.616f, 0.204f),
        glm::vec3(0.369f, 0.616f, 0.204f),
        glm::vec3(0.369f, 0.616f, 0.204f),

        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
    };

    _normals = std::vector<glm::vec3> {
        // front
        glm::vec3(0.0f,  0.0f,  1.0f), 
        glm::vec3(0.0f,  0.0f,  1.0f), 
        glm::vec3(0.0f,  0.0f,  1.0f), 
        glm::vec3(0.0f,  0.0f,  1.0f), 
        
        // right
        glm::vec3(1.0f,  0.0f,  0.0f), 
        glm::vec3(1.0f,  0.0f,  0.0f), 
        glm::vec3(1.0f,  0.0f,  0.0f), 
        glm::vec3(1.0f,  0.0f,  0.0f), 

        // back
        glm::vec3(0.0f,  0.0f, -1.0f), 
        glm::vec3(0.0f,  0.0f, -1.0f), 
        glm::vec3(0.0f,  0.0f, -1.0f), 
        glm::vec3(0.0f,  0.0f, -1.0f), 

        // right
        glm::vec3(-1.0f,  0.0f,  0.0f), 
        glm::vec3(-1.0f,  0.0f,  0.0f), 
        glm::vec3(-1.0f,  0.0f,  0.0f), 
        glm::vec3(-1.0f,  0.0f,  0.0f), 

        // top
        glm::vec3(0.0f,  1.0f,  0.0f), 
        glm::vec3(0.0f,  1.0f,  0.0f), 
        glm::vec3(0.0f,  1.0f,  0.0f), 
        glm::vec3(0.0f,  1.0f,  0.0f), 

        // bottom
        glm::vec3(0.0f, -1.0f,  0.0f), 
        glm::vec3(0.0f, -1.0f,  0.0f), 
        glm::vec3(0.0f, -1.0f,  0.0f), 
        glm::vec3(0.0f, -1.0f,  0.0f), 
    };

    TextureCoords& grassSideCoords = Textures::mainTextureMap.GetTextureCoords(1, 0);
    TextureCoords& grassTopCoords = Textures::mainTextureMap.GetTextureCoords(0, 0);
    TextureCoords& dirtCoords = Textures::mainTextureMap.GetTextureCoords(2, 0);

    _uvs = std::vector<glm::vec2> {
        // top
        glm::vec2(grassSideCoords.tl.x, grassSideCoords.tl.y),
        glm::vec2(grassSideCoords.tr.x, grassSideCoords.tr.y),
        glm::vec2(grassSideCoords.br.x, grassSideCoords.br.y),
        glm::vec2(grassSideCoords.bl.x, grassSideCoords.bl.y),

        // right
        glm::vec2(grassSideCoords.tl.x, grassSideCoords.tl.y),
        glm::vec2(grassSideCoords.tr.x, grassSideCoords.tr.y),
        glm::vec2(grassSideCoords.br.x, grassSideCoords.br.y),
        glm::vec2(grassSideCoords.bl.x, grassSideCoords.bl.y),

        // back
        glm::vec2(grassSideCoords.tl.x, grassSideCoords.tl.y),
        glm::vec2(grassSideCoords.tr.x, grassSideCoords.tr.y),
        glm::vec2(grassSideCoords.br.x, grassSideCoords.br.y),
        glm::vec2(grassSideCoords.bl.x, grassSideCoords.bl.y),

        // left
        glm::vec2(grassSideCoords.tl.x, grassSideCoords.tl.y),
        glm::vec2(grassSideCoords.tr.x, grassSideCoords.tr.y),
        glm::vec2(grassSideCoords.br.x, grassSideCoords.br.y),
        glm::vec2(grassSideCoords.bl.x, grassSideCoords.bl.y),

        // top
        glm::vec2(grassTopCoords.tl.x, grassTopCoords.tl.y),
        glm::vec2(grassTopCoords.tr.x, grassTopCoords.tr.y),
        glm::vec2(grassTopCoords.br.x, grassTopCoords.br.y),
        glm::vec2(grassTopCoords.bl.x, grassTopCoords.bl.y),

        // bottom
        glm::vec2(dirtCoords.tl.x, dirtCoords.tl.y),
        glm::vec2(dirtCoords.tr.x, dirtCoords.tr.y),
        glm::vec2(dirtCoords.br.x, dirtCoords.br.y),
        glm::vec2(dirtCoords.bl.x, dirtCoords.bl.y),
    };

    _indices = std::vector<GLuint> {
        // front
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

    static_cast<Mesh*>(this)->vertices = _verticesPos;
    static_cast<Mesh*>(this)->colors = _colors;
    static_cast<Mesh*>(this)->normals = _normals;
    static_cast<Mesh*>(this)->uvs = _uvs;
    static_cast<Mesh*>(this)->indices = _indices;
}
