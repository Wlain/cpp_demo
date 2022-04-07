//
// Created by cwb on 2022/4/7.
//

#ifndef CPP_DEMO_COORDINATESYSTEMS_H
#define CPP_DEMO_COORDINATESYSTEMS_H
#include "texturesCombined.h"

namespace graphicEngine::gl
{
class CoordinateSystems : public TexturesCombined
{
public:
    using TexturesCombined::TexturesCombined;
    void render() override;
    ~CoordinateSystems() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_COORDINATESYSTEMS_H
