//
// Created by cwb on 2022/4/13.
//

#ifndef CPP_DEMO_GEOMETRYSHADEREXPLODING_H
#define CPP_DEMO_GEOMETRYSHADEREXPLODING_H
#include "depthTest.h"
#include "model.h"
namespace graphicEngine::gl
{
class GeometryShaderExploding : public DepthTest
{
public:
    ~GeometryShaderExploding() override;
    void update(float elapseTime) override;
    void render() override;

protected:
    void initPrograms() override;
    void initModel() override;

private:
    std::unique_ptr<Model> m_model;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_GEOMETRYSHADEREXPLODING_H
