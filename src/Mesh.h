#include "pch/pch.h"
#include "VertexArray.h"


class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

    std::vector<glm::vec3>& vertices;
    std::vector<glm::vec3>& normals;
    std::vector<glm::vec2>& uv;
    std::vector<unsigned int>& indices;

    VertexArray VAO;

private:
    float *m_vertices;
    unsigned int *m_indices;

public:
    void ConstructMesh();
    VertexArray& GetVAO();
    void Delete();
};