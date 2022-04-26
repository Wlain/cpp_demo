//
// Created by cwb on 2022/4/20.
//

#ifndef CPP_DEMO_POINTSHADOWMAPPING_H
#define CPP_DEMO_POINTSHADOWMAPPING_H
#include "shadowMappingBase.h"
namespace graphicEngine::gl
{
class PointShadowMapping : public ShadowMappingBase
{
public:
    void render() override;
    void update(float elapseTime) override;

protected:
    void processInput() override;

protected:
    void renderDepthImage(const ProgramGL* program) override;
    void drawCubes(const ProgramGL* program) override;

protected:
    void initDepthFbo() override;
    void initPrograms() override;

private:
    bool m_shadows = true;
    bool m_shadowsKeyPressed = false;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_POINTSHADOWMAPPING_H
