#ifndef SHADER_H
#define SHADER_H

#include "glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

class Shader
{
public:
    unsigned int ID;

    Shader(){}
    Shader(const char* vertexPath, const char* fragmentPath);

    int getUniformLocation(const std::string &key);

    void use() const;
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setUInt(const std::string& name, unsigned int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec2(const std::string& name, float x, float y);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec3(const std::string& name, float x, float y, float z);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setVec4(const std::string& name, float x, float y, float z, float w);
    void setMat2(const std::string& name, const glm::mat2& mat);
    void setMat3(const std::string& name, const glm::mat3& mat);
    void setMat4(const std::string& name, const glm::mat4& mat);
    void SetVec4v(const std::string& name, glm::vec4 *array, int size);

private:
    std::unordered_map<std::string, int> uniformMap;

    void checkCompileErrors(GLuint shader, std::string type);
};

#endif