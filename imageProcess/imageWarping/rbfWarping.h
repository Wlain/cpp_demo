//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_RBFWARPING_H
#define CPP_DEMO_RBFWARPING_H
#include "imageWarping/baseWarping.h"

#include <eigen3/Eigen/Dense>

/// 径向基函数插值算法（Radial Basis Functions）

class RbfWarping final : public BaseWarping
{
public:
    RbfWarping(std::vector<Vector2> p, std::vector<Vector2> q);
    ~RbfWarping() override;
    Vector2 targetFunction(const Vector2& input) override;
    /// calc ri
    void calcRadius();
    /// calc gi
    void calcWeight();
    std::tuple<Eigen::VectorXd, Eigen::VectorXd> getAi();

private:
    std::vector<float> m_radius;
    Eigen::VectorXd m_weightX, m_weightY;
};

#endif //CPP_DEMO_RBFWARPING_H
