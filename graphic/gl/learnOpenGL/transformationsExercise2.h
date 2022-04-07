//
// Created by cwb on 2022/4/7.
//

#ifndef CPP_DEMO_TransformationsExercise2_H
#define CPP_DEMO_TransformationsExercise2_H
#include "texturesCombined.h"

namespace graphicEngine::gl
{
class TransformationsExercise2 : public TexturesCombined
{
public:
    using TexturesCombined::TexturesCombined;
    void render() override;
    ~TransformationsExercise2() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;

private:
    float m_elapseTime = 0.0f;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_TransformationsExercise2_H
