//
// Created by william on 2022/4/7.
//

#ifndef CPP_DEMO_MATERIALS_H
#define CPP_DEMO_MATERIALS_H

#include "colors.h"
namespace graphicEngine::gl
{
class Materials : public Colors
{
public:
    using Colors::Colors;
    ~Materials() override;
    void render() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void initLighting() override;
    void initCube() override;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_MATERIALS_H
