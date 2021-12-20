//
// Created by william on 2021/12/20.
//

#ifndef CPP_DEMO_HITABLELIST_H
#define CPP_DEMO_HITABLELIST_H
#include "hitable.h"

namespace rayTracing
{
class HitAbleList : public HitAble
{
public:
    HitAbleList() = default;
    HitAbleList(HitAble** l, int n) :
        list(l), listSize(n){};
    ~HitAbleList() = default;
    bool hit(const Ray& r, float timeMin, float timeMax, HitRecord& record) const override
    {
        HitRecord tempRecord;
        bool hitAnything = false;
        double closestSoFar = timeMax;
        for (int i = 0; i < listSize; ++i)
        {
            if(list[i]->hit(r, timeMin, closestSoFar, tempRecord))
            {
                hitAnything = true;
                closestSoFar = tempRecord.t;
                record = tempRecord;
            }
        }
        return hitAnything;
    }

public:
    HitAble** list;
    int listSize;
};
} // namespace rayTracing

#endif //CPP_DEMO_HITABLELIST_H
