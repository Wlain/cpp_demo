//
// Created by cwb on 2022/4/12.
//

#ifndef CPP_DEMO_MODEL_H
#define CPP_DEMO_MODEL_H
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "gl/common/programGL.h"
#include "gl/common/textureGL.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "mesh.h"
namespace graphicEngine
{
class Model
{
public:
    explicit Model(std::string const& path);
    ~Model();
    void render(const std::unique_ptr<ProgramGL>& program, uint32_t mount = 0);
    const std::vector<Mesh>& mesh() const { return m_meshes; }

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
