#include "Graphics.hpp"

#include <iostream>

#include <ogrsf_frmts.h>
#include <ogr_spatialref.h>
#include <ogr_srs_api.h>

#include "gdal_priv.h"
#include "cpl_conv.h"

#include "Engine.hpp"
#include "Camera.hpp"
#include "EntityManager.hpp"
#include "TerrainBorder.hpp"

using namespace Vancom;

Graphics::Graphics(Engine *engine) : engine(engine){

    // initialize light angle for direction lights
    lightAngle = -0.5f;
    isRaining = true;
    
    // call to create the light vector
    increaseLightAngle();

	camera = new Camera(engine);
}

Graphics::~Graphics(){

	if(camera)
		delete camera;
}

void Graphics::init(){

	// define default window width and height
	int w = 1800, h = 1000;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// create SDL window
    window = SDL_CreateWindow("Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if(!window){
    	std::cerr << "Failed to create SDL window" << std::endl;
    	exit(1);
    }

    gl_context = SDL_GL_CreateContext(window);

    // set window to update with vertical retrace
    SDL_GL_SetSwapInterval(1);

    //initialize glew
    glewExperimental = GL_TRUE;
    GLenum status = glewInit();
    if(status != GLEW_OK) {
        std::cerr << "Unable to initialize glew" << std::endl;
        exit(1);
    }

    // setup gdal
    GDALAllRegister();
    OGRRegisterAll();

    // set clear color and culling
    glClearColor(0.0, 0.0, 0.5, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    updateView();
    windowResized();

    // initialize programs
    if(!defaultProgram.init())
        std::cout << "defaultProgram failed to init" << std::endl;

    if(!selectionProgram.init())
        std::cout << "selectionProgram failed to init" << std::endl;
    selectionTexture.init(1800, 1000);
}

void Graphics::tick(float dt){
}

void Graphics::render(){

	// update the camera and view
	updateCamera();
	updateView();

    // render selectable entities
    selectionTexture.enableWriting();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    selectionProgram.enable();

    for(Entity* entity : engine->entityManager->defaultEntities){
        selectionProgram.setObjectIndex(entity->id);
        selectionProgram.setMVP(projection * view * entity->getModel());
        entity->render();
    }

    selectionTexture.disableWriting();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // check to see if object is in middle of screen
    SelectionTexture::PixelInfo pixel = selectionTexture.readPixel(900, 500);
    if((unsigned int) pixel.objectId != 0)
        std::cout << (unsigned int) pixel.objectId << std::endl;

    // render entities using default
    defaultProgram.enable();
    defaultProgram.setLightDirection(getLightDirection());
    defaultProgram.setSpotLightPosition(camera->getPos());
    defaultProgram.setSpotLightDirection(camera->getCameraDirection());
    defaultProgram.setCameraPosition(camera->getPos());
    defaultProgram.setSpecularFlag(isRaining);
    for(Entity* entity : engine->entityManager->defaultEntities){
        defaultProgram.setMVP(projection * view * entity->getModel());
        defaultProgram.setModelPos(entity->getModel());
        defaultProgram.setSpecularIntensity(entity->specularIntensity);
        defaultProgram.setSpecularPower(entity->specularPower);
        entity->render();
    }

    // render terrain border
    engine->entityManager->border->render(projection, view);

	SDL_GL_SwapWindow(window);
}

void Graphics::stop(){

	// clean up SDL
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
}

void Graphics::updateView(){

	view = camera->getView();
}

void Graphics::updateCamera(){

	camera->update();
}

void Graphics::windowResized(){

	// reset window and projection matrix
	int w, h;
	getWindowSize(w, h);
	glViewport(0, 0, w, h);
	projection = glm::perspective(45.0f, float(w) / float(h), 0.01f, 100000.0f);
}

void Graphics::getWindowSize(int &w, int &h) const{

	SDL_GetWindowSize(window, &w, &h);
}

void Graphics::setClearColor(glm::vec3 color){

	glClearColor(color.x, color.y, color.z, 1);
}

void Graphics::increaseLightAngle(){


    lightAngle -= 0.01;

    float y = -cos(lightAngle + 1.57);
    float x = -sin(lightAngle + 1.57);

    lightDir[1] = y;
    lightDir[0] = x;
    
}

void Graphics::decreaseLightAngle(){

    lightAngle += 0.01;

    float y = -cos(lightAngle + 1.57);
    float x = -sin(lightAngle + 1.57);

    lightDir[1] = y;
    lightDir[0] = x;
    
}
glm::vec3 Graphics::getLightDirection() const{
    return lightDir;
}
