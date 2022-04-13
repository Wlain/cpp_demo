//
// Created by cwb on 2022/4/12.
//

#ifndef CPP_DEMO_MESH_H
#define CPP_DEMO_MESH_H
#include "program.h"
#include "textureGL.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

namespace graphicEngine
{

class Mesh
{
public:
    struct Vertex
    {
        // position
        glm::vec3 position;
        // normal
        glm::vec3 normal;
        // texCoords
        glm::vec2 texCoords;
        // tangent
        glm::vec3 tangent;
        // bitangent
        glm::vec3 bitangent;
        //bone indexes which will influence this vertex
        int boneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float weights[MAX_BONE_INFLUENCE];
    };
    struct Texture
    {
        std::shared_ptr<TextureGL> tex;
        std::string type;
        std::string path;
    };

public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    ~Mesh();
    void destroy();
    void render(const std::unique_ptr<graphicEngine::Program>& program);
    void setupMesh();

private:
    // mesh data
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
};
} // namespace graphicEngine
#endif //CPP_DEMO_MESH_H
