#include <GL/glew.h> // glew must be included before the main gl libs
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "ShaderLoader.h"
#include "Terrain.h"

//--Data types
//This object will define the attributes of a vertex(position, color, etc...)
enum ShaderVariable {Attribute, Uniform};

//--Evil Global variables
//Just for this example!
SDL_Window *window;
SDL_GLContext gl_context;
bool running = true;
int windowWidth = 1000, windowHeight = 1000;// Window size
float camY = 50.0f, camX = 0.0f, camZ = -50.0f;
float drawDistance = 500.0f;
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint loc_scalar;
GLint loc_sampler;
Terrain *terrain;
Texture *colorMap;

// vertex and fragment shaders
ShaderLoader vertexShader(GL_VERTEX_SHADER);
ShaderLoader fragmentShader(GL_FRAGMENT_SHADER);

//transform matrices
glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojections

// SDL related functions
void createWindow(int width, int height);
void SDLMainLoop(float scaleFactor);
void pollEvents();
void update();
void render(float scaleFactor);

// Resource Management
void cleanup();
bool initialize(bool errorChecks, const char* fileName, float terrainScale);
bool getShaderLoc(GLint var, const char* shaderName, ShaderVariable type, bool errorCheck);

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

int main(int argc, char **argv) 
{
    char* fileName;
    float scaleFactor = 1.0f, terrainScale = 1.0f;
    if(argc > 2){
        // read in scaling factor for terrain and file name
        fileName = argv[1];
        scaleFactor = atof(argv[2]);
        if(argc > 3){
            terrainScale = atof(argv[3]);
        }
    }
    else{
        std::cout << "You must pass in a file name for a first argument and a scale factor for the terrain height as a second argument." << std::endl;
        exit(1);
    }

	// Intitialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
        std::cerr << "Cannot Initialize SDL" << std::endl;
        exit(1);
    }

	// Create the program window. Name of program can be set within the createWindow function
    createWindow(windowWidth, windowHeight);

    // Now that the window is created the GL context is fully set up
    // Because of that we can now initialize GLEW to prepare work with shaders
    GLenum status = glewInit();
    if( status != GLEW_OK)
    {
        std::cerr << "[F] GLEW NOT INITIALIZED: ";
        std::cerr << glewGetErrorString(status) << std::endl;
        return -1;
    }

    // setup gdal
    GDALAllRegister();

    // Initialize all of our resources(shaders, geometry)
    bool init = initialize(false, fileName, terrainScale);
    if(init)
    {
		t1 = std::chrono::high_resolution_clock::now();
        SDLMainLoop(scaleFactor);
    }
}

void createWindow(int width, int height)
{
	// Set double buffering to on
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create the window and set the title
    window = SDL_CreateWindow("GDAL Terrain Color Map", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_RESIZABLE);

    if(!window) {
        std::cerr << "Unable to create GL Window" << std::endl;
        exit(0);
    }

	// Set the newly created window as the current window
    gl_context = SDL_GL_CreateContext(window);

	// Synchronize screen updates with monitor vertical retrace
    SDL_GL_SetSwapInterval(1);
}

void SDLMainLoop(float scaleFactor)
{
	// While program is running check for events and render
    while(running) 
	{
    pollEvents();
		update();
		render(scaleFactor);
    }
}

void update(){

}

void render(float scaleFactor){
    // --Render the scene
    // clear the screen
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //premultiply the matrix for this example
    mvp = projection * view * model;

    // enable the shader program
    glUseProgram(program);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform1i(loc_sampler, 0);
    glUniform1f(loc_scalar, scaleFactor);

    terrain->render(loc_position);

    glUseProgram(0);

	// swap the render buffers
	SDL_GL_SwapWindow(window);
                           
}

void pollEvents()
{
    SDL_Event event;
	
	// Loop through all events that have occurred
    while(SDL_PollEvent(&event)) 
	{
        switch(event.type) 
		{
			// Check to see if a key was pressed down
            case SDL_KEYDOWN:
				// Get the type of key that was pressed
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
            			camX -= 2;
						view = glm::lookAt( glm::vec3(camX, camY, camZ), //Eye Position
						            glm::vec3(0.0, 0.0, 0.0), //Focus point
						            glm::vec3(0.0, 1.0, 0.0));
                        break;
                    case SDLK_RIGHT:
                        break;
                    case SDLK_UP:
                        break;
                    case SDLK_DOWN:
                        break;
                    case SDLK_ESCAPE:
                    	cleanup();
                    	exit(0);
                    	break;
                    default:
                        break;
                }
            break;
	        /* SDL_QUIT event (window close) */
	        case SDL_QUIT:
	    		cleanup();
                exit(0);
	            break;
            case SDL_WINDOWEVENT:
                switch(event.window.event){
                    case SDL_WINDOWEVENT_RESIZED:
                    int w, h;
                    SDL_GetWindowSize(window, &w, &h);
                    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                                   float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                                   0.01f, //Distance to the near plane, normally a small value like this
                                                   drawDistance); //Distance to the far plane,       
                    glViewport(0,0,w,h);                  
                    break;
                }
                break;
        }
    }
}

bool initialize(bool errorChecks, const char* fileName, float terrainScale)
{
    // Initialize basic geometry and shaders for this example
    // get terrain texture
    terrain = new Terrain();
    terrain->loadHeightMap(fileName, terrainScale);

    colorMap = new Texture(GL_TEXTURE_1D, "colorMap.png");
    colorMap->create();

    //--Geometry done

    //Shader Sources
	// Load vertex and fragment shaders
	if(!fragmentShader.loadShaderFromFile("../bin/shaders/fragShader.fs") || !vertexShader.loadShaderFromFile("../bin/shaders/vertShader.vs")){
		return false;
	}

    //Now we link the 2 shader objects into a program
    //This program is what is run on the GPU
    program = glCreateProgram();
    glAttachShader(program, vertexShader.getShader());
    glAttachShader(program, fragmentShader.getShader());
    glLinkProgram(program);

    //check if everything linked ok
	GLint shader_status;
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
        return false;
    }

    //Now we set the locations of the attributes and uniforms
    //this allows us to access them easily while rendering
    /*if(!getShaderLoc(loc_position, "v_position", Attribute, errorChecks)){
        return false;
    }

    if(!getShaderLoc(loc_mvpmat, "mvpMatrix", Uniform, errorChecks)){
        return false;
    }

    if(!getShaderLoc(loc_scalar, "verticalScalar", Uniform, errorChecks)){
        return false;
    }

    if(!getShaderLoc(loc_sampler, "gSampler", Uniform, errorChecks)){
        return false;
    }
    std::cout << loc_position << std::endl;*/

    loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
        return false;
    }

    loc_sampler = glGetUniformLocation(program,
                    const_cast<const char*>("gSampler"));
    if(loc_sampler == -1)
    {
        std::cerr << "[F] SAMPLER NOT FOUND" << std::endl;
        return false;
    }

    loc_scalar = glGetUniformLocation(program,
                    const_cast<const char*>("verticalScalar"));
    if(loc_scalar == -1)
    {
        std::cerr << "[F] SAMPLER NOT FOUND" << std::endl;
        return false;
    }

    loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] SAMPLER NOT FOUND" << std::endl;
        return false;
    }

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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //and its done
    return true;
}

bool getShaderLoc(GLint var, const char* shaderName, ShaderVariable type, bool errorCheck){
    if(type == Attribute){
        var = glGetAttribLocation(program, shaderName);
    }
    if(type == Uniform){
        var = glGetUniformLocation(program, shaderName);
    }

    if(var == -1 && errorCheck)
    {
        std::cerr << "[F] " << shaderName << " not found." << std::endl;
        return false;
    }
    else{
        return true;
    }
}

void cleanup()
{
    // Clean up, Clean up
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry);

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
