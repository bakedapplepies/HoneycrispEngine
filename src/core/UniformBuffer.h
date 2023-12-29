#include "../pch/pch.h"
#include "../utils/Debug.h"


template <typename... Ts>
class UniformBuffer
{
private:
    static std::unordered_map<GLuint, bool> takenBindingIndices;
    GLuint m_uboID;
    GLuint m_bindingIndex;  // not used after initialization for now
    std::array<size_t, sizeof...(Ts)> m_sizes;
    size_t totalByteSize = 0;

public:
    UniformBuffer(GLuint bindingIndex) : m_bindingIndex(bindingIndex)
    {
        AddTypeSizes();

        GLCall(glGenBuffers(1, &m_uboID));

        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_uboID));
        GLCall(glBufferData(GL_UNIFORM_BUFFER, totalByteSize, nullptr, GL_STATIC_DRAW));
        GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, m_uboID, 0, totalByteSize));
    }

public:
    template <typename... Args>
    void Update(const Args* const... args) const
    {
        static_assert(sizeof...(args) == sizeof...(Ts), "Invalid number of arguments for UBO");
        glBindBuffer(GL_UNIFORM_BUFFER, m_uboID);
        size_t offset = 0;
        size_t index = 0;
        Update(index, offset, args...);
    }

    template <typename T, typename... Args>
    void Update(size_t index, size_t offset, const T* const t, const Args* const... args) const
    {
        glBufferSubData(GL_UNIFORM_BUFFER, offset, m_sizes[index], t);
        offset += m_sizes[index];
        index++;
        Update(index, offset, args...);
    }

    template <typename T>
    void Update(size_t index, size_t offset, const T* const t) const
    {
        glBufferSubData(GL_UNIFORM_BUFFER, offset, m_sizes[index], t);
    }

private:
    template <int i = 0>
    void AddTypeSizes()
    {
        if constexpr(i < sizeof...(Ts))
        {
            using T = typename std::tuple_element<i, std::tuple<Ts...>>::type;
            m_sizes[i] = sizeof(T);
            totalByteSize += sizeof(T);

            AddTypeSizes<i + 1>();
        }
    }
};