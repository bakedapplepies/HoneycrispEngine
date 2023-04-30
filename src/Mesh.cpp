#include "Mesh.h"


Mesh::~Mesh()
{
    delete[] m_vertices;
}

void Mesh::ConstructMesh()
{
    unsigned int vertSize = vertices.size()*3 + normals.size()*3 + uv.size()*2;
    m_vertices = new float[vertSize];
    
    for (int vertIndex = 0; vertIndex < vertices.size(); vertIndex++)
    {
        m_vertices[vertIndex*8 + 0] = vertices[vertIndex*3 + 0].x;
        m_vertices[vertIndex*8 + 1] = vertices[vertIndex*3 + 1].y;
        m_vertices[vertIndex*8 + 2] = vertices[vertIndex*3 + 2].z;

        m_vertices[vertIndex*8 + 3] = normals[vertIndex*3 + 0].x;
        m_vertices[vertIndex*8 + 4] = normals[vertIndex*3 + 0].y;
        m_vertices[vertIndex*8 + 5] = normals[vertIndex*3 + 0].z;

        m_vertices[vertIndex*8 + 6] = uv[vertIndex*2 + 0].x;
        m_vertices[vertIndex*8 + 7] = uv[vertIndex*2 + 0].y;
    }

    m_indices = new unsigned int[indices.size()];
    
    for (int index = 0; index < indices.size(); index++)
    {
        m_indices[index] = indices[index];
    }


    VAO.CreateVAO(m_vertices, vertSize, m_indices, indices.size(), GL_STATIC_DRAW);
}

VertexArray& Mesh::GetVAO()
{
    return VAO;
}

void Mesh::Delete()
{
    VAO.Delete();
}