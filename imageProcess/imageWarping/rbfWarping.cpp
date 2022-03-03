//
// Created by cwb on 2022/2/21.
//

#include "imageWarping/rbfWarping.h"

RbfWarping::~RbfWarping() = default;

Vector2 RbfWarping::targetFunction(const Vector2& input)
{
    return BaseWarping::targetFunction(input);
}
