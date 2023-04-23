//
// Created by 58413 on 2023/4/5.
//

#include "TextureHelper.h"
#include "stb_image.h"

#include "glad/gl.h"

namespace LearnOpenGLFramework {
    TextureHelper::TextureHelper() {
    }

    TextureHelper::~TextureHelper() {
        for (const auto& i: cache) {
            stbi_image_free(i.second);
        }
    }

    unsigned char* TextureHelper::loadImage(const string &name, int* width, int* height, int* channels) {
        if (cache.find(name) != cache.end()) {
            return cache[name];
        }

        unsigned char* data = stbi_load(name.c_str(), width, height, channels, 0);
        if (data == nullptr) {
            const char* reason = stbi_failure_reason();
            printf("failed to load image: %s\n", reason);
        }
        cache[name] = data;
        return data;
    }

    unsigned int TextureHelper::createTexture(const string &name, unsigned int slot, bool rgba, bool flip) {
        stbi_set_flip_vertically_on_load(flip);

        int width, height, channel;
        auto data = this->loadImage(name, &width, &height, &channel);

        auto x = GL_RGB;
        if (rgba) {
            x = GL_RGBA;
        }

        unsigned int texture1;
        glGenTextures(1, &texture1);
        glActiveTexture(slot);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexImage2D(GL_TEXTURE_2D, 0, x, width, height, 0, x, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        return texture1;
    }
}