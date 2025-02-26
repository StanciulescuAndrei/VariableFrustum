#include "Scene.h"
#include <iostream>

Scene::Scene(){
    Shader vertexShader;
    vertexShader.initFromFile(VERTEX_SHADER, "../src/shaders/basic.vert");
    if(!vertexShader.isCompiled()){
        std::cout << "Vertex error: " << vertexShader.log()<<std::endl;
    }
    Shader fragmentShader;
    fragmentShader.initFromFile(FRAGMENT_SHADER, "../src/shaders/basic.frag");
    if(!fragmentShader.isCompiled()){
        std::cout << "Fragment error: " << fragmentShader.log()<<std::endl;
    }

    shaderProgram.init();
    shaderProgram.addShader(vertexShader);
    shaderProgram.addShader(fragmentShader);
    shaderProgram.link();
    if(!shaderProgram.isLinked()){
        std::cout << "Program error: " << shaderProgram.log() << std::endl;
    }

    fragmentShader.free();
    vertexShader.free();

    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(-3.0, -3.0, 0.0)), glm::vec3(1.0, 0.0, 0.0)));
    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(-3.0, 3.0, 0.0)), glm::vec3(0.0, 0.0, 1.0)));
    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(3.0, -3.0, 0.0)), glm::vec3(1.0, 0.0, 0.3)));
    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(3.0, 3.0, 0.0)), glm::vec3(1.0, 0.5, 1.0)));
    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(-3.0, -3.0, 5.0)), glm::vec3(0.5, 0.0, 1.0)));
    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(-3.0, 3.0, 5.0)), glm::vec3(0.5, 0.5, 1.0)));
    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(3.0, -3.0, 5.0)), glm::vec3(1.0, 0.0, 0.5)));
    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(3.0, 3.0, 5.0)), glm::vec3(0.0, 1.0, 1.0)));
}

Scene::~Scene(){
    for(Renderable * mesh : renderList){
        delete mesh;
    }
    shaderProgram.free();
}

void Scene::render(){
    glm::mat4 perspective = glm::perspective(60.f / 180.f * 3.1415f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.01f, 100.0f);
    glm::mat4 modelView = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f));
    // modelView = glm::translate(modelView, glm::vec3(0.0f, -10.0f, 0.0f));

    shaderProgram.use();
	shaderProgram.setUniformMatrix4f("projection", perspective);
    for(Renderable * mesh : renderList){
        glm::mat4 localTransform = mesh->getTransform() * modelView;
        shaderProgram.setUniformMatrix4f("modelview", localTransform);
        mesh->render();
    }
}