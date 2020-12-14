#pragma once

#include <string>
#include "vector3f.h"
#include "transformation.h"

namespace fst
{
    class Ray;
    struct HitRecord;
    class Scene;

    class Sphere
    {
    public:
        std::string transformations;
        math::Vector3f m_center;
        float m_radius;

        Sphere(const math::Vector3f& center, float radius, int material_id, std::string  transforms);
        bool intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const;
        bool intersectShadowRay(const Ray& ray, float max_distance) const;

    private:
        int m_material_id;
    };
}