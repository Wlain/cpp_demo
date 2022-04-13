//
// Created by cwb on 2022/4/12.
//

#ifndef CPP_DEMO_MODEL_H
#define CPP_DEMO_MODEL_H
#include "mesh.h"
#include "program.h"
#include "textureGL.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace graphicEngine
{
class Model
{
public:
    explicit Model(std::string const& path);
    ~Model();
    void render(const std::unique_ptr<graphicEngine::Program>& program);

private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
    std::vector<Mesh::Texture> m_textures;
    std::vector<Mesh> m_meshes;
    std::string m_directory;
};
} // namespace graphicEngine

#endif //CPP_DEMO_MODEL_H
