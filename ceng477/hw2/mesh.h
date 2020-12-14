#pragma once

#include "triangle.h"

#include <vector>
#include <string>

namespace fst
{
    class Mesh : public Triangular
    {
    public:
        std::string transformations;

        std::vector<Triangle> m_triangles;
        Mesh(const std::vector<Triangle>& triangles, int material_id, std::string  transforms);
        Mesh(std::vector<Triangle>&& triangles, int material_id, std::string  transforms);

        bool intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const override;
        bool intersectShadowRay(const Ray& ray, float max_distance) const override;

    private:
        int m_material_id;
    };
}
