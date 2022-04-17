//
// Created by cwb on 2022/4/12.
//

#ifndef CPP_DEMO_LOADINGMODEL_H
#define CPP_DEMO_LOADINGMODEL_H
#include "depthTest.h"
#include "common/model.h"
namespace graphicEngine::gl
{
class LoadingModel : public DepthTest
{
public:
    ~LoadingModel() override;
    void render() override;

protected:
    void initModel() override;
    void initPrograms() override;

protected:
    std::unique_ptr<Model> m_model;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_LOADINGMODEL_H
