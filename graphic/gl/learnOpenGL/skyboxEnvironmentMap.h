//
// Created by william on 2022/4/10.
//

#ifndef CPP_DEMO_SKYBOXENVIRONMENTMAP_H
#define CPP_DEMO_SKYBOXENVIRONMENTMAP_H
#include "skybox.h"

namespace graphicEngine::gl
{
class SkyboxEnvironmentMap : public Skybox
{
public:
    void update(float elapseTime) override;

protected:
    void initPrograms() override;
    void initVertices() override;
    void initVertexAttrib() override;
    void drawCubes() override;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_SKYBOXENVIRONMENTMAP_H
