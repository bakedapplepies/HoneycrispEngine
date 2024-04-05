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



    m_sphere = GenerateCubeSphere(50, 2.0f);
}

SpaceScene::~SpaceScene() {}

void SpaceScene::OnUpdate(const float& dt)
{
}

void SpaceScene::OnImGui()
{

}

// this may not be needed as a quadtree may be implemented? idk
std::shared_ptr<Honeycrisp::Mesh> SpaceScene::GenerateCubeSphere(
    unsigned int resolution,
    float radius
) {
    assert(resolution >= 2 && "Cube sphere resolution can't be less than 2.");
    std::vector<glm::vec3> pos;
    std::vector<GLuint> indices;
    pos.reserve(pow(resolution, 3));
    pos.reserve(6 * pow(resolution - 1, 2) * 6);  // indices in a quad * quad per face * faces

    glm::vec3 offset_to_origin = glm::vec3(static_cast<float>(resolution - 1) / 2.0f);

    // setting vertex positions
    for (float x = 0.0f; x < static_cast<float>(resolution); x += 1.0f)
    {
        for (float y = 0.0f; y < static_cast<float>(resolution); y += 1.0f)
        {
            for (float z = 0.0f; z < static_cast<float>(resolution); z += 1.0f)
            {
                glm::vec3 newPos = glm::vec3(x, y, z) - offset_to_origin;
                pos.push_back((newPos) * radius);
            }
        }
    }

    // setting indices
    for (unsigned int x = 0; x < resolution - 1; x++)
    {
        for (unsigned int y = 0; y < resolution - 1; y++)
        {
            for (unsigned int z = 0; z < resolution - 1; z++)
            {
                indices.push_back( z + resolution * y + pow(resolution, 2) * x);
                indices.push_back((z + resolution * y + pow(resolution, 2) * x) + 1);
                indices.push_back((z + resolution * y + pow(resolution, 2) * x) + resolution);
                indices.push_back((z + resolution * y + pow(resolution, 2) * x) + 1);
                indices.push_back((z + resolution * y + pow(resolution, 2) * x) + resolution + 1);
                indices.push_back((z + resolution * y + pow(resolution, 2) * x) + resolution);
            }
        }
    }

    return CreateStaticRenderObj<Honeycrisp::Mesh>(
        &pos,
        &indices,
        nullptr,
        nullptr,
        nullptr
    );
}