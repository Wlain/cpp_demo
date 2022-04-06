//
// Created by william on 2022/4/6.
//

#ifndef CPP_DEMO_TEXTUREEXERCISE2_H
#define CPP_DEMO_TEXTUREEXERCISE2_H
#include "texturesCombined.h"

namespace graphicEngine::gl
{
class TextureExercise2 : public TexturesCombined
{
public:
    using TexturesCombined::TexturesCombined;
    ~TextureExercise2() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_TEXTUREEXERCISE2_H
