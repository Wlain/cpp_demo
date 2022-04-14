//
// Created by cwb on 2022/4/14.
//

#ifndef CPP_DEMO_ASTEROIDS_INSTANCE_H
#define CPP_DEMO_ASTEROIDS_INSTANCE_H
#include "asteroids.h"
namespace graphicEngine::gl
{
class AsteroidsInstanced : public Asteroids
{
protected:
    void initPrograms() override;
    void initVertexAttrib() override;
    void drawRocks() override;

private:
    uint32_t m_rockVbo = 0;
};
} // namespace graphicEngine::gl
#endif //CPP_DEMO_ASTEROIDS_INSTANCE_H
