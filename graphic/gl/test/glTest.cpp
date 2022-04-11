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
#include "learnOpenGL/helloTriangle.h"
#include "learnOpenGL/helloTriangleIndexed.h"
#include "learnOpenGL/helloTriangleExereise1.h"
#include "learnOpenGL/helloTriangleExereise2.h"
#include "learnOpenGL/helloTriangleExereise3.h"
#include "learnOpenGL/shadersUniform.h"
#include "learnOpenGL/shadersInterpolation.h"
#include "learnOpenGL/textures.h"
#include "learnOpenGL/texturesCombined.h"
#include "learnOpenGL/textureExercise2.h"
#include "learnOpenGL/textureExercise3.h"
#include "learnOpenGL/transformations.h"
#include "learnOpenGL/transformationsExercise2.h"
#include "learnOpenGL/coordinateSystems.h"
#include "learnOpenGL/coordinateSystemsDepth.h"
#include "learnOpenGL/coordinateSystemsMultiple.h"
#include "learnOpenGL/colors.h"
#include "learnOpenGL/basicLighting.h"
#include "learnOpenGL/materials.h"
#include "learnOpenGL/materialsMapTexture.h"
#include "learnOpenGL/depthTest.h"
#include "learnOpenGL/stencilTest.h"
#include "learnOpenGL/blendingDiscard.h"
#include "learnOpenGL/blendingSort.h"
#include "learnOpenGL/frameBuffers.h"
#include "learnOpenGL/frameBuffersExercise1.h"
#include "learnOpenGL/skybox.h"
#include "learnOpenGL/skyboxEnvironmentMap.h"
#include "learnOpenGL/advancedUbo.h"
#include "learnOpenGL/geometryShaderHouse.h"

using namespace graphicEngine::gl;

void glTest()
{
    GeometryShaderHouse obj;
    obj.run();
}