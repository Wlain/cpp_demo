//
// Created by william on 2022/5/18.
//

#include "model.h"

#include <string_view>

Model::Model(const char* name)
{
    m_diffuseMap = nullptr;
    m_normalMap = nullptr;
    m_specularMap = nullptr;
    std::ifstream in;
    in.open(name, std::ios::in | std::ios::binary);
    if (in.fail())
    {
        std::cerr << "open file: " << name << "failed" << std::endl;
        return;
    }
    std::string line;
    while (in.good())
    {
        std::getline(in, line);
        std::istringstream iss(line);
        char trash;
        if (line.compare(0, 2, "v ") == 0)
        {
            iss >> trash;
            Vec3f v;
            iss >> v[0] >> v[1] >> v[2];
            m_vertices.emplace_back(v);
        }
        else if (line.compare(0, 3, "vn ") == 0)
        {
            iss >> trash >> trash;
            Vec3f vn;
            iss >> vn[0] >> vn[1] >> vn[2];
            m_normals.emplace_back(vn);
        }
        else if (line.compare(0, 3, "vt ") == 0)
        {
            iss >> trash >> trash;
            Vec2f uv;
            iss >> uv[0] >> uv[1];
            m_uv.emplace_back(uv);
        }
        else if (line.compare(0, 2, "f ") == 0)
        {
            std::vector<Vec3i> f;
            Vec3i temp{};
            iss >> trash;
            while (iss >> temp[0] >> trash >> temp[1] >> trash >> temp[2])
            {
                for (int i = 0; i < 3; i++)
                    temp[i]--;
                f.emplace_back(temp);
            }
            m_faces.emplace_back(f);
        }
    }
    std::cout << "# v# " << m_vertices.size() << " f# " << m_faces.size() << "\n";
    m_diffuseMap = loadTexture(name, "_diffuse.bmp");
    m_normalMap = loadTexture(name, "_nm.bmp");
    m_specularMap = loadTexture(name, "_spec.bmp");
}

Model::~Model()
{
    if (m_diffuseMap)
    {
        delete m_diffuseMap;
        m_diffuseMap = nullptr;
    }
    if (m_normalMap)
    {
        delete m_normalMap;
        m_diffuseMap = nullptr;
    }
    if (m_specularMap)
    {
        delete m_specularMap;
        m_specularMap = nullptr;
    }
}

Bitmap* Model::loadTexture(const char* name, const char* suffix)
{
    std::string textFile(name);
    auto dot = textFile.find_last_of('.');
    if (dot == std::string_view::npos) return nullptr;
    textFile = textFile.substr(0, dot) + suffix;
    Bitmap* texture = Bitmap::loadFile(textFile.c_str());
    std::cout << "loading: " << textFile << ((texture) ? " OK" : " failed") << "\n";
    texture->flipVertical();
    return texture;
}
