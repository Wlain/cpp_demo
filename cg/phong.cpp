//
// Created by william on 2021/11/29.
//

#include "mathDefine.h"
#include "cgCommonDefine.h"

namespace cg
{
void phongTest()
{
    auto* imageData = new Color3f[width * height];
    display("phongTest", imageData);
    delete[] imageData;
}
} // namespace cg