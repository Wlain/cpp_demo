//
// Created by cwb on 2022/2/21.
//

#include "imageWarping/rbfWarping.h"

RbfWarping::~RbfWarping() = default;

Vector2 RbfWarping::targetFunction(const Vector2& input)
{
    return BaseWarping::targetFunction(input);
}

void RbfWarping::render(QImage& image, const QImage& m_originImage)
{
}

float RbfWarping::getRadiusI()
{
    float min = INT_MAX;
    auto size = m_pointP.size();
    for (int i = 0; i <size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {

        }
    }

    return 0;
}
