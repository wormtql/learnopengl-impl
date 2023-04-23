//
// Created by 58413 on 2023/4/5.
//

#ifndef LEARNOPENGL_TEXTUREHELPER_H
#define LEARNOPENGL_TEXTUREHELPER_H

#include <string>
#include <map>

using std::string;

namespace LearnOpenGLFramework {
    class TextureHelper {
    private:
        std::map<string, unsigned char*> cache;
    public:
        TextureHelper();
        ~TextureHelper();

        unsigned char* loadImage(const string& name, int* width, int* height, int* channels);
        unsigned int createTexture(const string& name, unsigned int slot, bool rgba, bool flip);
    };
}



#endif //LEARNOPENGL_TEXTUREHELPER_H
