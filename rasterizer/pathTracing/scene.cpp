//
// Created by william on 2021/11/24.
//

#include "scene.h"
namespace graphics::pathTracing
{
Scene::Scene(int w, int h) :
    m_width(w),
    m_height(h)
{
}
Scene::~Scene() = default;

void Scene::add(std::unique_ptr<Object> object)
{
}
void Scene::add(std::unique_ptr<Ray> light)
{
}
} // namespace graphics