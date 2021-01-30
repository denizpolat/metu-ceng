#include <vector>
#include "helper.h"
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"


static GLFWwindow* win = NULL;
int widthWindow = 1000, heightWindow = 1000;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idHeightTexture;
GLuint idMVPMatrix;
GLuint x;
GLuint depthMapFBO;

// Buffers
GLuint idVertexBuffer;
GLuint idIndexBuffer;

int textureWidth, textureHeight;
float heightFactor = 10.0;


glm::vec3 lightPos;

GLuint depthCubemap;
bool lightPosFlag = false;


int windowX = 0;
int windowY = 0;


struct Camera
{
    glm::vec3 position;
    glm::vec3 gaze;
    glm::vec3 up;
    float speed;
    glm::vec3 left;

};

Camera camera;

Camera initialCamera;

Camera initializeCamera(Camera camera) {
    glm::vec3 temp;
    temp.x = textureWidth/2;
    temp.y = textureWidth/10;
    temp.z = -textureWidth/4;
    camera.position = temp;
    camera.gaze = glm::vec3(0,0,1); // pdfte yzan değer
    camera.speed = 0;
    camera.up = glm::vec3(0,1,0); // uydurdum todo
    camera.left = glm::cross(camera.up, camera.gaze);
    return camera;
}


static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {

            case GLFW_KEY_R:
                heightFactor += 0.5;
                break;
            case GLFW_KEY_L:
                heightFactor -= 0.5;
                break;
            case GLFW_KEY_Q:
                // todo
                break;
            case GLFW_KEY_E:
                // todo
                break;

            case GLFW_KEY_A:
                //camera left
                camera.gaze = glm::normalize(rotate(camera.gaze, 0.05f, camera.up));
                camera.left = glm::normalize(rotate(camera.left, 0.05f, camera.up));
                break;
            case GLFW_KEY_D:
                //camera right
                camera.gaze = glm::normalize(rotate(camera.gaze, -0.05f, camera.up));
                camera.left = glm::normalize(rotate(camera.left, -0.05f, camera.up));
                break;
            case GLFW_KEY_W:
                //camera up 0.05
                camera.gaze = glm::normalize(rotate(camera.gaze, 0.05f, camera.left));
                camera.up = glm::normalize(rotate(camera.up, 0.05f, camera.left));

                break;
            case GLFW_KEY_S:
                camera.gaze = glm::normalize(rotate(camera.gaze, -0.05f, camera.left));
                camera.up = glm::normalize(rotate(camera.up, -0.05f, camera.left));

                break;
            case GLFW_KEY_Y:
                camera.speed += 0.01;
                break;
            case GLFW_KEY_H:
                camera.speed -= 0.01;
                break;
            case GLFW_KEY_X:
                camera.speed = 0.;
                break;

            case GLFW_KEY_I:
                camera.speed = 0.;
                camera.gaze = glm::vec3(0., 0., 1.);
                camera.up = glm::vec3(0., 1., 0.);
                camera.left = glm::cross(camera.up, camera.gaze);
                camera.position = glm::vec3(textureWidth/2, textureWidth/10, -textureWidth/4);
                break;


            case GLFW_KEY_P:
                glfwGetWindowPos(window, &windowX, &windowY);
                glfwGetWindowSize(window, &windowX, &windowY);
                auto monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);

                glfwSetWindowMonitor(win, monitor, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
                //glViewport(0, 0, vidmode->width, vidmode->height);
                break;

        }
    }
}


void drawMap()
{

    glGenVertexArrays(1, &depthMapFBO);
    glBindVertexArray(depthMapFBO);

    glEnableVertexAttribArray(x);

    glGenBuffers(1, &idVertexBuffer);
    glGenBuffers(1, &idIndexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndexBuffer);


    int w = textureWidth;
    int h = textureHeight;

    GLfloat vertices [ ( textureWidth + 1) * (textureHeight + 1) * 3];
    std::vector<GLuint> *indices = new std::vector<GLuint> ();

    int iter = 0;
    for (int i = 0; i < h + 1; i ++) {
        for (int j = 0; j < w + 1; j++) {
            vertices[iter] = j;
            vertices[iter +1] = 0;
            vertices[iter +2] = i;
            iter = iter + 3;
        }
    }


    for (int i = 0; i < h ; i++) {
        for (int j= 0; j < w ; j++) {
            // two triangles for a pixel
            // a __  b
            // |  \  |
            // d __  c
            // first triangle d a c - b c a
            indices->push_back(i * (w + 1)  + j); //d
            indices->push_back((i + 1) * (w + 1) + j); //a
            indices->push_back(i * (w + 1) + j + 1); //c

            //other triangle upper
            indices->push_back((i + 1) * ( 1 + w) + j + 1); // b
            indices->push_back(i * (w +1) + j + 1); // c
            indices->push_back((i + 1) * (1 +w) + j); //a


        }
    }



    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * (w + 1) * (h+1), &vertices, GL_STATIC_DRAW); // to check TODO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint) * w * h * 2, indices->data(),  GL_STATIC_DRAW);

    glVertexAttribPointer(x, 3, GL_FLOAT, GL_FALSE, 0, 0);

}


void locations() {

    // query uniform locations

    idHeightTexture = glGetUniformLocation(idProgramShader, "heights");
    idJpegTexture = glGetUniformLocation(idProgramShader, "rgbTexture");

    GLint heightFactorLoc = glGetUniformLocation(idProgramShader, "heightFactor");
    glUniform1f(heightFactorLoc, heightFactor);

    GLint widthTextureLoc = glGetUniformLocation(idProgramShader, "widthTexture");
    glUniform1i(widthTextureLoc, textureWidth);

    GLint heightTextureLoc = glGetUniformLocation(idProgramShader, "heightTexture");
    glUniform1i(heightTextureLoc, textureHeight);

    GLint cameraPositionLoc = glGetUniformLocation(idProgramShader, "cameraPosition");
    glUniform3fv(cameraPositionLoc, 1, &camera.position.x);

    glm::mat4 M_projection = glm::perspective(45.0, 1.0, 0.1, 1000.0);

    glm::mat4 M_view = glm::lookAt(camera.position, glm::vec3(camera.position + camera.gaze), camera.up);

    idMVPMatrix = glGetUniformLocation(idProgramShader, "MVP");
    glUniformMatrix4fv(idMVPMatrix, 1, GL_FALSE, &(M_projection * M_view)[0][0]);


}


void render() {

    glDrawElements(GL_TRIANGLES, 2 * textureWidth * textureHeight * 3, GL_UNSIGNED_INT, 0);
}

void janitor(){

    glClearColor(0, 0, 0, 1);
    glClearStencil(0);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

}


void update() {
    int width, height;
    glfwGetWindowSize(win, &width, &height);
    glViewport(0,0, width, height);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Please provide height and texture image files!\n");
        exit(-1);
    }

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    //  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This is required for remote
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This might be used for local

    win = glfwCreateWindow(widthWindow, heightWindow, "CENG477 - HW4", NULL, NULL);

    if (!win) {
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(win);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

        glfwTerminate();
        exit(-1);
    }

    glfwSetKeyCallback(win, keyCallback);

    glEnable(GL_DEPTH_TEST);

    std::string vert = "shader.vert";
    std::string frag = "shader.frag";
    initShaders(idProgramShader, vert, frag);

    glUseProgram(idProgramShader);
    initTexture(argv[1], argv[2], &textureWidth, &textureHeight);

    locations();

    drawMap();

    camera = initializeCamera(camera);

    locations();


    while(!glfwWindowShouldClose(win)) {

        glfwSwapBuffers(win);
        janitor();
        locations();
        camera.position += camera.gaze * camera.speed;
        update();
        render();
        glfwPollEvents();
    }

    glfwDestroyWindow(win);
    glfwTerminate();

    return 0;
}
