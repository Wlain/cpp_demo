//
// Created by william on 2021/10/24.
//

#include "objLoader.h"

#include "commonMacro.h"

namespace graphics
{
ObjLoader::ObjLoader() = default;

ObjLoader::~ObjLoader() = default;

bool ObjLoader::loadFile(std::string& path)
{
    ASSERT(!path.empty());
    if (path.substr(path.size() - 4, 4) != ".obj")
        return false;

    std::ifstream file(path);
    if (!file.is_open())
        return false;

    m_meshes.clear();

    std::vector<Vector3> positions;
    std::vector<Vector2> texCoords;
    std::vector<Vector3> normals;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::string> materialNames;
    std::string meshName;
    bool listening = false;
    Mesh tempMesh;
    const unsigned int outputEveryNth = 1000;
    unsigned int outputIndicator = outputEveryNth;
    std::string curline;
    while (std::getline(file, curline))
    {
        if ((outputIndicator == ((outputIndicator + 1) % outputEveryNth)) == 1)
        {
            if (!meshName.empty())
            {
                std::cout
                    << "\r- " << meshName
                    << "\t| vertices > " << positions.size()
                    << "\t| texcoords > " << texCoords.size()
                    << "\t| normals > " << normals.size()
                    << "\t| triangles > " << (vertices.size() / 3)
                    << (!materialNames.empty() ? "\t| material: " + materialNames.back() : "");
            }
        }
        auto firstTokenStr = firstToken(curline);
        if (firstTokenStr == "o" || firstTokenStr == "g")
        {
            if (!listening)
            {
                listening = true;
                meshName = tail(curline);
            }
            else
            {
                if (!indices.empty() && !vertices.empty())
                {
                    tempMesh = Mesh(vertices, indices, meshName);
                    m_meshes.emplace_back(tempMesh);
                    meshName.clear();
                }
                else
                {
                    meshName = tail(curline);
                }
            }
            std::cout << std::endl;
            outputIndicator = 0;
        }
        if (firstTokenStr == "v")
        {
            auto sPos = split(tail(curline), " ");
            positions.emplace_back(std::stof(sPos[0]), std::stof(sPos[1]), std::stof(sPos[2]));
        }
        if (firstTokenStr == "vt")
        {
            auto sTex = split(tail(curline), " ");
            texCoords.emplace_back(std::stof(sTex[0]), std::stof(sTex[1]));
        }
        if (firstTokenStr == "vn")
        {
            auto sNor = split(tail(curline), " ");
            normals.emplace_back(std::stof(sNor[0]), std::stof(sNor[1]), std::stof(sNor[2]));
        }
        // generate a Face (vertices & indices)
        if (firstTokenStr == "f")
        {
            // generate the vertices
            std::vector<Vertex> vVerts;
            genVerticesFromRawOBJ(vVerts, positions, texCoords, normals, curline);

            // add vertices
            for (const auto& vVert : vVerts)
            {
                vertices.emplace_back(vVert);
            }

            std::vector<unsigned int> vIndices;
            vertexTriangluation(vIndices, vVerts);

            // add indices
            for (unsigned int vIndice : vIndices)
            {
                unsigned int indnum = (unsigned int)((vertices.size()) - vVerts.size()) + vIndice;
                indices.push_back(indnum);
            }
        }

        // get mesh material name
        if (firstTokenStr == "usemtl")
        {
            materialNames.emplace_back(tail(curline));

            // create new mesh, if material changes within a group
            if (!indices.empty() && !vertices.empty())
            {
                // create Mesh
                auto tempName = meshName;
                int i = 2;
                while (true)
                {
                    tempName = meshName + "_" + std::to_string(i);
                    for (auto& m : m_meshes)
                        if (m.name == tempName)
                            continue;
                    break;
                }
                // Insert Mesh
                m_meshes.emplace_back(vertices, indices, tempName);

                // Cleanup
                vertices.clear();
                indices.clear();
            }
            outputIndicator = 0;
        }

        // load Materials
        if (firstTokenStr == "mtllib")
        {
            auto temp = split(path, "/");

            std::string pathTomat;

            if (temp.size() != 1)
            {
                for (int i = 0; i < temp.size() - 1; i++)
                {
                    pathTomat += temp[i] + "/";
                }
            }

            pathTomat += tail(curline);
            std::cout << std::endl
                      << "- find materials in: " << pathTomat << std::endl;
            // load Materials
            loadMaterials(pathTomat);
        }
    }
    std::cout << std::endl;

    // deal with last mesh
    if (!indices.empty() && !vertices.empty())
    {
        m_meshes.emplace_back(vertices, indices, meshName);
    }

    file.close();

    // set Materials for each Mesh
    for (int i = 0; i < (int)materialNames.size(); ++i)
    {
        std::string matName = materialNames[i];

        // Find corresponding material name in loaded materials
        // when found copy material variables into mesh material
        for (auto& m_material : m_materials)
        {
            if (m_material.name == matName)
            {
                m_meshes[i].material = m_material;
                break;
            }
        }
    }
    return !m_meshes.empty();
}

std::string ObjLoader::firstToken(const std::string& in)
{
    if (!in.empty())
    {
        size_t tokenStart = in.find_first_not_of(" \t");
        size_t tokenEnd = in.find_first_of(" \t", tokenStart);
        if (tokenStart != std::string::npos && tokenEnd != std::string::npos)
        {
            return in.substr(tokenStart, tokenEnd - tokenStart);
        }
        else if (tokenStart != std::string::npos)
        {
            return in.substr(tokenStart);
        }
    }
    return "";
}

std::string ObjLoader::tail(const std::string& in)
{
    if (!in.empty())
    {
        size_t tokenStart = in.find_first_not_of(" \t");
        size_t spaceStart = in.find_first_of(" \t", tokenStart);
        size_t tailStart = in.find_first_not_of(" \t", spaceStart);
        size_t tailEnd = in.find_last_not_of(" \t");
        if (tailStart != std::string::npos && tailEnd != std::string::npos)
        {
            return in.substr(tailStart, tailEnd - tailStart + 1);
        }
        else if (tailStart != std::string::npos)
        {
            return in.substr(tailStart);
        }
    }
    return "";
}

std::vector<std::string> ObjLoader::split(const std::string& in, const std::string& token)
{
    std::string temp;
    std::vector<std::string> result;
    for (int i = 0; i < (int)in.size(); ++i)
    {
        std::string test = in.substr(i, token.size());
        if (test == token)
        {
            if (!temp.empty())
            {
                result.emplace_back(temp);
                temp.clear();
                i += (int)token.size() - 1;
            }
            else
            {
                result.emplace_back("");
            }
        }
        else if (i + token.size() >= in.size())
        {
            temp += in.substr(i, token.size());
            result.emplace_back(temp);
            break;
        }
        else
        {
            temp += in[i];
        }
    }
    return result;
}

void ObjLoader::genVerticesFromRawOBJ(std::vector<Vertex>& verts,
                                      const std::vector<Vector3>& positions,
                                      const std::vector<Vector2>& texCoords,
                                      const std::vector<Vector3>& normals,
                                      std::string& curline)
{
    auto sFace = split(tail(curline), " ");
    Vertex vVert;
    bool noNormal = false;
    for (auto& i : sFace)
    {
        int type = 0;
        auto sVert = split(i, "/");
        // v1
        if (sVert.size() == 1)
        {
            // only position
            type = 1;
        }
        // v1/vt1
        if (sVert.size() == 2)
        {
            // position texture
            type = 2;
        }
        // v1/vt1/vn1，or v1//vn1
        if (sVert.size() == 3)
        {
            if (!sVert[1].empty())
            {
                // position, texture, and normal
                type = 4;
            }
            else
            {
                // position & normal
                type = 3;
            }
        }
        switch (type)
        {
        case 1: // P
        {
            vVert.position = getElement(positions, sVert[0]);
            vVert.texCoords = { 0, 0 };
            noNormal = true;
            verts.emplace_back(vVert);
            break;
        }
        case 2: // P/T
        {
            vVert.position = getElement(positions, sVert[0]);
            vVert.texCoords = getElement(texCoords, sVert[1]);
            noNormal = true;
            verts.emplace_back(vVert);
            break;
        }
        case 3: // P//N
        {
            vVert.position = getElement(positions, sVert[0]);
            vVert.texCoords = { 0, 0 };
            vVert.normal = getElement(normals, sVert[2]);
            verts.emplace_back(vVert);
            break;
        }
        case 4: // P/T/N
        {
            vVert.position = getElement(positions, sVert[0]);
            vVert.texCoords = getElement(texCoords, sVert[1]);
            vVert.normal = getElement(normals, sVert[2]);
            verts.emplace_back(vVert);
            break;
        }
        default: {
            break;
        }
        }
    }
    /// 给一个假的法线
    if (noNormal)
    {
        Vector3 A = verts[0].position - verts[1].position;
        Vector3 B = verts[2].position - verts[1].position;
        Vector3 normal;
        Vector3::cross(A, B, &normal);
        for (auto& vert : verts)
        {
            vert.normal = normal;
        }
    }
}

void ObjLoader::vertexTriangluation(std::vector<unsigned int>& indices,
                                    const std::vector<Vertex>& verts)
{
    // if there are 2 or less verts,no triangle can be created,
    if (verts.size() < 3)
    {
        return;
    }
    // if it is a triangle no need to calculate it
    if (verts.size() == 3)
    {
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        return;
    }
    // create a list of vertices
    std::vector<Vertex> vertLists = verts;
    while (true)
    {
        for (int i = 0; i < (int)vertLists.size(); ++i)
        {
            // pPrev = the previous vertex in the list
            Vertex pPrev;
            if (i == 0)
            {
                pPrev = vertLists[vertLists.size() - 1];
            }
            else
            {
                pPrev = vertLists[i - 1];
            }

            // pCur = the current vertex;
            Vertex pCur = vertLists[i];

            // pNext = the next vertex in the list
            Vertex pNext;
            if (i == vertLists.size() - 1)
            {
                pNext = vertLists[0];
            }
            else
            {
                pNext = vertLists[i + 1];
            }

            // check to see if there are only 3 verts left
            // if so this is the last triangle
            if (vertLists.size() == 3)
            {
                // Create a triangle from pCur, pPrev, pNext
                for (int j = 0; j < (int)vertLists.size(); ++j)
                {
                    if (verts[j].position == pCur.position)
                        indices.emplace_back(j);
                    if (verts[j].position == pPrev.position)
                        indices.emplace_back(j);
                    if (verts[j].position == pNext.position)
                        indices.emplace_back(j);
                }
                vertLists.clear();
                break;
            }
            if (vertLists.size() == 4)
            {
                // Create a triangle from pCur, pPrev, pNext
                for (int j = 0; j < int(verts.size()); ++j)
                {
                    if (verts[j].position == pCur.position)
                        indices.emplace_back(j);
                    if (verts[j].position == pPrev.position)
                        indices.emplace_back(j);
                    if (verts[j].position == pNext.position)
                        indices.emplace_back(j);
                }
                Vector3 tempVec;
                for (auto& vertList : vertLists)
                {
                    if (vertList.position != pCur.position && vertList.position != pPrev.position && vertList.position != pNext.position)
                    {
                        tempVec = vertList.position;
                        break;
                    }
                }
                // create a triangle from pCur, pPrev, pNext
                for (int j = 0; j < int(vertLists.size()); ++j)
                {
                    if (verts[j].position == pPrev.position)
                        indices.emplace_back(j);
                    if (verts[j].position == pNext.position)
                        indices.emplace_back(j);
                    if (verts[j].position == tempVec)
                        indices.emplace_back(j);
                }
                vertLists.clear();
                break;
            }

            // if Vertex is not an interior vertex
            float angle = Vector3::angle(pPrev.position - pCur.position, pNext.position - pCur.position) * (180.0f / 3.14159265359f);
            if (angle <= 0 && angle >= 180)
                continue;
            // if any vertices are within this triangle
            bool inTri = false;
            for (const auto& vert : verts)
            {
                if (inTriangle(vert.position, pPrev.position, pCur.position, pNext.position) && vert.position != pPrev.position && vert.position != pCur.position && vert.position != pNext.position)
                {
                    inTri = true;
                    break;
                }
            }

            if (inTri)
                continue;

            // create a triangle from pCur, pPrev, pNext
            for (int j = 0; j < int(verts.size()); ++j)
            {
                if (verts[j].position == pCur.position)
                    indices.emplace_back(j);
                if (verts[j].position == pPrev.position)
                    indices.emplace_back(j);
                if (verts[j].position == pNext.position)
                    indices.emplace_back(j);
            }

            // Delete pCur from the list
            for (int j = 0; j < int(vertLists.size()); j++)
            {
                if (vertLists[j].position == pCur.position)
                {
                    vertLists.erase(vertLists.begin() + j);
                    break;
                }
            }
            // reset i to the start
            // -1 since loop will add 1 to it
            i = -1;
        }
        // if no triangles were created
        if (indices.empty())
            break;
        // if no more vertices
        if (vertLists.empty())
            break;
    }
}

bool ObjLoader::loadMaterials(std::string& path)
{
    ASSERT(!path.empty());
    if (path.substr(path.size() - 4, path.size()) != ".mtl")
        return false;
    std::ifstream file(path);
    // if the file is not found return false
    if (!file.is_open())
        return false;

    Material material;
    bool listening = false;

    std::string curline;
    while (std::getline(file, curline))
    {
        // new material and material name
        auto firstTokenStr = firstToken(curline);
        if (firstTokenStr == "newmtl")
        {
            if (!listening)
            {
                listening = true;
                material.name = curline.size() > 7 ? tail(curline) : "none";
            }
            else
            {
                m_materials.emplace_back(material);
                // Clear Loaded Material
                material = Material();
                material.name = curline.size() > 7 ? tail(curline) : "none";
            }
        }
        // ambient Color
        if (firstTokenStr == "Ka")
        {
            auto ambient = split(tail(curline), " ");
            if (ambient.size() != 3)
                continue;
            material.Ka.set(std::stof(ambient[0]), std::stof(ambient[1]), std::stof(ambient[2]));
        }

        // diffuse Color
        if (firstTokenStr == "Kd")
        {
            auto diffuse = split(tail(curline), " ");
            if (diffuse.size() != 3)
                continue;
            material.Kd.set(std::stof(diffuse[0]), std::stof(diffuse[1]), std::stof(diffuse[2]));
        }

        // specular Color
        if (firstTokenStr == "Ks")
        {
            auto specular = split(tail(curline), " ");
            if (specular.size() != 3)
                continue;
            material.Ks.set(std::stof(specular[0]), std::stof(specular[1]), std::stof(specular[2]));
        }

        // specular exponent
        if (firstTokenStr == "Ns")
        {
            material.Ns = std::stof(tail(curline));
        }

        // optical Density
        if (firstTokenStr == "Ni")
        {
            material.Ni = std::stof(tail(curline));
        }

        // dissolve
        if (firstTokenStr == "d")
        {
            material.dissolve = std::stof(tail(curline));
        }

        // Illumination
        if (firstTokenStr == "illum")
        {
            material.Illumination = std::stof(tail(curline));
        }

        // ambient texture map
        if (firstTokenStr == "map_Ka")
        {
            material.mapKa = tail(curline);
        }

        // diffuse texture map
        if (firstTokenStr == "map_Kd")
        {
            material.mapKd = tail(curline);
        }

        // specular texture map
        if (firstTokenStr == "map_Ks")
        {
            material.mapKs = tail(curline);
        }

        // specular hightlight map
        if (firstTokenStr == "map_Ns")
        {
            material.mapNs = tail(curline);
        }

        // alpha texture map
        if (firstTokenStr == "map_d")
        {
            material.mapAlpha = tail(curline);
        }

        // bump Map
        if (firstTokenStr == "map_Bump" || firstTokenStr == "map_bump" || firstTokenStr == "bump")
        {
            material.mapBump = tail(curline);
        }
    }
    m_materials.emplace_back(material);
    return !m_materials.empty();
}

Mesh::Mesh(std::vector<Vertex>& vertices_, std::vector<unsigned int>& indices_, std::string name_) :
    vertices(vertices_), indices(indices_), name(std::move(name_))
{
}
} // namespace graphics