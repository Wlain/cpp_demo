//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_RBFWARPING_H
#define CPP_DEMO_RBFWARPING_H
#include "imageWarping/baseWarping.h"

class RbfWarping final : public BaseWarping
{
public:
    using BaseWarping::BaseWarping;
    ~RbfWarping() override;
};

#endif //CPP_DEMO_RBFWARPING_H
