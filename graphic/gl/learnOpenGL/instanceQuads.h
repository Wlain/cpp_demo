//
// Created by cwb on 2022/4/13.
//

#ifndef CPP_DEMO_INSTANCEQUADS_H
#define CPP_DEMO_INSTANCEQUADS_H
#include "gl/common/commonInterface.h"
namespace graphicEngine::gl
{
class InstanceQuads : public CommonInterface
{
public:
    ~InstanceQuads() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

private:
    std::vector<float> m_quadVertices;
    uint32_t m_instanceVbo;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_INSTANCEQUADS_H
