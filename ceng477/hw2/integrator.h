#pragma once

#include "scene.h"
#include "transformation.h"

namespace fst
{
    class Integrator
    {
    public:
        Integrator(const parser::Scene& parser);

        math::Vector3f renderPixel(const Ray& ray, int depth) const;
        void integrate() const;
        void transformation();


    private:
        Scene m_scene;
        void transformSpheres();
        void transformMeshes();
        Triangle transformOneTriangle(Matrix matrix, Triangle triangle);
        Matrix applyOneTransformSpheres(std::string tr, Sphere sp);
        Matrix applyOneTransformMeshes(std::string tr);
    };
}