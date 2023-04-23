//
// Created by 58413 on 2023/4/13.
//

#ifndef LEARNOPENGL_GEOMETRY_H
#define LEARNOPENGL_GEOMETRY_H

#include "Shader.h"

namespace LearnOpenGLFramework {
    class Quad {
    private:
        static float vertices[];
        static unsigned int indices[];

        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;
    public:
        Quad();

        void draw(const Shader& shader) const;
    };

    class Cube {
    private:
        static float vertices[];

        unsigned int VBO;
        unsigned int VAO;
    public:
        Cube();

        void draw(const Shader& shader) const;
    };
}



#endif //LEARNOPENGL_GEOMETRY_H
