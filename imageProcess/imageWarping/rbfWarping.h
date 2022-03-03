//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_RBFWARPING_H
#define CPP_DEMO_RBFWARPING_H
#include "imageWarping/baseWarping.h"

/// 径向基函数插值算法（Radial Basis Functions）

class RbfWarping final : public BaseWarping
{
public:
    using BaseWarping::BaseWarping;
    ~RbfWarping() override;
    Vector2 targetFunction(const Vector2& input) override;

private:

};

#endif //CPP_DEMO_RBFWARPING_H
