#ifndef SCENE_H
#define SCENE_H

#include "Renderable.h"
#include "FrustumTracker.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <vector>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

class Scene{
public:
    Scene(Scene &s) = delete;
    void operator=(const Scene &) = delete;

    static Scene *GetInstance();

    ~Scene();
    void render();
    void keyCallback(int key);
private:
    Scene();
    static Scene * scene;

    ShaderProgram shaderProgram;
    FrustumTracker frustumTracker;
    std::vector<Renderable *> renderList;

    GLuint cvTextureId;
};

#endif