#include "api/pch/hncrsp_pch.h"

HNCRSP_NAMESPACE_START

template <typename... Ts>
class UniformBuffer
{
private:
    static std::unordered_map<GLuint, bool> takenBindingIndices;
    GLuint m_uboID;
    GLuint m_bindingIndex;  // not used after initialization for now
    std::array<size_t, sizeof...(Ts)> m_sizes;
    std::array<size_t, sizeof...(Ts)> m_offsets;
    size_t totalByteSize = 0;

public:
    UniformBuffer(GLuint bindingIndex) :
        m_bindingIndex(bindingIndex)
    {
        if (takenBindingIndices[bindingIndex])
        {
            // Technically an error, but doesn't crash OpenGL
            HNCRSP_ERROR("Binding index already taken.");
            return;
        }
        takenBindingIndices[bindingIndex] = true;

        _AddTypeSizes();

        glGenBuffers(1, &m_uboID);
        glBindBuffer(GL_UNIFORM_BUFFER, m_uboID);
        glBufferData(GL_UNIFORM_BUFFER, totalByteSize, nullptr, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, m_uboID, 0, totalByteSize);
    }

    template <typename... Args>
    void Update(const Args* const... args) const
    {
        HNCRSP_STATIC_ASSERT(sizeof...(args) <= sizeof...(Ts), "Invalid number of arguments for UBO.");
        glBindBuffer(GL_UNIFORM_BUFFER, m_uboID);
        size_t offset = 0;
        size_t index = 0;
        _Update(index, offset, args...);
    }

    ~UniformBuffer()
    {
        HNCRSP_CHECK_RENDER_CONTEXT();

        glDeleteBuffers(1, &m_uboID);
    }

private:
    template <typename T, typename... Args>
    inline void _Update(size_t index, size_t offset, const T* const t, const Args* const... args) const
    {
        glBufferSubData(GL_UNIFORM_BUFFER, m_offsets[index], m_sizes[index], t);
        offset += m_sizes[index];
        index++;
        _Update(index, offset, args...);
    }

    template <typename T>
    inline void _Update(size_t index, size_t offset, const T* const t) const
    {
        glBufferSubData(GL_UNIFORM_BUFFER, m_offsets[index], m_sizes[index], t);
    }

public:
    template <typename T>
    inline void Update(size_t index, const T* const t) const
    {
        glBufferSubData(GL_UNIFORM_BUFFER, m_offsets[index], m_sizes[index], t);
    }

    inline void Bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_uboID);
    }

private:
    template <int i = 0>
    constexpr void _AddTypeSizes()
    {
        if constexpr(i < sizeof...(Ts))
        {
            using T = typename std::tuple_element<i, std::tuple<Ts...>>::type;

            m_offsets[i] = totalByteSize;
            constexpr size_t remainder = sizeof(T) % 16;
            if constexpr(remainder != 0)
                totalByteSize += sizeof(T) + 16 - remainder;
            else
                totalByteSize += sizeof(T);

            m_sizes[i] = sizeof(T);

            _AddTypeSizes<i + 1>();
        }
    }
};

template <typename... Ts>
std::unordered_map<GLuint, bool> UniformBuffer<Ts...>::takenBindingIndices;

HNCRSP_NAMESPACE_END