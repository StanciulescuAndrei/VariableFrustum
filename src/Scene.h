#ifndef SCENE_H
#define SCENE_H

#include "ShaderProgram.h"
#include "TriangleMesh.h"
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Scene{
public:
    Scene();
    ~Scene();
    void render();
private:
    ShaderProgram shaderProgram;
    std::vector<TriangleMesh *> renderList;
};

#endif