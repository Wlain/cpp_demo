//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_BASEWARPING_H
#define CPP_DEMO_BASEWARPING_H
#include <vector>
#include "vector2.h"

class BaseWarping
{
public:
    BaseWarping();
    BaseWarping(std::vector<Vector2> p, std::vector<Vector2> q);
    virtual ~BaseWarping();
    virtual Vector2 targetFunction(const Vector2& input);
    virtual float weightFunction(const Vector2& point, const Vector2& start);
    virtual Vector2 basicFunction(const Vector2& point, const Vector2& start, const Vector2& end);
    void setPointP(const std::vector<Vector2>& mPointP);
    void setPointQ(const std::vector<Vector2>& mPointQ);

protected:
    std::vector<Vector2> m_pointP, m_pointQ;
};

#endif //CPP_DEMO_BASEWARPING_H
