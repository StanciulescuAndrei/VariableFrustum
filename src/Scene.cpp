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

    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3( 0.0, 0.0, -0.1)), glm::vec3(1.0, 0.0, 0.0)));
    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(-0.2, 0.0, -0.1)), glm::vec3(0.0, 0.0, 1.0)));
    renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(0.2, 0.0, -0.1)), glm::vec3(0.0, 1.0, 0.0)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3( 3.0, -3.0, 1.0)), glm::vec3(1.0, 0.0, 0.3)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3( 3.0,  3.0, 1.0)), glm::vec3(1.0, 0.5, 1.0)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(-3.0, -3.0, 5.0)), glm::vec3(0.5, 0.0, 1.0)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3(-3.0,  3.0, 5.0)), glm::vec3(0.5, 0.5, 1.0)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3( 3.0, -3.0, 5.0)), glm::vec3(1.0, 0.0, 0.5)));
    // renderList.push_back(new Renderable(&shaderProgram, glm::translate(glm::mat4(1.0), glm::vec3( 3.0,  3.0, 5.0)), glm::vec3(0.0, 1.0, 1.0)));

    // OpenCV feed
    glGenTextures(1, &cvTextureId);
    glBindTexture(GL_TEXTURE_2D, cvTextureId);

    const glm::ivec2 & frame_resolution = frustumTracker.getFrameResolution();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_resolution.x, frame_resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Scene::drawOverlay()
{
    glUseProgram(0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cvTextureId);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(-0.5f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -0.5f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
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
    cv::Mat frame = frustumTracker.getFrame();

    glBindTexture(GL_TEXTURE_2D, cvTextureId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_RGB, GL_UNSIGNED_BYTE, frame.data);

    glm::mat4 trackedFrustum = frustumTracker.getFrustum();
    // glm::mat4 modelView = glm::lookAt(-frustumTracker.getEstimatedPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f));
    glm::vec3 head_position = frustumTracker.getEstimatedPosition();
    glm::mat4 modelView = glm::lookAt(head_position, glm::vec3(head_position.x, head_position.y, 0.0f), glm::vec3(0.f, 1.f, 0.f));

    shaderProgram.use();
	shaderProgram.setUniformMatrix4f("projection", trackedFrustum);

    for(Renderable * mesh : renderList){
        glm::mat4 localTransform =  modelView * mesh->getTransform();
        shaderProgram.setUniformMatrix4f("modelview", localTransform);
        mesh->render();
    }

    drawOverlay();

}

void Scene::keyCallback(int key)
{
    // if(key == GLFW_KEY_A){
    //     frustumTracker.movePosition(glm::vec3(-0.01f, 0.0f, 0.0f));
    // }
    // else if(key == GLFW_KEY_D){
    //     frustumTracker.movePosition(glm::vec3(0.01f, 0.0f, 0.0f));
    // }
}
