//
// Created by william on 2021/11/18.
//
#include "animation.h"
#include "background.h"
#include "cube.h"
#include "instancing.h"
#include "massSpringSystem/massSpringSystem.h"
#include "point.h"
#include "sharing.h"
#include "showGLInfo.h"
#include "triangle.h"
#include "triangleIndex.h"
#include "window.h"
#include "windowClear.h"

using namespace graphicEngine::gl;

void glTest()
{
    MassSpringSystem obj;
    obj.run();
}