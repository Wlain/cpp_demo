//
// Created by cwb on 2022/4/19.
//

#ifndef CPP_DEMO_SHADOWMAPPINGBASE_H
#define CPP_DEMO_SHADOWMAPPINGBASE_H

#include "shadowMappingDepth.h"

namespace graphicEngine::gl
{
class ShadowMappingBase : public ShadowMappingDepth
{
public:
    void render() override;
    void update(float elapseTime) override;

protected:
    void initTextures() override;
    void initPrograms() override;
    virtual void renderScene(const std::unique_ptr<ProgramGL>& program);

protected:
    std::unique_ptr<TextureGL> m_woodTexture;
    std::unique_ptr<ProgramGL> m_programShadowBase;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_SHADOWMAPPINGBASE_H
