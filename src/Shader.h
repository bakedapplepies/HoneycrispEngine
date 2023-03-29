#include <string>

class Shader {
private:
    GLuint shaderID;

public:
    std::string ParseShader(const std::string& path);
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    
    void useShader() const;
    void deleteShader() const;

    GLuint getID() const;

    void setFloatUniform(const std::string& name, float value) const;
};