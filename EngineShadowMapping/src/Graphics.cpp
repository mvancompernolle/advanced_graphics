#include "Graphics.hpp"

#include <iostream>

#include <ogrsf_frmts.h>
#include <ogr_spatialref.h>
#include <ogr_srs_api.h>

#include "gdal_priv.h"
#include "cpl_conv.h"

#include "Entity.hpp"
#include "Engine.hpp"
#include "Camera.hpp"
#include "EntityManager.hpp"

using namespace Vancom;

Graphics::Graphics(Engine *engine) : engine(engine){

    // initialize light angle for direction lights
    lightAngle = -1.0f;
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

///////////////////////////////////////////////////////////////////////////////////////////
// temp create cubes for shadow mapping
// initalize program
shadowProgram.init();

shadowProgram.enable();

// init the fbo
std::cout << w << " " << h << std::endl;
shadowFBO.init(w, h);

// create shadow vao
glGenVertexArrays(1, &shadowVao);
glBindVertexArray(shadowVao);

// add a cube
cube1 = new Cube(glm::vec3(0, 10, 0));
cube1->init(1);
// add a second cube
cube2 = new Cube(glm::vec3(0, 0, 0));
cube2->init(100);

// set texture
tex = new Texture(GL_TEXTURE_2D, "../assets/terrain_dirt.jpg");

if(!tex->create()){
    std::cerr << "There was an error creating the terrain texture" << std::endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////
}

void Graphics::tick(float dt){
}

void Graphics::render(){

	// update the camera and view
	updateCamera();
	updateView();

/////////////////////////////////////////////////////////////
// temp render shadow map squares
shadowProgram.enable();
glBindVertexArray(shadowVao);

// shadow map pass
shadowFBO.bindForWriting();
glClear(GL_DEPTH_BUFFER_BIT);
//glm::mat4 model;
//model = glm::translate(model, glm::vec3(0, 220, 0));
//shadowProgram.setMVP(projection * glm::lookAt(glm::vec3(0, 220, 0), glm::vec3(0.01, 0, 0), glm::vec3(0, 1, 0)) * model);
cube1->render(projection, glm::lookAt(glm::vec3(0, 20, 0), glm::vec3(0.1, 0, 0), glm::vec3(-1, -1, 0)), shadowProgram);

glBindFramebuffer(GL_FRAMEBUFFER, 0);

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

shadowProgram.setTextureUnit(0);
//tex->bind(GL_TEXTURE0);
shadowFBO.bindForReading(GL_TEXTURE0);
//glm::mat4 camModel;
//camModel = glm::translate(camModel, camera->getPos());
//shadowProgram.setMVP(projection * view * camModel);
cube2->render(projection, view, shadowProgram);

SDL_GL_SwapWindow(window);
/////////////////////////////////////////////////////////////

    /*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(Entity *entity : engine->entityManager->entities)
        entity->render(projection, view);


	SDL_GL_SwapWindow(window);*/
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
