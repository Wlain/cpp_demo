//
// Created by william on 2021/11/18.
//
#include "WindowClear.h"
#include "animation.h"
#include "background.h"
#include "cube.h"
#include "instancing.h"
#include "point.h"
#include "sharing.h"
#include "showGLInfo.h"
#include "triangle.h"
#include "triangleIndex.h"
#include "window.h"

using namespace graphicEngine::gl;

void glTest()
{
    Triangle obj;
    obj.initialize();
    obj.draw();
}