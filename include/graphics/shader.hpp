//
// Originally created by Oscar Jauffret on 11/04/2026.
// Extracted into the reusable `graphics` library; ocean-specific
// (`setWave`, oceanConfig include) removed, generic setters added.
//

#ifndef GRAPHICS_SHADER_HPP
#define GRAPHICS_SHADER_HPP

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int ID = 0;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    // Non-copyable (it owns a GL program handle), but movable.
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void use() const;
    void del() const;

    // Hot-reload: recompiles from the original paths. On failure the
    // currently-bound program is kept and false is returned, so a typo
    // in a shader won't black-screen you mid-session.
    bool reload();

    void setBool (const std::string& name, bool value)             const;
    void setInt  (const std::string& name, int value)              const;
    void setFloat(const std::string& name, float value)            const;
    void setVec2 (const std::string& name, const glm::vec2& vec)   const;
    void setVec3 (const std::string& name, const glm::vec3& vec)   const;
    void setVec4 (const std::string& name, const glm::vec4& vec)   const;
    void setMat3 (const std::string& name, const glm::mat3& mat)   const;
    void setMat4 (const std::string& name, const glm::mat4& mat)   const;

private:
    std::string m_vertexPath;
    std::string m_fragmentPath;

    // Compiles + links from paths and returns a new program ID.
    // Throws std::runtime_error on any failure.
    static unsigned int build(const char* vertexPath, const char* fragmentPath);
};

#endif //GRAPHICS_SHADER_HPP
