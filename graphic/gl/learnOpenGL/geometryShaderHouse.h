//
// Created by cwb on 2022/4/11.
//

#ifndef CPP_DEMO_GEOMETRYSHADERHOUSE_H
#define CPP_DEMO_GEOMETRYSHADERHOUSE_H
#include "gl/common/commonInterface.h"
namespace graphicEngine::gl
{

class GeometryShaderHouse : public CommonInterface
{
public:
    ~GeometryShaderHouse() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

private:
    std::vector<float> m_points;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_GEOMETRYSHADERHOUSE_H
