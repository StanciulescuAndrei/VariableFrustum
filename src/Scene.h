#ifndef SCENE_H
#define SCENE_H

#include "Renderable.h"
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

class Scene{
public:
    Scene();
    ~Scene();
    void render();
private:
    ShaderProgram shaderProgram;
    std::vector<Renderable *> renderList;
};

#endif