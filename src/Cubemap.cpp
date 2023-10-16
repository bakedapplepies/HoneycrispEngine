#include "Cubemap.h"


Cubemap::Cubemap(const std::vector<std::string>& faces, const std::source_location& location)
{
    GLCall(glGenTextures(1, &m_cubemapTextureID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTextureID));

    int width, height, nrChannels, desiredChannels = 0;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::string root("../");  // Executable is in build folder
        std::filesystem::path textureRelativePath(root);
        textureRelativePath /= std::filesystem::path(location.file_name()).remove_filename();  // where the file is
        textureRelativePath /= faces[i].c_str();  // add relative path relative to the above path <---------
                                                  // in case this is absolute, it will replace everything  |
        textureRelativePath = std::filesystem::absolute(textureRelativePath);  // make absolute -----------
        textureRelativePath.make_preferred();
        std::string path = textureRelativePath.string();

        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, desiredChannels);

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
            stbi_image_free(data);
        }
        else
        {
            Debug::Error(fmt::format("Texture failed to load: {}", stbi_failure_reason()));
            stbi_image_free(data);
            assert(false && "Cubemap textures failed to load.");
        }

        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    }

    SetVAO();
}

void Cubemap::SetVAO()
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

    cubemapMesh.vertices = verticesPos;
    cubemapMesh.indices = indicesData;

    cubemapMesh.ConstructMesh();
    cubemapMesh.AddPosition(glm::vec3(0.0f));
}

void Cubemap::Draw(std::shared_ptr<Shader> shader) const
{
    glDepthMask(GL_FALSE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTextureID);
    cubemapMesh.Draw(shader);
    glDepthMask(GL_TRUE);
}