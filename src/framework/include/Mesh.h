//
// Created by 58413 on 2023/4/7.
//

#ifndef LEARNOPENGL_MESH_H
#define LEARNOPENGL_MESH_H

#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"

using std::string;
using std::vector;

namespace LearnOpenGLFramework {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    struct Texture {
        unsigned int id;
        string type;
        aiString path;
    };

    class Mesh {
    public:
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        Mesh(vector<Vertex>&& vertices, vector<unsigned int>&& indices, vector<Texture>&& textures);
        Mesh();
        void draw(const Shader& shader) const;
    private:
        unsigned int VAO, VBO, EBO;
        void setupMesh();
    };

    class Model {
    public:
        Model(const string& path);

        void draw(const Shader& shader) const;
    private:
        static std::map<string, Texture> textures_loaded;

        vector<Mesh> meshes;
        string directory;

        void loadModel(const string& path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
    };
}

#endif //LEARNOPENGL_MESH_H
