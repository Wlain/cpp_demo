//
// Created by william on 2022/5/18.
//

#ifndef CPP_DEMO_MODEL_H
#define CPP_DEMO_MODEL_H
#include "renderHelp.h"

#include <fstream>
#include <iostream>
#include <sstream>
class Model
{
public:
    explicit Model(const char* name);
    ~Model();
    inline int vertNum() const { return (int)m_vertices.size(); }
    inline int facesNum() const { return (int)m_faces.size(); }
    inline std::vector<int> face(int idx) const
    {
        std::vector<int> face;
        for (int i = 0; i < (int)m_faces[idx].size(); i++)
            face.push_back(m_faces[idx][i][0]);
        return face;
    }

    inline Vec3f vert(int i) const { return m_vertices[i]; }
    inline Vec3f vert(int iface, int nthvert) { return m_vertices[m_faces[iface][nthvert][0]]; }
    inline Vec2f uv(int iface, int nthvert) const
    {
        return m_uv[m_faces[iface][nthvert][1]];
    }
    inline Vec3f normal(int iface, int nthvert) const
    {
        int idx = m_faces[iface][nthvert][2];
        return vectorNormalize(m_normals[idx]);
    }

    inline Vec4f diffuse(Vec2f uv) const
    {
        assert(m_diffuseMap);
        return m_diffuseMap->sample2D(uv);
    }

    inline Vec3f normal(Vec2f uv) const
    {
        assert(m_normalMap);
        Vec4f color = m_normalMap->sample2D(uv);
        for (int i = 0; i < 3; i++)
            color[i] = color[i] * 2.0f - 1.0f;
        return color.xyz();
    }

    inline float specular(Vec2f uv)
    {
        Vec4f color = m_specularMap->sample2D(uv);
        return color.b;
    }

private:
    Bitmap* loadTexture(const char* name, const char* suffix);

private:
    std::vector<Vec3f> m_vertices;
    std::vector<std::vector<Vec3i>> m_faces;
    std::vector<Vec3f> m_normals;
    std::vector<Vec2f> m_uv;
    Bitmap* m_diffuseMap;
    Bitmap* m_normalMap;
    Bitmap* m_specularMap;
};

#endif //CPP_DEMO_MODEL_H
