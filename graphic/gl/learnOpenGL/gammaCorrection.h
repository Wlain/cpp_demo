//
// Created by cwb on 2022/4/15.
//

#ifndef CPP_DEMO_GAMMACORRECTION_H
#define CPP_DEMO_GAMMACORRECTION_H
#include "advancedLighting.h"
namespace graphicEngine::gl
{
class GammaCorrection : public AdvancedLighting
{
public:
    void update(float elapseTime) override;

protected:
    void initPrograms() override;
    void initVertices() override;
    void processInput() override;

private:
    std::vector<glm::vec3> m_lightPositions;
    std::vector<glm::vec3> m_lightColors;
    bool m_gammaEnabled;
    bool m_gammaKeyPressed;
};
} // namespace graphicEngine::gl
#endif //CPP_DEMO_GAMMACORRECTION_H
