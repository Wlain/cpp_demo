//
// Created by cwb on 2022/4/13.
//

#ifndef CPP_DEMO_GEOMETRYSHADERNORMALS_H
#define CPP_DEMO_GEOMETRYSHADERNORMALS_H
#include "loadingModel.h"

namespace graphicEngine::gl
{
class GeometryShaderNormals : public LoadingModel
{
public:
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

protected:
    void initPrograms() override;

private:
    std::unique_ptr<graphicEngine::ProgramGL> m_normalsProgram;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_GEOMETRYSHADERNORMALS_H
