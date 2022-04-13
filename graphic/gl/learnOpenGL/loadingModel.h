//
// Created by cwb on 2022/4/12.
//

#ifndef CPP_DEMO_LOADINGMODEL_H
#define CPP_DEMO_LOADINGMODEL_H
#include "depthTest.h"
#include "model.h"
namespace graphicEngine::gl
{
class LoadingModel : public DepthTest
{
public:
    ~LoadingModel() override;
    void initialize() override;
    void update(float elapseTime) override;
    void render() override;

private:
    std::shared_ptr<graphicEngine::Program> m_modelProgram;
    std::unique_ptr<Model> m_model;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_LOADINGMODEL_H
