#include "src/graphics/AxesCrosshair.h"
#include "src/managers/ShaderManager.h"


HNCRSP_NAMESPACE_START

AxesCrosshair::AxesCrosshair()
{
    // Setup VAO
    uint8_t vertex_attrib_bits = 
        VERTEX_ATTRIB_POSITION_BIT | VERTEX_ATTRIB_COLOR_BIT;

    std::vector<float> vertex_data = {
    //  positions            colors
        0.0f,  0.0f,  0.0f,  0.0f,  0.0f, 0.0f
    };    

    std::vector<GLuint> indices = { 0 };
    m_VAO = VertexArray(vertex_attrib_bits, vertex_data, indices);

    // Get shader
    m_shader = g_ShaderManager.GetShader(
        FileSystem::Path("resources/shaders/gizmos/AxesCrosshairVertex.glsl"),
        FileSystem::Path("resources/shaders/gizmos/AxesCrosshairFragment.glsl"),
        FileSystem::Path("resources/shaders/gizmos/AxesCrosshairGeometry.glsl")
    );
}

void AxesCrosshair::Render() const
{
    glDisable(GL_DEPTH_TEST);
    m_shader->Use();
    glDrawElements(GL_POINT, 1, GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
}

HNCRSP_NAMESPACE_END