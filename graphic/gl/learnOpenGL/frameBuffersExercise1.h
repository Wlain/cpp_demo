//
// Created by william on 2022/4/9.
//

#ifndef CPP_DEMO_FRAMEBUFFERSEXERCISE1_H
#define CPP_DEMO_FRAMEBUFFERSEXERCISE1_H

#include "frameBuffers.h"

namespace graphicEngine::gl
{
class FrameBuffersExercise1 : public FrameBuffers
{
public:
    using FrameBuffers::FrameBuffers;
    ~FrameBuffersExercise1() override;
    void render() override;
protected:
    void initVertices() override;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_FRAMEBUFFERSEXERCISE1_H
