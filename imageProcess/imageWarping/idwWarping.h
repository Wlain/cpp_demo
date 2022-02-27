//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_IDWWARPING_H
#define CPP_DEMO_IDWWARPING_H
#include "baseWarping.h"

class IdwWarping final : public BaseWarping
{
public:
    using BaseWarping::BaseWarping;
    ~IdwWarping() override;

    Vector2 targetFunction(const Vector2& input) override;
    float weightFunction(const Vector2& point, const Vector2& start) override;
    /// 基函数：取线性基函数
    Vector2 basicFunction(const Vector2& point, const Vector2& start, const Vector2& end) override;
    /// 设置公式中的指数
    inline void setExponent(int e) { m_exponent = e; }
    /// 平滑函数
    float smoothnessFunction(const Vector2& pointP, const Vector2& pointPi) const;

private:
    int m_exponent = 2;
};

#endif //CPP_DEMO_IDWWARPING_H
