#include <GL/glew.h> // glew must be included before the main gl libs
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "Sparkler.h"
#include "Camera.h"

//--Evil Global variables
//Just for this example!
SDL_Window *window;
SDL_GLContext gl_context;
int windowWidth = 1000, windowHeight = 1000;// Window size
float camY = 150.0f, camX = 0.0f, camZ = 0.01f;
float drawDistance = 1500.0f;
Sparkler* sparkler;
Camera* camera = new Camera(glm::vec3(0.0f, 20.0f, 3.0f), glm::vec3(0, -20, -3), glm::vec3(0, 1, 0), 2, .2);

//transform matrices
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojections

// SDL related functions
void createWindow(int width, int height);
void SDLMainLoop();
void update();
void render();

// Resource Management
void cleanup();
bool initialize(bool errorChecks);

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

void injectInput (bool & running) 
{
    SDL_Event e;
    float x, y;
    /* go through all available events */
    while (SDL_PollEvent(&e)) {
        /* we use a switch to determine the event type */
        switch (e.type) {
            /* mouse motion handler */
            case SDL_MOUSEMOTION:
                x = e.motion.xrel;
                y = e.motion.yrel;
                camera->rotate(x, y);
                break;
 
            /* mouse down handler */
            case SDL_MOUSEBUTTONDOWN:
                break;
 
            /* mouse up handler */
            case SDL_MOUSEBUTTONUP:
                break;
 
            /* key down */
            case SDL_KEYDOWN:
                // Get the type of key that was pressed
                switch( e.key.keysym.sym ){
                    case SDLK_UP:
                        break;
                    case SDLK_DOWN:
                        break;
                    case SDLK_LEFT:
                        break;
                    case SDLK_RIGHT:
                        break;
                    case SDLK_1:
                        break;
                    case SDLK_2:
                        break;
                    case SDLK_SPACE:
                        camera->moveUp();
                        break;
                    case SDLK_a:
                        camera->moveLeft();
                        break;
                    case SDLK_d:
                        camera->moveRight();
                        break;
                    case SDLK_w:
                        camera->moveForward();
                        break;
                    case SDLK_s:
                        camera->moveBackward();
                        break;
                    case SDLK_q:
                        camera->moveUp();
                        break;
                    case SDLK_e:
                        camera->moveDown();
                        break;
                    case SDLK_ESCAPE:
                        cleanup();
                        exit(0);
                        break;
                    default:
                        break;
                }
                break;
 
            /* key up */
            case SDL_KEYUP:
                break;
 
            /* WM quit event occured */
            case SDL_QUIT:
                cleanup();
                running = false;
                break;

             case SDL_WINDOWEVENT:
                switch(e.window.event){
                    case SDL_WINDOWEVENT_RESIZED:
                    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                                   float(windowWidth)/float(windowHeight), //Aspect Ratio, so Circles stay Circular
                                                   0.01f, //Distance to the near plane, normally a small value like this
                                                   drawDistance); //Distance to the far plane,       
                    glViewport(0,0,windowWidth,windowHeight);            
                    break;
                }
                break;
        }
    }
}

int main(int argc, char **argv) 
{
	// Intitialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) 
	  {
        std::cerr << "Cannot Initialize SDL" << std::endl;
        exit(1);
    }

    createWindow(windowWidth, windowHeight);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Now that the window is created the GL context is fully set up
    // Because of that we can now initialize GLEW to prepare work with shaders
    GLenum status = glewInit();
    if( status != GLEW_OK)
    {
        std::cerr << "[F] GLEW NOT INITIALIZED: ";
        std::cerr << glewGetErrorString(status) << std::endl;
        return -1;
    }

    // Initialize all of our resources(shaders, geometry)
    bool init = initialize(false);
    if(init)
    {
        SDLMainLoop();
    }
}

void createWindow(int width, int height)
{
	// Set double buffering to on
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create the window and set the title
    window = SDL_CreateWindow("Sparkler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP |SDL_WINDOW_RESIZABLE);

    if(!window) {
        std::cerr << "Unable to create GL Window" << std::endl;
        exit(0);
    }

	// Set the newly created window as the current window
    gl_context = SDL_GL_CreateContext(window);

	// Synchronize screen updates with monitor vertical retrace
    SDL_GL_SetSwapInterval(1);
}

void SDLMainLoop()
{
	// While program is running check for events and render
    bool running = true;
    t1 = std::chrono::high_resolution_clock::now();
    while(running) {
    injectInput(running);
		update();
		render();
    }
}

void update(){
    camera->update();
}

void render(){
    // --Render the scene
    // clear the screen
    glClearColor(0.6, 0.6, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sparkler->render(projection, camera->getView(), camera->getPos(), getDT() * 1000);

    // swap the render buffers
    SDL_GL_SwapWindow(window);
                           
}

bool initialize(bool errorChecks)
{
    // create sparkler
    sparkler = new Sparkler();
    if(!sparkler->init(glm::vec3(0,0,0)))
        exit(1);

    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    view = glm::lookAt( glm::vec3(camX, camY, camZ), //Eye Position
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(windowWidth)/float(windowHeight), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   drawDistance); //Distance to the far plane, 

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //and its done
    return true;
}

void cleanup()
{
    delete sparkler;
	// Clean up SDL
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//returns the time delta
float getDT()
{
    float ret;
    t2 = std::chrono::high_resolution_clock::now();
    ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
    t1 = std::chrono::high_resolution_clock::now();
    return ret;
}