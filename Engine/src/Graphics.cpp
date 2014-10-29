#include "Graphics.hpp"
#include "Engine.hpp"
#include "Camera.hpp"

#include <iostream>

using namespace Vancom;

Graphics::Graphics(Engine *engine) : engine(engine){

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

    // set clear color and culling
    glClearColor(0.0, 0.0, 0.0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    updateView();
    windowResized();

    // initialize 
   	cube = new Cube();
}

void Graphics::tick(float dt){
}

void Graphics::render(){

	// update the camera and view
	updateCamera();
	updateView();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// tick all graphical entities
	cube->render(projection, view);

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