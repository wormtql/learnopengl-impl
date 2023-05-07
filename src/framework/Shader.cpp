//
// Created by 58413 on 2023/4/4.
//

#include <fstream>
#include <vector>

#include <glad/gl.h>

#include "Shader.h"

namespace LearnOpenGLFramework {
//    std::map<string, const char*> Shader::shaderSources = {
//            { "color_fragmentShader", R"(@COLOR_FRAGMENT_SHADER@)" },
//            { "color_vertexShader", R"(@COLOR_VERTEX_SHADER@)" },
//            { "color_light_fragmentShader", R"(@COLOR_LIGHT_FRAGMENT_SHADER@)" },
//            { "material_fragmentShader", R"(@MATERIAL_FRAGMENT_SHADER@)" }
//    };

    std::map<string, string> Shader::shaderSources = {};

    string Shader::loadShaderSource(const string &path) {
        if (shaderSources.find(path) != shaderSources.end()) {
            return shaderSources[path];
        }
        std::fstream is(path);
        if (!is.is_open()) {
            printf("cannot open file\n");
            return "";
        }
        string result((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

        shaderSources[path] = result;

        return std::move(result);
    }

    Shader::Shader(const string &vertexShaderName, const string &geometryShaderName, const string &fragmentShaderName) {
        string vertexSource = loadShaderSource(R"(E:\CLionProjects\learnopengl\src\shaders\)" + vertexShaderName + ".glsl");
        string geometryShaderSource = loadShaderSource(R"(E:\CLionProjects\learnopengl\src\shaders\)" + geometryShaderName + ".glsl");
        string fragmentSource = loadShaderSource(R"(E:\CLionProjects\learnopengl\src\shaders\)" + fragmentShaderName + ".glsl");

        auto vertexSourceC = vertexSource.c_str();
        auto geometryShaderC = geometryShaderSource.c_str();
        auto fragmentSourceC = fragmentSource.c_str();

        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSourceC, nullptr);
        glCompileShader(vertexShader);

        int isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            int maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

            printf("compile vertex shader error: %s\n", &errorLog[0]);

            glDeleteShader(vertexShader);
        }

        unsigned int geometryShader;
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderC, nullptr);
        glCompileShader(geometryShader);

        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            int maxLength = 0;
            glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(geometryShader, maxLength, &maxLength, &errorLog[0]);

            printf("compile geometry shader error: %s\n", &errorLog[0]);
        }

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSourceC, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            int maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

            printf("compile fragment shader error: %s\n", &errorLog[0]);
        }

        this->shaderID = glCreateProgram();
        glAttachShader(this->shaderID, vertexShader);
        glAttachShader(this->shaderID, geometryShader);
        glAttachShader(this->shaderID, fragmentShader);
        glLinkProgram(this->shaderID);

        glDeleteShader(vertexShader);
        glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);
    }

    Shader::Shader(const string &vertexShaderName, const string &fragmentShaderName) {
        string vertexSource = loadShaderSource(R"(E:\CLionProjects\learnopengl\src\shaders\)" + vertexShaderName + ".glsl");
        string fragmentSource = loadShaderSource(R"(E:\CLionProjects\learnopengl\src\shaders\)" + fragmentShaderName + ".glsl");

        auto vertexSourceC = vertexSource.c_str();
        auto fragmentSourceC = fragmentSource.c_str();

        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSourceC, nullptr);
        glCompileShader(vertexShader);

        int isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            int maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

            printf("compile vertex shader error: %s\n", &errorLog[0]);

            glDeleteShader(vertexShader);
        }

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSourceC, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            int maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

            printf("compile fragment shader error: %s\n", &errorLog[0]);
        }

        this->shaderID = glCreateProgram();
        glAttachShader(this->shaderID, vertexShader);
        glAttachShader(this->shaderID, fragmentShader);
        glLinkProgram(this->shaderID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void Shader::use() const {
        glUseProgram(this->shaderID);
    }

    void Shader::setUniformVec3(const string &name, float a, float b, float c) const {
        auto location = glGetUniformLocation(this->shaderID, name.c_str());
        glUniform3f(location, a, b, c);
    }

    void Shader::setUniformVec3_2(const string &name, glm::vec3 value) const {
        auto location = glGetUniformLocation(this->shaderID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void Shader::setUniform1i(const string &name, unsigned int value) const {
        auto location = glGetUniformLocation(this->shaderID, name.c_str());
        glUniform1i(location, value);
    }

    void Shader::setUniform1f(const string &name, float value) const {
        auto location = glGetUniformLocation(this->shaderID, name.c_str());
        glUniform1f(location, value);
    }

    void Shader::setUniform1b(const string &name, bool value) const {
        auto location = glGetUniformLocation(this->shaderID, name.c_str());
        glUniform1i(location, value);
    }

    void Shader::setUniformMatrix4f(const string &name, const glm::mat4 &value) const {
        auto location = glGetUniformLocation(this->shaderID, name.c_str());
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
    }
}