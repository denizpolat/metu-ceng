
struct RGB {
    unsigned char r, g, b;
};

struct Ray {
    parser::Point3d start, direction;
    // char hitObject; // 0 ---> sphere, 1 --->triangle, 2---->face, 3---->pointlight

};

struct Hit {
    parser::Point3d intersectionPoint;
    parser::Point3d normal;
    bool doesHit;
    int materialID;
    Ray ray;
    double ray_t;
    unsigned int hitObjId; // least sig ::::: 2->face, 1->triangle, 0->sphere
};

struct Plane {
    double x, y, z, t;
};

struct justTriangle { // parserdaki triangle direkt xml den çektiği için onu kullanmak hatalara yol açabiliyo.
    parser::Point3d a, b, c;
};

struct Index {
    int i, j;
};


