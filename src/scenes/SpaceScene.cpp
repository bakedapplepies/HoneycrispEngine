#include "SpaceScene.h"


using namespace Honeycrisp;

SpaceScene::SpaceScene()
{
    CreateCubemap(
        FileSystem::Path("resources/textures/cubemaps/space/right.png"),
        FileSystem::Path("resources/textures/cubemaps/space/left.png"),
        FileSystem::Path("resources/textures/cubemaps/space/top.png"),
        FileSystem::Path("resources/textures/cubemaps/space/bottom.png"),
        FileSystem::Path("resources/textures/cubemaps/space/front.png"),
        FileSystem::Path("resources/textures/cubemaps/space/back.png")
    );

    m_sphere = GenerateCubeSphere(m_sphere_resolution, m_sphere_radius);
}

void SpaceScene::OnUpdate(const float& dt)
{
    unsigned int sphere_resolution = static_cast<unsigned int>(m_sphere_resolution);
    if (m_change_sphere)
    {
        m_sphere.reset();
        m_sphere = GenerateCubeSphere(sphere_resolution, m_sphere_radius);
    }
}

void SpaceScene::OnImGui()
{
    m_change_sphere = 
        ImGui::SliderFloat("Sphere Radius", &m_sphere_radius, 0.1f, 10.0f) ||
        ImGui::SliderInt("Sphere Resolution", &m_sphere_resolution, 2, 50);
}

// this may not be needed as a quadtree may be implemented? idk
std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Mesh> > SpaceScene::GenerateCubeSphere(
    unsigned int resolution,
    float radius
) {
    assert(resolution >= 2 && "Cube sphere resolution can't be less than 2.");
    std::vector<glm::vec3> pos;
    std::vector<glm::vec3> color;
    std::vector<GLuint> indices;
    pos.reserve((resolution - 1) * 4 * resolution + pow(resolution - 2, 2));
    color.reserve((resolution - 1) * 4 * resolution);
    indices.reserve(pow(resolution - 1, 2) * 6 * 6);  // sub-faces per face * indices per quad * 6 sides

    glm::vec3 offset_to_origin = glm::vec3(static_cast<float>(resolution - 1) / 2.0f);
    // HNCRSP_LOG_INFO(offset_to_origin);
    // glm::vec3 offset_to_origin = glm::vec3(0.0f);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    #define RAND dist(mt)

    // setting vertex positions - 4 sides
    for (float y = 0.0f; y < static_cast<float>(resolution); y += 1.0f)
    {
        // first face
        for (float x = 0.0f; x < static_cast<float>(resolution - 1); x += 1.0f)
        {
            glm::vec3 newPos = glm::vec3(x, y, 0.0f) - offset_to_origin;
            pos.push_back(glm::normalize(newPos) * radius);
            color.push_back(glm::vec3(y/static_cast<float>(resolution - y)));
        }
        // second face
        for (float z = 0.0f; z < static_cast<float>(resolution - 1); z += 1.0f)
        {
            glm::vec3 newPos = glm::vec3(static_cast<float>(resolution - 1), y, z) - offset_to_origin;
            pos.push_back(glm::normalize(newPos) * radius);
            color.push_back(glm::vec3(y/static_cast<float>(resolution - y)));
        }
        // third face
        for (float x = static_cast<float>(resolution - 1); x > 0.0f; x -= 1.0f)
        {
            glm::vec3 newPos = glm::vec3(x, y, static_cast<float>(resolution - 1)) - offset_to_origin;
            pos.push_back(glm::normalize(newPos) * radius);
            color.push_back(glm::vec3(y/static_cast<float>(resolution - y)));
        }
        // fourth face
        for (float z = static_cast<float>(resolution - 1); z > 0.0f; z -= 1.0f)
        {
            glm::vec3 newPos = glm::vec3(0.0f, y, z) - offset_to_origin;
            pos.push_back(glm::normalize(newPos) * radius);
            color.push_back(glm::vec3(y/static_cast<float>(resolution - y)));
        }
    }

    // setting vertex positions - top & bottom
    // top
    for (float z = 1.0f; z < static_cast<float>(resolution - 1); z += 1.0f)
    {
        for (float x = 1.0f; x < static_cast<float>(resolution - 1); x += 1.0f)
        {
            glm::vec3 newPos = glm::vec3(x, static_cast<float>(resolution - 1), z) - offset_to_origin;
            pos.push_back(glm::normalize(newPos) * radius);
            color.push_back(glm::vec3(RAND, RAND, RAND));
        }
    }
    // bottom
    for (float z = 1.0f; z < static_cast<float>(resolution - 1); z += 1.0f)
    {
        for (float x = 1.0f; x < static_cast<float>(resolution - 1); x += 1.0f)
        {
            glm::vec3 newPos = glm::vec3(x, 0.0f, z) - offset_to_origin;
            pos.push_back(glm::normalize(newPos) * radius);
            color.push_back(glm::vec3(RAND, RAND, RAND));
        }
    }

    // setting indices - 4 sides
    unsigned int strip_length = (resolution - 1) * 4;
    for (unsigned int y = 0; y < resolution - 1; y++)
    {
        for (unsigned int x = 0; x < strip_length; x++)
        {
            if (x == strip_length - 1)
            {
                indices.push_back(strip_length * y + x);
                indices.push_back(strip_length * (y + 1) + x);
                indices.push_back(y * strip_length);
                indices.push_back(y * strip_length);
                indices.push_back(strip_length * (y + 1) + x);
                indices.push_back((y + 1) * strip_length);
                continue;
            }
            indices.push_back(strip_length * y + x);
            indices.push_back(strip_length * (y + 1) + x);
            indices.push_back(strip_length * (y + 1) + x + 1);
            indices.push_back(strip_length * y + x);
            indices.push_back(strip_length * (y + 1) + x + 1);
            indices.push_back(strip_length * y + x + 1);
        }
    }

    // setting indices - top
    if (resolution > 2)
    {
        unsigned int sidesOffsetTop = strip_length * resolution;
        for (unsigned int y = 0; y < resolution - 3; y++)
        {
            for (unsigned int x = 0; x < resolution - 3; x++)
            {
                indices.push_back(sidesOffsetTop + (resolution - 2) * y + x);
                indices.push_back(sidesOffsetTop + (resolution - 2) * y + x + 1);
                indices.push_back(sidesOffsetTop + (resolution - 2) * (y + 1) + x + 1);
                indices.push_back(sidesOffsetTop + (resolution - 2) * y + x);
                indices.push_back(sidesOffsetTop + (resolution - 2) * (y + 1) + x + 1);
                indices.push_back(sidesOffsetTop + (resolution - 2) * (y + 1) + x);
            }
        }

        // setting indices - bottom
        unsigned int sidesOffsetBottom = sidesOffsetTop + pow(resolution - 2, 2);
        for (unsigned int y = 0; y < resolution - 3; y++)
        {
            for (unsigned int x = 0; x < resolution - 3; x++)
            {
                indices.push_back(sidesOffsetBottom + (resolution - 2) * y + x);
                indices.push_back(sidesOffsetBottom + (resolution - 2) * (y + 1) + x + 1);
                indices.push_back(sidesOffsetBottom + (resolution - 2) * y + x + 1);
                indices.push_back(sidesOffsetBottom + (resolution - 2) * y + x);
                indices.push_back(sidesOffsetBottom + (resolution - 2) * (y + 1) + x);
                indices.push_back(sidesOffsetBottom + (resolution - 2) * (y + 1) + x + 1);
            }
        }

        for (unsigned int x = 1; x < resolution - 2; x++)
        {
            indices.push_back(x);
            indices.push_back(x + 1);
            indices.push_back(sidesOffsetTop + x);
            indices.push_back(x);
            indices.push_back(sidesOffsetTop + x);
            indices.push_back(sidesOffsetTop + x - 1);
        }
        for (unsigned int x = 1; x < resolution - 2; x++)
        {
            indices.push_back(x + (resolution - 1) * 1);
            indices.push_back(x + 1);
            indices.push_back(sidesOffsetTop + (resolution - 2) * (resolution - 2 - x));
            indices.push_back(x);
            indices.push_back(sidesOffsetTop + x);
            indices.push_back(sidesOffsetTop + x - 1);
        }
        
    }

    return CreateStaticRenderObj<Honeycrisp::Mesh>(
        &pos,
        &indices,
        nullptr,
        &color,
        nullptr
    );
}