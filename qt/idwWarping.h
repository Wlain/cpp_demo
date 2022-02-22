//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_IDWWARPING_H
#define CPP_DEMO_IDWWARPING_H
#include "baseWarping.h"

class IdwWarping final : public BaseWarping
{
public:
    IdwWarping();
    ~IdwWarping() override;
};

#endif //CPP_DEMO_IDWWARPING_H
