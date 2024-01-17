#include "Cubemap.h"


Cubemap::Cubemap(const std::vector<std::string>& faces)
{
    GLCall(glGenTextures(1, &m_cubemapTextureID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTextureID));

    int width, height, nrChannels, desiredChannels = 0;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, desiredChannels);
        stbi_set_flip_vertically_on_load(true);

        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        if (data)
        {
            GLCall(glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                width,
                height,
                0,
                format,
                GL_UNSIGNED_BYTE,
                data
            ));

            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

            stbi_image_free(data);
        }
        else
        {
            HNCRSP_LOG_ERROR(fmt::format("Texture failed to load: {}.", stbi_failure_reason()));
            HNCRSP_LOG_INFO(fmt::format("Texture path: {}", faces[i]));
            stbi_image_free(data);
            TERMINATE("Cubemap textures failed to load.");
        }
    }

    SetMesh();
}

void Cubemap::SetMesh()
{
    verticesPos = {
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),

        // right
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f,  1.0f),

        // back
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),

        // left
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),

        // top
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),

        // bottom
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3( 1.0f, -1.0f,  1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
    };

    indicesData = {
        // front
        0, 2, 1,
        0, 3, 2,

        // right
        4, 6, 5,
        4, 7, 6,

        // back
        8, 10, 9,
        8, 11, 10,

        // left
        12, 14, 13,
        12, 15, 14,

        // top
        16, 18, 17,
        16, 19, 18,

        // back
        20, 22, 21,
        20, 23, 22
    };

    cubemapMesh.vertices = verticesPos;
    cubemapMesh.indices = indicesData;

    cubemapMesh.ConstructMesh();
    cubemapMesh.addTransform(Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)));
}

void Cubemap::Draw(std::shared_ptr<Shader> shader) const
{
    // GLCall(glDepthMask(GL_FALSE));  // TODO: ????
    GLCall(glDepthFunc(GL_LEQUAL));
    GLCall(glActiveTexture(GL_TEXTURE0 + 10));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTextureID));
    cubemapMesh.Draw(shader);
    GLCall(glDepthFunc(GL_LESS));
    // GLCall(glDepthMask(GL_TRUE));
}