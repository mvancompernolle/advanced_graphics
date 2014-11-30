// SOURCE : http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html

#include "SkyBox.hpp"

#include <iostream>
#include "Model.hpp"
#include "SkyBoxProgram.hpp"
#include "CubeMapTexture.hpp"
#include "Engine.hpp"
#include "Graphics.hpp"
#include "LightingManager.hpp"
#include "Camera.hpp"

using namespace Vancom;

SkyBox::SkyBox(const Engine* engine) : engine(engine){

    program = NULL;
    texture = NULL;
    box = NULL;
}

SkyBox::~SkyBox(){

    delete program;
    delete texture;
    delete box;
}

bool SkyBox::init(const std::string& directory,
          const std::string& posXFilename,
          const std::string& negXFilename,
          const std::string& posYFilename,
          const std::string& negYFilename,
          const std::string& posZFilename,
          const std::string& negZFilename){

    program = new SkyBoxProgram();

    if (!program->init()) {
        printf("Error initializing the skybox technique\n");
        return false;
    }

    program->enable();
    program->setTextureUnit(0);
    
    texture = new CubeMapTexture(directory,
                                       posXFilename,
                                       negXFilename,
                                       posYFilename,
                                       negYFilename,
                                       posZFilename,
                                       negZFilename);

    if (!texture->load()) {
        printf("Error loading skybox texture.\n");
        return false;
    }
        
    box = new Model();
    //box->scale(20);

    return box->init("../assets/models/sphere.obj"); 

}

void SkyBox::render(glm::mat4 projection, glm::mat4 view){

    program->enable();
    
    GLint oldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
    GLint oldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
    
    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 temp;
    glm::mat4 model = glm::translate(temp, engine->graphics->camera->getPos());
    program->setLightDir(engine->lightingManager->dirLight.direction);
    program->setMVP(projection * view * model);
    texture->bind(GL_TEXTURE0);
    box->render();
    
    glCullFace(oldCullFaceMode);        
    glDepthFunc(oldDepthFuncMode);
}
