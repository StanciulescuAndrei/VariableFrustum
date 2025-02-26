#include "Renderable.h"

Renderable::Renderable(ShaderProgram * _program, glm::mat4 _modelMatrix, glm::vec3 _color){
    mesh.buildCube();
    mesh.sendToOpenGL(*_program);
    modelMatrix = _modelMatrix;
    color = _color;
    program = _program;
}

Renderable::~Renderable(){
    mesh.free();
}

void Renderable::render(){
    program->setUniform3f("color", color.r, color.g, color.b);
    mesh.render();
}