#include "DefaultSceneTwo.h"


DefaultSceneTwo::DefaultSceneTwo()
{
    cube = CreateObject(Cube(), EObjectRenderType::STATIC);
    cube->AddPosition(glm::vec3(1.0f, 3.0f ,5.0f));
    cube->AddPosition(glm::vec3(3.0f, 2.0f ,1.0f));
    cube->AddPosition(glm::vec3(-5.0f, -2.0f ,3.0f));

    TextureCoords& grassUV = Textures::mainTextureMap.GetTextureCoords(0, 0);
    int width = 50, height = 50;
    int vertW = width+1, vertH = height+1;
    int totalVerts = vertW * vertH;
    glm::vec2 uvDistVertical = grassUV.tl - grassUV.bl;
    glm::vec2 uvDistHorizontal = grassUV.br - grassUV.bl;

    std::vector<glm::vec3> vertices;
    vertices.reserve(totalVerts);
    for (int i = 0; i < vertH; i++)
    {
        for (int j = 0; j < vertW; j++)
        {
            vertices.emplace_back(glm::vec3((float)i, 0.0f, (float)j));
        }
    }

    std::vector<glm::vec3> colors;
    colors.reserve(totalVerts);
    for (int i = 0; i < totalVerts; i++)
    {
        colors.emplace_back(glm::vec3(0.369f, 0.616f, 0.204f));
    }

    std::vector<glm::vec3> normals;
    normals.reserve(totalVerts);
    for (int i = 0; i < totalVerts; i++)
    {
        normals.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));
    }

    std::vector<glm::vec2> uvs;
    uvs.reserve(totalVerts);
    for (int i = 0; i < vertW; i++)
    {
        for (int j = 0; j < vertH; j++)
        {
            uvs.emplace_back(grassUV.bl + 
            uvDistHorizontal * ((float)i/(float)(vertW-1)) +
            uvDistVertical * ((float)j/(float)(vertH-1)));
        }
    }

    std::vector<GLuint> indices;
    indices.reserve(width*height*6);
    for (GLuint h = 0; h < height; h++)
    {
        for (GLuint w = 0; w < width; w++)
        {
            indices.emplace_back(h*vertW + w);
            indices.emplace_back((h+1)*vertW + w);
            indices.emplace_back(h*vertW + w+1);

            indices.emplace_back((h+1)*vertW + w+1);
            indices.emplace_back(h*vertW + w+1);
            indices.emplace_back((h+1)*vertW + w);
        }
    }

    mesh = CreateObject(Mesh(), EObjectRenderType::STATIC);
    mesh->vertices = vertices;
    mesh->colors = colors;
    mesh->normals = normals;
    mesh->uvs = uvs;
    mesh->indices = indices;

    mesh->ConstructMesh();
    mesh->AddPosition(glm::vec3(0.0f, -6.0f, 0.0f));
}

void DefaultSceneTwo::OnUpdate(Shader& shader)
{
    cube->Draw(shader);
    mesh->Draw(shader);
}