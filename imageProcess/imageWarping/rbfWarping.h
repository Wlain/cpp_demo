//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_RBFWARPING_H
#define CPP_DEMO_RBFWARPING_H
#include "imageWarping/baseWarping.h"
#include "vec2.h"
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
    void calcAi();
    /// 高斯基函数
    double basicFunction(const Vector2& start, const Vector2& end, float ri);

private:
    std::vector<float> m_radius;
    Vec2<Eigen::VectorXd> m_Ai;
};

#endif //CPP_DEMO_RBFWARPING_H
