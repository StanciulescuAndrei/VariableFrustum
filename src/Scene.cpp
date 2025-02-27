#include "Scene.h"
#include <iostream>

Scene* Scene::scene= nullptr;

Scene * Scene::GetInstance(){
    if(scene == nullptr){
        scene = new Scene();
    }
    return scene;
}

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

    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3( 0.0, 0.0, 0.0)), glm::vec3(1.0, 0.0, 0.0)));
    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(-0.2, 0.0, 0.0)), glm::vec3(0.0, 0.0, 1.0)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3( 3.0, -3.0, 1.0)), glm::vec3(1.0, 0.0, 0.3)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3( 3.0,  3.0, 1.0)), glm::vec3(1.0, 0.5, 1.0)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(-3.0, -3.0, 5.0)), glm::vec3(0.5, 0.0, 1.0)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(-3.0,  3.0, 5.0)), glm::vec3(0.5, 0.5, 1.0)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3( 3.0, -3.0, 5.0)), glm::vec3(1.0, 0.0, 0.5)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3( 3.0,  3.0, 5.0)), glm::vec3(0.0, 1.0, 1.0)));
}

Scene::~Scene(){
    for(Renderable * mesh : renderList){
        delete mesh;
    }
    shaderProgram.free();
    delete scene;
}

void Scene::render(){
    frustumTracker.refreshFrustum();

    glm::mat4 trackedFrustum = frustumTracker.getFrustum();
    glm::mat4 modelView = glm::lookAt(-frustumTracker.getEstimatedPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f));
    //glm::mat4 modelView = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f));


    shaderProgram.use();
	shaderProgram.setUniformMatrix4f("projection", trackedFrustum);

    for(Renderable * mesh : renderList){
        glm::mat4 localTransform =  modelView * mesh->getTransform();
        shaderProgram.setUniformMatrix4f("modelview", localTransform);
        mesh->render();
    }
}

void Scene::keyCallback(int key)
{
    if(key == GLFW_KEY_A){
        frustumTracker.movePosition(glm::vec3(-0.01f, 0.0f, 0.0f));
    }
    else if(key == GLFW_KEY_D){
        frustumTracker.movePosition(glm::vec3(0.01f, 0.0f, 0.0f));
    }
}
