//
// Created by william on 2021/11/24.
//

#ifndef CPP_DEMO_RENDERER_H
#define CPP_DEMO_RENDERER_H
#include "pathTracing/scene.h"

namespace graphics::pathTracing
{
class Renderer
{
public:
    void Render(const Scene& scene);
};
}
#endif //CPP_DEMO_RENDERER_H
