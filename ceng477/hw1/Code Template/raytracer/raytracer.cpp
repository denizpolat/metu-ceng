#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "raytracer.h"
#include <math.h>

typedef unsigned char RGBold[3];
using namespace parser;

Point3d crossProduct(Point3d a, Point3d b) {
    Point3d tmp;
    tmp.x = a.y*b.z-b.y*a.z;
    tmp.y = b.x*a.z-a.x*b.z;
    tmp.z = a.x*b.y-b.x*a.y;
    return tmp;
}

double dotProduct(Point3d a, Point3d b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double length(Point3d a) {
    return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}

Point3d normalizeVector(Point3d param) {
    Point3d result;
    double d = length(param);
    result.x = param.x / d;
    result.y = param.y / d;
    result.z = param.z / d;
    return result;
}

double distanceBetween(Point3d a, Point3d b) {
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
}

bool areSamePoints(Point3d a, Point3d b, double epsilon) {
    return (abs((a.x - b.x)) < epsilon) && (abs((a.y - b.y)) < epsilon) && (abs((a.z - b.z)) < epsilon);
}

Point3d scale(Point3d prev, double scalar) {
    Point3d result;
    result.x = prev.x * scalar;
    result.y = prev.y * scalar;
    result.z = prev.z * scalar;
    return result;
}

Point3d add(Point3d a, Point3d b) {
    Point3d result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Point3d sub(Point3d a, Point3d b) {
    Point3d result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

Point3d computeTriangleNormal( Point3d a, Point3d b, Point3d c) {
    return crossProduct(sub(b, a), sub(c, a));
}

Plane computePlaneEquation(  Point3d a, Point3d b, Point3d c ) {
    Point3d normal = computeTriangleNormal(a,b,c);
    Plane plane;
    plane.x = normal.x;
    plane.y = normal.y;
    plane.z = normal.z;
    plane.t = (-1) * dotProduct(a,normal); // TODO a dedim ama herhangi bir nokta olabilir
    return plane;
}

RGB correctRGBValue(Point3d rgb) { // TODO bu şey mi demek 2f
    RGB result;
    if(rgb.x > 255) result.r = 255;
    else result.r = round(rgb.x);
    if(rgb.y > 255) result.g = 255;
    else result.g = round(rgb.y);
    if(rgb.z > 255) result.b = 255;
    else result.b = round(rgb.z);
    return result;
}

double intersectSphere(Ray ray, Sphere sphere, Scene scene) {
    double delta, a, b, c, r = sphere.radius, t, tmp;
    Point3d p;
    Point3d center = scene.vertex_data[sphere.center_vertex_id-1]; // if id=1, then index=0
    c = dotProduct(sub(ray.start, center), sub(ray.start, center)) - r*r;
    b = 2 * dotProduct(ray.direction, sub(ray.start, center));
    a = dotProduct(ray.direction, ray.direction);
    delta = b*b-4*a*c;
    if(delta < 0) t = -1; //no intersection
    else { // 2 points, find closer
        double tmp1, tmp2;
        delta = sqrtf(delta);
        tmp1 = (-1 * b + delta) / (2 * a);
        tmp2 = (-1 * b - delta) / (2 * a);
        tmp = fmin(tmp1, tmp2); // tmp = smallest of tmp1,2, no matter what!
        t = tmp;
    }
    return t;
}

double intersectTriangle(Ray r, Triangle triangle, Scene scene)
{
    Point3d vertex_a, vertex_b, vertex_c; // vertices of the triangle
    double beta,gamma,t; // barycentric coordinates alpha beta gamma and parameter for the ray
    double a,b,c,d,e,f,g,h,i,j,k,l; // elements of matrix
    double eimhf,gfmdi,dhmeg,akmjb,jcmal,blmkc; // some multiplications for the matrix
    double det; // determinant


    // vertices of the triangle

    vertex_a = scene.vertex_data[triangle.indices.v0_id -1];
    vertex_b = scene.vertex_data[triangle.indices.v1_id -1];
    vertex_c = scene.vertex_data[triangle.indices.v2_id -1];


    /*  equation system : A * x = res
     | a d g |    |  beta |      | j |
     | b e h | x  | gamma |  =   | k |
     | c f i |    |   t   |      | l |

    */
    a = vertex_a.x-vertex_b.x;
    b = vertex_a.y-vertex_b.y;
    c = vertex_a.z-vertex_b.z;

    d = vertex_a.x-vertex_c.x;
    e = vertex_a.y-vertex_c.y;
    f = vertex_a.z-vertex_c.z;

    g = r.direction.x;
    h = r.direction.y;
    i = r.direction.z;
    // a = 1; b = 5; c = 4; d = 2; e = 2; f = 5; g = 4; h = 3; i = 6; det = 29

    j = vertex_a.x-r.start.x;
    k = vertex_a.y-r.start.y;
    l = vertex_a.z-r.start.z;

    // some multiplications to compute determinants
    eimhf = e*i-h*f;  // ei multiplied by hf
    gfmdi = g*f-d*i;  // gf multiplied by di
    dhmeg = d*h-e*g;
    akmjb = a*k-j*b;
    jcmal = j*c-a*l;
    blmkc = b*l-k*c;

    det = a*eimhf + b*gfmdi + c*dhmeg;  // determinant of the coefficients matrix A
    if (det==0) return -1;            // if det = 0, it was something bad i guess.
    t = -(f*akmjb+e*jcmal+d*blmkc)/det;   // Cramer's Rule

    // if (t<1.0) return -1;               // t must be greater than 1, to be able too seen by camera  // TODO this threshold may need to be removed


    gamma = (i*akmjb+h*jcmal+g*blmkc)/det;  // Cramer's Rule

    if (gamma<0 || gamma>1) return -1;   // gamma must be between 0 and 1

    beta = (j*eimhf+k*gfmdi+l*dhmeg)/det;  // Cramer's Rule

    if (beta<0 || beta>(1-gamma)) return -1; // beta must be between 0 and 1

    return t;  // returns the t, where the ray hits the triangle.  r = o + td = start + t*direction
}

Hit nearestHitSphere(Scene scene, Ray ray) {
    double t_min = MAXFLOAT;
    Hit hit;
    hit.doesHit = false;
    hit.hitObjId = 9;
    int closestSphereIndex = -1, sphereIndex = 0, sphereCount = scene.spheres.size();
    Sphere currSphere;
    double t;
    if(!scene.spheres.empty()) {
        for(sphereIndex=0; sphereIndex<sphereCount; sphereIndex++) {
            currSphere = scene.spheres[sphereIndex];
            t = intersectSphere(ray, currSphere, scene);
            if((t >= 0) && (t < t_min)) {
                t_min = t;
                closestSphereIndex = sphereIndex; // curr sphere index
            }
        }
        if(closestSphereIndex != -1) {
            Point3d n;
            hit.doesHit = true;
            hit.ray = ray;
            hit.materialID = scene.spheres[closestSphereIndex].material_id;
            hit.intersectionPoint = add(ray.start, scale(ray.direction, t_min));
            n = sub(hit.intersectionPoint, scene.vertex_data[scene.spheres[closestSphereIndex].center_vertex_id-1]);
            n = normalizeVector(n);
            hit.normal = n;
            hit.ray_t = t_min;
            hit.hitObjId = closestSphereIndex * 10;
        }
    }
    return hit;
}

Ray computeRay(Camera camera, int x, int y) { //theres a problem obviously
    double su_d, sv_d, left, right, bottom, top, pixelHeight, pixelWidth, halfWidth, halfHeight;
    left = camera.near_plane.x;
    right = camera.near_plane.y;
    bottom = camera.near_plane.z;
    top = camera.near_plane.w;
    pixelHeight = abs(top - bottom) / camera.image_height;
    pixelWidth = abs(right - left) / camera.image_width;
    halfHeight = pixelHeight * 0.5;
    halfWidth = pixelWidth * 0.5;
    Point3d q, m, gaze = camera.gaze, u, v, w;
    gaze = normalizeVector(gaze);
    v = camera.up;
    v = normalizeVector(v);
    w = scale(gaze, -1);
    w = normalizeVector(w);
    u = crossProduct(gaze, v); // v, w
    m.x = camera.position.x + (gaze.x * camera.near_distance);
    m.y = camera.position.y + (gaze.y * camera.near_distance);
    m.z = camera.position.z + (gaze.z * camera.near_distance);
    q.x = m.x + (u.x*left) + (v.x*top);
    q.y = m.y + (u.y*left) + (v.y*top);
    q.z = m.z + (u.z*left) + (v.z*top);
    Ray ray;
    ray.start = camera.position;
    Point3d su, sv, currPixel;
    su_d = x * pixelWidth + halfWidth; // coordinate of the point ray intersects with the plane
    su = scale(u, su_d);
    sv_d = y * pixelHeight + halfHeight;
    sv = scale(v, sv_d);
    currPixel = add(q, sub(su, sv));
    ray.direction = normalizeVector(sub(currPixel, ray.start));
    return ray;
}

Point3d calculateIrradiance(double distance, Point3d intensity) {
    Point3d i;
    distance = pow(distance, 2);
    i.x = intensity.x / distance;
    i.y = intensity.y / distance;
    i.z = intensity.z / distance;
    return i;
}

Point3d calculateDiffuse(PointLight pl, Scene scene, Hit hit) { // without irradiance!!!
    Point3d diffuse, sphereToLightRay, irradiance;
    double dot = 0, distance = 0;
    distance = distanceBetween(pl.position, hit.intersectionPoint);
    if(abs(distance - 0.00000001)) irradiance = calculateIrradiance(distance, pl.intensity);

    if(hit.doesHit) {
        sphereToLightRay = sub(pl.position, hit.intersectionPoint);
        sphereToLightRay = normalizeVector(sphereToLightRay);
        dot = dotProduct(sphereToLightRay, hit.normal);
    }

    if(dot < 0) dot = 0;
    diffuse.x = scene.materials[hit.materialID - 1].diffuse.x * dot * irradiance.x;
    diffuse.y = scene.materials[hit.materialID - 1].diffuse.y * dot * irradiance.y;
    diffuse.z = scene.materials[hit.materialID - 1].diffuse.z * dot * irradiance.z;
    return diffuse;
}

Point3d calculateAmbient(Scene scene, Hit hit) {
    Point3d result;
    result.x = scene.materials[hit.materialID -1 ].ambient.x * scene.ambient_light.x;
    result.y = scene.materials[hit.materialID -1 ].ambient.y * scene.ambient_light.y;
    result.z = scene.materials[hit.materialID -1 ].ambient.z * scene.ambient_light.z;
    return result;
}

Point3d calculateSpecular(Material material, Hit hit, PointLight pl) {
    double distance = distanceBetween(hit.intersectionPoint, pl.position), dot = 0;
    Point3d specular, irradiance;
    if(distance > 0.0000001) irradiance = calculateIrradiance(distance, pl.intensity);
    Point3d wi, wo, h;
    wi = normalizeVector(sub(pl.position, hit.intersectionPoint));
    h = normalizeVector(sub(wi, hit.ray.direction));
    dot = dotProduct(hit.normal, h);
    if(dot < 0) dot = 0;
    double dotToPhong = pow(dot, material.phong_exponent);
    specular.x = material.specular.x * dotToPhong * irradiance.x;
    specular.y = material.specular.y * dotToPhong * irradiance.y;
    specular.z = material.specular.z * dotToPhong * irradiance.z;
    return specular;
}

bool doesIntersectPlane (Point3d normal, Ray ray)  { // does plane and ray intersect?
    return !dotProduct(ray.direction, normal); // if n.u != 0 they intersect
}

Hit closestHitTriangle(Scene scene, Ray ray) {
    double min_t = MAXFLOAT;
    int closestTriangleIndex = 0;
    Point3d normal;
    Face face;
    Hit hit;
    hit.doesHit = false;
    hit.hitObjId = 9;
    Triangle triangle;
    double t;

    if (!scene.triangles.empty()) {
        for (int i = 0; i < scene.triangles.size(); i++) {
            triangle = scene.triangles[i];
            t = intersectTriangle(ray, triangle, scene);
            if (t != -1 && t < min_t && t>=1) {
                min_t = t;
                closestTriangleIndex = i;
            }
        }
        if (closestTriangleIndex != -1) {
            triangle = scene.triangles[closestTriangleIndex];
            face = triangle.indices;
            normal = normalizeVector(computeTriangleNormal(scene.vertex_data[face.v0_id], scene.vertex_data[face.v1_id],scene.vertex_data[face.v2_id]));
            hit.doesHit = true;
            hit.materialID = triangle.material_id;
            hit.normal = normal;
            hit.ray = ray;
            hit.ray_t = min_t;
            hit.intersectionPoint = add(ray.start, scale(normalizeVector(ray.direction),hit.ray_t));
            hit.hitObjId = closestTriangleIndex * 10 + 1;
        }
    }
    else hit.doesHit = false;
    return hit;
}

// operation overload two faces are equal

bool equalFaces(Face face1, Face face2) {
    return face1.v0_id == face2.v0_id && face1.v1_id == face2.v1_id && face1.v2_id == face2.v2_id;
}

Hit nearestHitFace(Scene scene, Ray ray) {
    double t_min = MAXFLOAT;
    Hit hit;
    hit.doesHit = false;
    hit.hitObjId = 9;
    Index closestFaceIndex;
    closestFaceIndex.i = -1; closestFaceIndex.j = 0;
    Mesh mesh;
    Face face;
    Triangle triangle;
    double t;
    if(!scene.meshes.empty()) {
        for(int i =0; i< scene.meshes.size(); i++) {
            mesh = scene.meshes[i];
            for (int j=0; j < mesh.faces.size(); j++) {
                face = mesh.faces[j];
                triangle.indices = face;
                triangle.material_id = mesh.material_id;
                t = intersectTriangle(ray, triangle, scene);
                if ((t!= -1) && (t >= 1) && (t < t_min)) {
                    t_min = t;
                    closestFaceIndex.i = i;
                    closestFaceIndex.j = j;
                }
            }
        }
        if(closestFaceIndex.i != -1) {
            Point3d n;
            face = scene.meshes[closestFaceIndex.i].faces[closestFaceIndex.j];
            //     printf("%d %d %d ", scene.vertex_data[face.v0_id -1] ,scene.vertex_data[face.v1_id -1],scene.vertex_data[face.v2_id -1 ]);
            hit.doesHit = true;
            hit.ray = ray;
            hit.materialID = scene.meshes[closestFaceIndex.i].material_id;
            hit.intersectionPoint = add(ray.start, scale(ray.direction, t_min));
            n = computeTriangleNormal(scene.vertex_data[face.v0_id - 1],scene.vertex_data[face.v1_id -1 ],scene.vertex_data[face.v2_id -1] );
            n = normalizeVector(n);
            hit.normal = n;
            hit.ray_t = t_min;
            hit.hitObjId = (closestFaceIndex.i * 10 + closestFaceIndex.j) * 10 + 2;
        }
    }
    return hit;
}

Hit nearestHitObject (Scene scene, Ray ray) {
// hit nearest object, it maybe a face or a sphere or something it maybe anyth
    Hit hit;
    hit.doesHit = false;
    hit.hitObjId = 9;
    double closest_sphere, closest_face, closest_triangle;
    if (nearestHitSphere(scene,ray).doesHit ) closest_sphere = nearestHitSphere(scene,ray).ray_t; else closest_sphere = 987654;
    if (nearestHitFace(scene,ray).doesHit)  closest_face = nearestHitFace(scene, ray).ray_t ; else closest_face = 987654;
    if (closestHitTriangle(scene,ray).doesHit) closest_triangle = closestHitTriangle(scene,ray).ray_t; else closest_triangle = 987654;
    if ( closest_sphere < closest_face  && closest_sphere < closest_triangle )
        hit = nearestHitSphere(scene,ray);
    else if (closest_triangle < closest_sphere && closest_triangle < closest_face)
        hit = closestHitTriangle(scene, ray);
    else if (closest_face < closest_triangle && closest_face < closest_sphere) hit = nearestHitFace(scene,ray);
    return hit;
}

Point3d findPixelColor(Scene scene, Hit hit, Ray ray, int recursionDepth) {

    int lightnum = scene.point_lights.size();
    Point3d pixelResult = {0,0,0};
    double pixel_x = 0, pixel_y = 0, pixel_z = 0;

    if(hit.doesHit) {
        Point3d ambient = calculateAmbient(scene, hit);
        pixel_x = ambient.x; //direk pixelResult.x falan yapmama sebebi recursiondır.
        pixel_y = ambient.y;
        pixel_z = ambient.z;

        for (int curr = 0; curr < lightnum; curr++) {
            bool isShadow = false;
            PointLight currentLight = scene.point_lights[curr];
            double lightCamDistance = distanceBetween(currentLight.position, ray.start);
            Point3d wi = sub(currentLight.position, hit.intersectionPoint); //slayttaki wi
            wi = normalizeVector(wi);
            //Point3d wi_epsilon = scale(wi, scene.shadow_ray_epsilon);
            // todo shadow rayin başlangıcını epsilon kadar kaydırmadım
            Ray shadowRay = {hit.intersectionPoint, wi};
            double currentPixel_t = distanceBetween(hit.intersectionPoint, currentLight.position);
            Hit doesShadowHit = nearestHitObject(scene, shadowRay); //shadowRay in ilk hit i

            float tLight = sub(currentLight.position, shadowRay.start).x / shadowRay.direction.x;


            // todo: epsilon farkından cismin hemen dibindeki shadow oluşmamakta

            if (doesShadowHit.doesHit && doesShadowHit.ray_t >= 0 && doesShadowHit.hitObjId != hit.hitObjId && tLight > doesShadowHit.ray_t ) isShadow = true;

            if (!isShadow || (lightCamDistance < scene.shadow_ray_epsilon)) {
                Point3d diffuse = calculateDiffuse(currentLight, scene, hit);
                Point3d specular = calculateSpecular(scene.materials[hit.materialID - 1], hit, currentLight);
                pixel_x += diffuse.x + specular.x;
                pixel_y += diffuse.y + specular.y;
                pixel_z += diffuse.z + specular.z;
            }
        }

        // calculate mirrorness
        Material material = scene.materials[hit.materialID - 1];
        bool isMirror = (material.mirror.x > 0 || material.mirror.y > 0 || material.mirror.z > 0);
        Point3d reflection = {0, 0, 0};
        if (isMirror && recursionDepth > 0) {
            double cosTheta = -2 * (dotProduct(hit.normal, ray.direction));
            Point3d wr = scale(hit.normal, cosTheta);
            wr = add(wr, ray.direction);
            wr = normalizeVector(wr);
            Point3d wr_movedEpsilon = scale(wr, scene.shadow_ray_epsilon);
            Ray reflectionRay = {add(hit.intersectionPoint, wr_movedEpsilon), wr};
            Hit reflectionHit = nearestHitObject(scene, reflectionRay);
            if ((reflectionHit.doesHit)) {
                if ((reflectionHit.hitObjId != hit.hitObjId))
                    reflection = findPixelColor(scene, reflectionHit, reflectionRay, (recursionDepth - 1));
            }

            pixel_x += reflection.x * material.mirror.x;
            pixel_y += reflection.y * material.mirror.y;
            pixel_z += reflection.z * material.mirror.z;

        }
    }

    else {
        pixel_x = scene.background_color.x;
        pixel_y = scene.background_color.y;
        pixel_z = scene.background_color.z;
    }

    pixelResult.x = pixel_x;
    pixelResult.y = pixel_y;
    pixelResult.z = pixel_z;


    return pixelResult;

}

int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    Scene scene;

    scene.loadFromXml(argv[1]);

    int camSize = scene.cameras.size();

    for(int currentCam = 0; currentCam<camSize; currentCam++) {
        Camera currentCamera = scene.cameras[currentCam];
        std::string out = currentCamera.image_name; // trying for one element example, will be in a for loop l8r
        char* output = &out[0]; // string to char*, obviously.

        int imageWidth = currentCamera.image_width;
        int imageHeight = currentCamera.image_height;
        unsigned char* image = new unsigned char [imageWidth * imageHeight * 3];
        Ray ray;
        Hit hit;
        int i = 0;
        for (int y = 0; y < imageHeight; y++)
        {
            for (int x = 0; x < imageWidth; x++)
            {
                ray = computeRay(currentCamera, x,y); // y, x
                hit = nearestHitObject(scene,ray);
                Point3d pixelColor = findPixelColor(scene, hit, ray, scene.max_recursion_depth);
                RGB rgb = correctRGBValue(pixelColor);
                image[i] = rgb.r;
                image[i+1] = rgb.g;
                image[i+2] = rgb.b;

                i+=3;

            }
        }
        write_ppm(output, image, imageWidth, imageHeight);
    }

}
