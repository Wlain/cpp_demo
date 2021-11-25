//
// Created by william on 2021/11/24.
//

#ifndef CPP_DEMO_SCENE_H
#define CPP_DEMO_SCENE_H
#include "light.h"
#include "object.h"
#include "vector3.h"

#include <memory>

namespace graphics::pathTracing
{
class Scene
{
public:
    Scene(int w, int h);
    ~Scene();

    void add(std::unique_ptr<Object> object);
    void add(std::unique_ptr<Light> light);
    inline const std::vector<std::unique_ptr<Object>>& getObjects() const
    {
        return m_objects;
    }
    inline const std::vector<std::unique_ptr<Light>>& getLight() const
    {
        return m_lights;
    }

private:
    std::vector<std::unique_ptr<Object>> m_objects;
    std::vector<std::unique_ptr<Light>> m_lights;
    Vector3 m_backgroundColor{ 0.235294, 0.67451, 0.843137 };
    int maxDepth = 5;
    int m_width = 0;
    int m_height = 0;
    float m_fov = 90.0f;
    float epsilon = 0.00001;
};
} // namespace graphics

#endif //CPP_DEMO_SCENE_H
