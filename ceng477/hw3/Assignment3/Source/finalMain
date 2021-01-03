#include <iostream>
#include "parser.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
using namespace parser;

//////-------- Global Variables -------/////////

GLuint gpuVertexBuffer;
GLuint gpuNormalBuffer;
GLuint gpuIndexBuffer;
GLuint VertexArrayID;
std::vector <Vec3f > normals;
int numTriangles = 0;
int numVertices = 0;
int numIndices  = 0;

double lastTime;
int nbFrames;

// Sample usage for reading an XML scene file
parser::Scene scene;
static GLFWwindow* win = NULL;

static void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

}


parser::Vec3f multiplyScalar(parser::Vec3f v1, double num) {
    parser::Vec3f result;
    result.x = v1.x * num;
    result.y = v1.y * num;
    result.z = v1.z * num;
    return result;
}

parser::Vec3f addVec3f(parser::Vec3f v1, parser::Vec3f v2) {
    parser::Vec3f result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

void setLights() {
    int i=0;
    glEnable(GL_LIGHTING);
    GLfloat ambient[4] = {scene.ambient_light.x, scene.ambient_light.y, scene.ambient_light.z, 1.};
    int size = scene.point_lights.size();
    for(i=0; i<size; i++) {
        glEnable(GL_LIGHT0 + i);
        parser::PointLight currLight = scene.point_lights[i];
        GLfloat position[4] = {currLight.position.x, currLight.position.y, currLight.position.z, 1.};
        GLfloat intensity[4] = {currLight.intensity.x, currLight.intensity.y, currLight.intensity.z, 1.};
        glLightfv(GL_LIGHT0 + i, GL_POSITION, position);
        glLightfv(GL_LIGHT0 + i, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, intensity);
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, intensity);
    }
}

void setCamera() {
    parser::Vec3f cameraPos = scene.camera.position;
    parser::Vec3f cameraUp = scene.camera.up;
    parser::Vec3f cameraGaze = scene.camera.gaze;
    parser::Vec3f center = addVec3f(cameraPos, multiplyScalar(cameraGaze, scene.camera.near_distance));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(scene.camera.near_plane.x, scene.camera.near_plane.y, scene.camera.near_plane.z, scene.camera.near_plane.w,
              scene.camera.near_distance, scene.camera.far_distance);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    gluLookAt(0.1, 0.2, 0.3, 0.1, 0.2, 0.3, 0.2, 0.2, 0.2);
   gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, center.x, center.y, center.z, cameraUp.x, cameraUp.y, cameraUp.z);

}


Vec3f crossProduct(Vec3f a, Vec3f b) {
    Vec3f tmp;
    tmp.x = a.y*b.z-b.y*a.z;
    tmp.y = b.x*a.z-a.x*b.z;
    tmp.z = a.x*b.y-b.x*a.y;
    return tmp;
}


Vec3f sub(Vec3f a, Vec3f b) {
    Vec3f result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

Vec3f div3f(Vec3f a, int b) {
    Vec3f result;
    if (b!= 0){
        result.x = a.x / b;
        result.y = a.y / b;
        result.z = a.z / b;
        return result;
    }
    else return a;
}


Vec3f computeFaceNormal(Face face) {
    Vec3f a =  scene.vertex_data.at(face.v0_id -1 );
    Vec3f b =  scene.vertex_data.at(face.v1_id - 1 );
    Vec3f c =  scene.vertex_data.at(face.v2_id - 1);
    return crossProduct(sub(b,a), sub(c,a));

}


Vec3f normalizeVector(Vec3f param) {
    Vec3f result;
    double d = sqrt(pow(param.x, 2) + pow(param.y, 2) + pow(param.z, 2));
    if (d != 0) {
        result.x = param.x / d;
        result.y = param.y / d;
        result.z = param.z / d;
        return result;
    }
    else return param;
}


std::vector < Vec3f>  calculateNormals( )  {
    int num = scene.vertex_data.size();
    std::vector <Vec3f> normals(num);
    std::vector <int> count(num);

    for (int i = 0; i < scene.meshes.size(); i++) {
        Mesh mesh = scene.meshes.at(i);
        for (int j = 0; j < mesh.faces.size(); j++) {
            Face face = mesh.faces.at(j);
            Vec3f normal = computeFaceNormal(face);
            normals.at(face.v0_id - 1) = addVec3f(normal, normals.at(face.v0_id - 1));
            count.at(face.v0_id - 1) += 1;
            normals.at(face.v1_id - 1) = addVec3f(normal, normals.at(face.v1_id - 1));
            count.at(face.v1_id - 1) += 1;
            normals.at(face.v2_id - 1) = addVec3f(normal, normals.at(face.v2_id - 1));
            count.at(face.v2_id - 1) += 1;
        }
    }
    for (int i = 0; i < num; ++i) {
        normals.at(i) = normalizeVector(div3f(normals.at(i), count.at(i)));  // normalization opengl todo
    }
    return normals;

}


void draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER,gpuVertexBuffer);
    glVertexPointer(3,GL_FLOAT,0,0);
    glBindBuffer(GL_ARRAY_BUFFER,gpuNormalBuffer);
    glNormalPointer(GL_FLOAT,0,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,gpuIndexBuffer);
    

    int numOfMeshes = scene.meshes.size();
    int meshStep = 0;
    for(int i=0; i<numOfMeshes; i++) {
        Mesh currMesh = scene.meshes[i];
        int materialIndex = currMesh.material_id - 1;
        int numOfTransformations = currMesh.transformations.size();
        glPushMatrix();

        Material currMaterial = scene.materials[currMesh.material_id-1];
        GLfloat ambient[4] = {currMaterial.ambient.x, currMaterial.ambient.y, currMaterial.ambient.z, 1.};
        GLfloat diffuse[4] =  {currMaterial.diffuse.x, currMaterial.diffuse.y, currMaterial.diffuse.z, 1.};
        GLfloat specular[4] = {currMaterial.specular.x, currMaterial.specular.y, currMaterial.specular.z, 1.};
        GLfloat phong_exp[1] = {currMaterial.phong_exponent};

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, phong_exp);
        // transformationsss
        for(int j=numOfTransformations-1; j>=0; j--) { // apply transformations in reverse order
            Transformation t = currMesh.transformations[j];
            if(t.transformation_type == "Translation") {
                Vec3f tr = scene.translations[t.id - 1];
                glTranslatef(tr.x, tr.y, tr.z);
            }

            else if(t.transformation_type == "Rotation") {
                Vec4f rot = scene.rotations[t.id - 1];
                glRotatef(rot.x, rot.y, rot.z, rot.w);
            }

            else if(t.transformation_type == "Scaling") {
                Vec3f sc = scene.scalings[t.id - 1];
                glScalef(sc.x, sc.y, sc.z);
            }
        }

        if (currMesh.mesh_type == "Solid") {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        glDrawElements(GL_TRIANGLES, currMesh.faces.size() * 3, GL_UNSIGNED_INT,
                       (const void*)(meshStep * 3 * sizeof(GLuint)));
        meshStep += currMesh.faces.size();
        glPopMatrix();

    }


}

void initScene() {


    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    if(scene.culling_enabled == 1) {
        glEnable(GL_CULL_FACE);
    }
    if(scene.culling_face == 0) {
        glCullFace(GL_BACK);
    }
    else glCullFace(GL_FRONT);


    numVertices = scene.vertex_data.size();


    for (int i = 0; i < scene.meshes.size(); ++i) {
        numTriangles = numTriangles + scene.meshes.at(i).faces.size();
    }

    normals = calculateNormals();
    GLfloat vertexData [numVertices * 3] ;
    GLfloat normalData [numVertices * 3];
    GLint indexData [numTriangles * 3];

    for (int i = 0; i < numVertices; i++) {

        vertexData[ 3 * i] =  scene.vertex_data.at(i).x ;
        vertexData[ 3 * i+1] = scene.vertex_data.at(i).y;
        vertexData[ 3 * i+2] = scene.vertex_data.at(i).z;

    }

    for (int i = 0; i < numVertices ; i++) {
        normalData[3 * i] = normals[i].x;
        normalData[3 * i+1] = normals[i].y;
        normalData[3 * i+2] = normals[i].z;
    }


    for (int i = 0; i < scene.meshes.size(); i++ ) {
        Mesh mesh = scene.meshes.at(i);
        for (int j = 0; j < mesh.faces.size(); j++) {
            Face face = mesh.faces.at(j);
            indexData[numIndices] =   face.v0_id -1;
            indexData[numIndices +1] = face.v1_id -1;
            indexData[numIndices +2] = face.v2_id -1;
            numIndices = numIndices +3;
        }

    }

    glGenBuffers(1, &gpuVertexBuffer);
    glGenBuffers(1, &gpuNormalBuffer);
    glGenBuffers(1, &gpuIndexBuffer);


    glBindBuffer(GL_ARRAY_BUFFER, gpuVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), vertexData, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, gpuNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), normalData, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpuIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numTriangles * 3 * sizeof(GLuint), indexData, GL_STATIC_DRAW);


}


void showFPS(GLFWwindow *pWindow)
{
    // Measure speed
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    char ss[500] = {};
    nbFrames++;
    if ( delta >= 1.0 ){ // If last cout was more than 1 sec ago
        //cout << 1000.0/double(nbFrames) << endl;

        double fps = ((double)(nbFrames)) / delta;

        sprintf(ss,"CENG477 - HW3 [ %lf FPS ]",fps);

        glfwSetWindowTitle(pWindow, ss);

        nbFrames = 0;
        lastTime = currentTime;
    }
}


int main(int argc, char* argv[]) {
    scene.loadFromXml(argv[1]);

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);


    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);



    win = glfwCreateWindow(scene.camera.image_width, scene.camera.image_height, "CENG477 - HW3",  NULL, NULL);
    if (!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    glfwMakeContextCurrent(win);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(win, keyCallback);

    initScene(); // stuff about vertices normals & initializing
    setLights();
    setCamera(); //new
  
    lastTime = glfwGetTime();
    nbFrames = 0;

    while(!glfwWindowShouldClose(win)) {

        glfwWaitEvents();
        draw(); // sth about lights rotations etc.
        showFPS(win);
        glfwSwapBuffers(win);
    }


    glfwDestroyWindow(win);
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}
