#include "integrator.h"
#include "image.h"
#include "ctpl_stl.h"


#include <iostream>

namespace fst
{
    Integrator::Integrator(const parser::Scene& parser)
    {
        m_scene.loadFromParser(parser);
        transformation();
    }

    math::Vector3f Integrator::renderPixel(const Ray& ray, int depth) const
    {
        if (depth > m_scene.max_recursion_depth)
        {
            return math::Vector3f(0.0f, 0.0f, 0.0f);
        }

        HitRecord hit_record;
        auto result = m_scene.intersect(ray, hit_record, std::numeric_limits<float>::max());

        if (!result)
        {
            return m_scene.background_color;
        }

        auto& material = m_scene.materials[hit_record.material_id - 1];
        auto color = material.get_ambient() * m_scene.ambient_light;
        auto intersection_point = ray.getPoint(hit_record.distance);

        for (auto& light : m_scene.point_lights)
        {
            auto to_light = light.get_position() - intersection_point;
            auto light_pos_distance = math::length(to_light);
            to_light = to_light / light_pos_distance;

            Ray shadow_ray(intersection_point + m_scene.shadow_ray_epsilon * to_light, to_light);

            if (!m_scene.intersectShadowRay(shadow_ray, light_pos_distance))
            {
                color = color + light.computeRadiance(light_pos_distance) * material.computeBrdf(to_light, -ray.get_direction(), hit_record.normal);
            }
        }

        auto& mirror = material.get_mirror();
        if (mirror.x + mirror.y + mirror.z > 0.0f)
        {
            auto new_direction = math::reflect(ray.get_direction(), hit_record.normal);
            Ray secondary_ray(intersection_point + m_scene.shadow_ray_epsilon * new_direction, new_direction);

            return color + mirror * renderPixel(secondary_ray, depth + 1);
        }
        else
        {
            return color;
        }
    }




    void Integrator::integrate() const
    {
        for (auto& camera : m_scene.cameras)
        {
            auto& resolution = camera.get_screen_resolution();

            Image image(resolution.x, resolution.y);

            ctpl::thread_pool pool(128);
            for (int i = 0; i < resolution.x; ++i)
            {
                pool.push([i, &resolution, &camera, &image, this](int id) {
                    for (int j = 0; j < resolution.y; ++j)
                    {
                        auto ray = camera.castPrimayRay(static_cast<float>(i), static_cast<float>(j));
                        auto color = renderPixel(ray, 0);
                        image.setPixel(i, j, math::clamp(color, 0.0f, 255.0f));
                    }
                });
            }
            pool.stop(true);

            image.save(camera.get_image_name());
        }
    }



    using namespace std ;
    void printer(Matrix m)
    {
        cout.precision(2);
        cout << "Matrix:" << endl;
        for(int i=0; i<4; i++) {
            cout << "[";
            for(int j=0; j<4; j++) {
                cout << m.m[i][j] << " ";
            }
            cout << "]" << endl;

        }
    }

    void printer(math::Vector4f m)
    {
        cout.precision(2);
        cout << "Vector:" << endl;
            cout << "[";
            cout << m.x << " " << m.y << " " << m.z << " " << m.w;
            cout << "]" << endl;

    }

    void printer(Triangle m)
    {
        cout.precision(2);
        cout << "Triangle:" << endl;
        cout << "vertex";
        cout << m.m_v0.x << " " << m.m_v0.y << " " << m.m_v0.z << endl;
        cout << "edge1";
        cout << m.m_edge1.x << " " << m.m_edge1.y << " " << m.m_edge1.z << endl;
        cout << "edge2";
        cout << m.m_edge2.x << " " << m.m_edge2.y << " " << m.m_edge2.z << endl;
        cout << "normal";
        cout << m.m_normal.x << " " << m.m_normal.y << " " << m.m_normal.z << endl;
        cout << "]" << endl;

    }

    void Integrator::transformation() { // todo @param: Vec3f position, pass it to Transformation constructor.

        transformSpheres();
        transformMeshes();

        //tester position
        /*math::Vector3f pos;
        pos.x = 3;
        pos.y = 4;
        pos.z = 5;

        // translations
        int translationSize = m_scene.translations.size();
        for(int i=0; i<translationSize; i++) {
            Translation curr = m_scene.translations[i];
            Transformation tr = Transformation(curr, pos);
            //printer(tr.matrix);
            //printer(tr.p_prime);
        }

        int scalingSize = m_scene.scalings.size();
        for(int i=0; i<scalingSize; i++) {
            Scaling curr = m_scene.scalings[i];
            Transformation tr = Transformation(curr, pos);
            cout << "Scaling:" << endl;
            printer(tr.matrix);
            printer(tr.p_prime);
        }

        int rotationSize = m_scene.rotations.size();
        for(int i=0; i<rotationSize; i++) {
            Rotation curr = m_scene.rotations[i];
            Transformation tr = Transformation(curr, pos);
            cout << "Rotation:" << endl;
            printer(tr.matrix);
            printer(tr.p_prime);
        }*/
    }

    void Integrator::transformSpheres() {

            int sphereSize = m_scene.spheres.size();
            for(int i=0; i< sphereSize; i++) {
                Sphere curr = m_scene.spheres[i];

                std::string delimiter = " ";
                size_t pos = 0;
                std::string transformation;
                Matrix final;
                while ((pos = curr.transformations.find(delimiter)) != std::string::npos) {

                    transformation = curr.transformations.substr(0, pos);

                    final = final.multiply(applyOneTransformSpheres(transformation, curr));

                    // debug
                    //printer(final);
                    //printer(applyOneTransformSpheres(transformation, curr));


                    curr.transformations.erase(0, pos + delimiter.length());
                }

                if(curr.transformations.length()) final = final.multiply(applyOneTransformMeshes(curr.transformations));

                math::Vector4f currPos = math::Vector4f(curr.m_center.x, curr.m_center.y, curr.m_center.z, 1);
                math::Vector4f newPos = final.multiply(currPos);
                m_scene.spheres[i].m_center.x = newPos.x;
                m_scene.spheres[i].m_center.y = newPos.y;
                m_scene.spheres[i].m_center.z = newPos.z;

            }

    }

    Matrix Integrator::applyOneTransformSpheres(std::string tr, Sphere sp) {
        char transformationType = tr[0];
        int transformationID = std::stoi(tr.erase(0, 1));
        switch (transformationType) {
            case ('t'):
                return m_scene.translations[transformationID - 1].resultMatrix();
                break;
            case ('r'): // todo kendine gel kendi centerında rotate edilmiş mi bakmadın
                return m_scene.rotations[transformationID - 1].resultMatrix();
                break;
            case ('s'):
                double newRadius = m_scene.scalings[transformationID - 1].sphereScaling(sp.m_radius);
                if(newRadius >= 0) {
                    sp.m_radius = newRadius;
                    return m_scene.scalings[transformationID - 1].resultMatrix();
                }
                Matrix m;
                return m;
                break;
        }
    }

    void Integrator::transformMeshes() {
        int meshSize = m_scene.meshes.size();
        for(int i=0; i<meshSize; i++) {
            Mesh curr = m_scene.meshes[i];

            std::string delimiter = " ";
            size_t pos = 0;
            std::string transformation;
            Matrix final;
            while ((pos = curr.transformations.find(delimiter)) != std::string::npos) {


                transformation = curr.transformations.substr(0, pos);

                final = final.multiply(applyOneTransformMeshes(transformation));

                // debug
                cout << "Transform mesh" << endl;
                 printer(final);
                //printer(applyOneTransformMeshes(transformation));


                curr.transformations.erase(0, pos + delimiter.length());
            }

            // take last transformation

            if(curr.transformations.length()) final = final.multiply(applyOneTransformMeshes(curr.transformations));

            int triSize = curr.m_triangles.size();

            for(int t=0; t<triSize; t++) {

                Triangle currTri = curr.m_triangles[t];

                m_scene.meshes[i].m_triangles[t] = transformOneTriangle(final, currTri);

                double d = 5;

            }

            double d = 3;

        }
    }

    Matrix Integrator::applyOneTransformMeshes(std::string tr) {
        char transformationType = tr[0];
        int transformationID = std::stoi(tr.erase(0, 1));
        switch (transformationType) {
            case ('t'):
                return m_scene.translations[transformationID - 1].resultMatrix();
                break;
            case ('r'):
                return m_scene.rotations[transformationID - 1].resultMatrix();
                break;
            case ('s'):
                return m_scene.scalings[transformationID - 1].resultMatrix();
                break;
        }    }

    Triangle Integrator::transformOneTriangle(Matrix matrix, Triangle triangle) {

        math::Vector3f vertice1 = triangle.m_v0 + triangle.m_edge1;
        math::Vector3f vertice2 = triangle.m_v0 + triangle.m_edge2;

        math::Vector4f vertice0Vec = math::Vector4f(triangle.m_v0.x, triangle.m_v0.y, triangle.m_v0.z, 1);
        math::Vector4f vertice1Vec = math::Vector4f(vertice1.x, vertice1.y, vertice1.z, 1);
        math::Vector4f vertice2Vec = math::Vector4f(vertice2.x, vertice2.y, vertice2.z, 1);

        vertice0Vec = matrix.multiply(vertice0Vec);
        vertice1Vec = matrix.multiply(vertice1Vec);
        vertice2Vec = matrix.multiply(vertice2Vec);

        //triangle.m_v0.x = vertice0Vec.x;
        //triangle.m_v0.y = vertice0Vec.y;
        //triangle.m_v0.z = vertice0Vec.z;
        math::Vector3f v0 = math::Vector3f(vertice0Vec.x, vertice0Vec.y, vertice0Vec.z);

        //triangle.m_edge1.x = vertice1Vec.x - triangle.m_v0.x;
        //triangle.m_edge1.y = vertice1Vec.y - triangle.m_v0.y;
        //triangle.m_edge1.z = vertice1Vec.z - triangle.m_v0.z;
        math::Vector3f e1 = math::Vector3f(vertice1Vec.x - vertice0Vec.x, vertice1Vec.y - vertice0Vec.y, vertice1Vec.z - vertice0Vec.z);

        //triangle.m_edge2.x = vertice2Vec.x - triangle.m_v0.x;
        //triangle.m_edge2.y = vertice2Vec.y - triangle.m_v0.y;
        //triangle.m_edge2.z = vertice2Vec.z - triangle.m_v0.z;
        math::Vector3f e2 = math::Vector3f(vertice2Vec.x - vertice0Vec.x, vertice2Vec.y - vertice0Vec.y, vertice2Vec.z - vertice0Vec.z);


        // todo : transform triangle normal

        return Triangle(v0, e1, e2);
    }
}
