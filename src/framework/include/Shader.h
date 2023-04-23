//
// Created by 58413 on 2023/4/4.
//

#ifndef LEARNOPENGL_SHADER_IN_H
#define LEARNOPENGL_SHADER_IN_H

#include <map>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;

namespace LearnOpenGLFramework {
    class Shader {
    private:
        static std::map<string, string> shaderSources;
        unsigned int shaderID;

        static string loadShaderSource(const string& path);

    public:
        Shader(const string& vertexShaderName, const string& fragmentShaderName);
        Shader(const string& vertexShaderName, const string& geometryShaderName, const string& fragmentShaderName);

        void setUniformVec3(const string& name, float a, float b, float c) const;

        void setUniform1i(const string& name, unsigned int value) const;

        void setUniformMatrix4f(const string& name, const glm::mat4& value) const;

        void setUniform1f(const string& name, float value) const;

        void use() const;

        [[nodiscard]] unsigned int getID() const {
            return this->shaderID;
        }
    };
}



#endif //LEARNOPENGL_SHADER_IN_H
