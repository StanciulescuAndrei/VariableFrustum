#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "TriangleMesh.h"
#include "ShaderProgram.h"

class Renderable{
public: 
    Renderable(ShaderProgram * _program, glm::mat4 _modelMatrix, glm::vec3 _color = glm::vec3(1.0f, 0.0f, 0.0f));
    ~Renderable();
    void render();
    glm::mat4 getTransform(){ return modelMatrix;};

private:
    TriangleMesh mesh;
    ShaderProgram * program;
    glm::mat4 modelMatrix;
    glm::vec3 color;
};

#endif