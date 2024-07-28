#include "Cubemap.h"
#include "src/managers/ShaderManager.h"
#include "src/serialize/ImageSerializer.h"


HNCRSP_NAMESPACE_START

Cubemap::Cubemap(const std::array<FileSystem::Path, 6>& faces)
{
    GLCall(glGenTextures(1, &m_cubemapTextureID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTextureID));

    int width, height, nrChannels, desiredChannels = 3;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        ImageSerializer imageSerializer(desiredChannels);
        Serialized::Image* deserialized_image = imageSerializer.GetDeserializedObject(faces[i]);
        uint8_t* data;

        if (!deserialized_image)
        {
            stbi_set_flip_vertically_on_load(false);
            data = stbi_load(faces[i].string().c_str(), &width, &height, &nrChannels, desiredChannels);
            stbi_set_flip_vertically_on_load(true);

            imageSerializer.AddImage(data, width, height, faces[i]);
        }
        else
        {
            data = deserialized_image->mutable_image_data()->data();
            width = deserialized_image->width();
            height = deserialized_image->height();
        }

        GLenum format = GL_RGB;

        if (data)
        {
            if (i == 0)
            {
                GLCall(glTexStorage2D(
                    GL_TEXTURE_CUBE_MAP,
                    1,
                    GL_SRGB8,  // have to be sized
                    width,
                    height
                ));
            }
            GLCall(glTexSubImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                0, 0,
                width, height,
                format,
                GL_UNSIGNED_BYTE,
                data
            ));

            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

            if (!deserialized_image) stbi_image_free(data);
        }
        else
        {
            HNCRSP_LOG_ERROR(fmt::format("Texture failed to load: {}.", stbi_failure_reason()));
            HNCRSP_LOG_INFO(fmt::format("Texture path: {}", faces[i].string()));
            stbi_image_free(data);
            HNCRSP_TERMINATE("Cubemap textures failed to load.");
        }
    }

    _SetMesh();
}

Cubemap::~Cubemap()
{
    HNCRSP_CHECK_RENDER_CONTEXT();

    GLCall(glDeleteTextures(1, &m_cubemapTextureID));
}

void Cubemap::_SetMesh()
{
    std::vector<glm::vec3> verticesPos = {
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

    std::vector<GLuint> indicesData = {
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

    m_VAO = std::make_unique<VertexArray>(
        &verticesPos,
        &indicesData,
        nullptr,
        nullptr,
        nullptr
    );
    m_verticesCount = indicesData.size();
}

static GLuint vao_id = -1;
void Cubemap::Draw() const
{
    // GLCall(glDepthMask(GL_FALSE));  // TODO: ????
    m_VAO->Bind();
    GLCall(glDepthFunc(GL_LEQUAL));

    g_ShaderManager.cubemapShader->Use();
    if (vao_id != m_VAO->GetID())
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + CUBEMAP_TEXTURE_UNIT_INDEX));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTextureID));  // TODO: don't rebind every frame
        g_ShaderManager.cubemapShader->SetIntUnf("u_cubemap", CUBEMAP_TEXTURE_UNIT_INDEX);

        vao_id = m_VAO->GetID();
    }

    GLCall(glDrawElements(GL_TRIANGLES, m_verticesCount, GL_UNSIGNED_INT, nullptr));
    GLCall(glDepthFunc(GL_LESS));
    // GLCall(glDepthMask(GL_TRUE));
}

HNCRSP_NAMESPACE_END