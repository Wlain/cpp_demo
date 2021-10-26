//
// Created by william on 2021/10/24.
//

#ifndef CPP_DEMO_OBJLOADER_H
#define CPP_DEMO_OBJLOADER_H
#include "vector2.h"
#include "vector3.h"

#include <cmath>
#include <eigen3/Eigen/Eigen>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace graphics
{
struct Vertex
{
    Vector3 position;
    Vector3 normal;
    Vector2 texCoords;
};

struct Material
{
    /// 材质名
    std::string name;
    /// 环境光颜色
    Vector3 Ka;
    /// 漫反射光颜色
    Vector3 Kd;
    /// 镜面反射光颜色
    Vector3 Ks;
    /// 镜面反射光指数
    float Ns{};
    /// 光密度
    float Ni{};
    /// 溶解度
    float dissolve{};
    /// 光照强度
    int Illumination{};
    /// Ambient Texture Map
    std::string mapKa;
    /// Diffuse Texture Map
    std::string mapKd;
    /// Specular Texture Map
    std::string mapKs;
    /// Specular Hightlight Map
    std::string mapNs;
    /// Alpha Texture Map
    std::string mapAlpha;
    /// Bump Map
    std::string mapBump;
};

struct Mesh
{
    Mesh() = default;
    Mesh(std::vector<Vertex>& vertices_, std::vector<unsigned int>& indices_) :
        vertices(vertices_), indices(indices_)
    {
    }
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;
};

class ObjLoader
{
public:
    ObjLoader();
    ~ObjLoader();
    bool loadFile(std::string& path);
    bool loadMaterials(std::string& path);

private:
    /// 获取第一个符号
    static std::string firstToken(const std::string& in);
    /// 获取尾部符号
    static std::string tail(const std::string& in);
    /// 在给定的标记处将一个字符串分割成一个字符串数组
    static std::vector<std::string> split(const std::string& in, std::string token);

    template <class T>
    inline const T& getElement(const std::vector<T>& elements, std::string& index)
    {
        int idx = std::stoi(index);
        if (idx < 0)
            idx = int(elements.size()) + idx;
        else
            idx--;
        return elements[idx];
    }

    ///  generate vertices from a list of positions,tcoords, normals and a face line
    void genVerticesFromRawOBJ(std::vector<Vertex>& verts,
                               const std::vector<Vector3>& positions,
                               const std::vector<Vector2>& texCoords,
                               const std::vector<Vector3>& normals,
                               std::string& curline);

    void vertexTriangluation(std::vector<unsigned int>& indices,
                             const std::vector<Vertex>& verts);

public:
    std::vector<Mesh> m_meshes;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Material> m_materials;
};

} // namespace graphics
#endif //CPP_DEMO_OBJLOADER_H
