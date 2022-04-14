//
// Created by cwb on 2022/4/14.
//

#ifndef CPP_DEMO_ASTEROIDS_H
#define CPP_DEMO_ASTEROIDS_H
#include "loadingModel.h"
namespace graphicEngine::gl
{
class Asteroids : public LoadingModel
{
public:
    ~Asteroids() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

protected:
    void initModel() override;
    void initPrograms() override;
    virtual void drawRocks();

protected:
    std::unique_ptr<Program> m_rockProgram;
    std::unique_ptr<Model> m_rockModel;
    glm::mat4* m_rockModelMatrices = nullptr;
    uint32_t m_rockAmount = 1000;
};
} // namespace graphicEngine::gl
#endif //CPP_DEMO_ASTEROIDS_H
