#include "Renderable.h"

Renderable::Renderable(ShaderProgram * _program, glm::mat4 _modelMatrix, glm::vec3 _color, std::string modelPath /* = std::string("") */){
    if(modelPath.length() < 2){
        mesh.buildCube();
    }
    else{
        PLYReader ply_reader;
        ply_reader.readMesh(modelPath, mesh);
    }
    mesh.sendToOpenGL(*_program);
    modelMatrix = _modelMatrix;
    color = _color;
    program = _program;
}

Renderable::~Renderable(){
    mesh.free();
}

void Renderable::render(){
    program->setUniform4f("color", color.r, color.g, color.b, 1.0f);
    mesh.render();
}