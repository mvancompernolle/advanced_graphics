#include <GL/glew.h> // glew must be included before the main gl libs
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "ShaderLoader.h"
#include "Water.h"

//--Data types
enum ShaderVariable {Attribute, Uniform};

struct BaseLight{
    glm::vec3 color;
    float ambientIntensity;
    float diffuseIntensity;

    BaseLight(glm::vec3 col, float ambIntensity, float diffIntensity){
        color = col;
        ambientIntensity = ambIntensity;
        diffuseIntensity = diffIntensity;
    }
};

struct DirectionalLight : public BaseLight{
    public: 
    glm::vec3 direction;

    DirectionalLight(glm::vec3 col, glm::vec3 dir, float diffIntensity, float ambIntensity) : BaseLight(col, ambIntensity, diffIntensity){
        direction = dir;
    }
};

struct PointLight : public BaseLight{

    public:
    glm::vec3 pos;

    struct{
        float constant, linear, exp;
    } Attenuation;


    PointLight(glm::vec3 col, glm::vec3 pos, float diffIntensity, float ambIntensity) : BaseLight(col, ambIntensity, diffIntensity){
        this->pos = pos;
    }
};

GLint loc_specularStrength, loc_matSpecularIntensity;
glm::vec3 ambientLightColor(1.0, 1.0, 1.0);
float ambientLightIntensity = 0.2;
GLint ambLightColorLoc, ambLightAmbientIntensityLoc; 
BaseLight ambLight(ambientLightColor, ambientLightIntensity, 1.0f);

glm::vec3 dirLightColor(1.0, 1.0, 1.0);
float dirLightDifIntensity = 0.4, dirLightAmbIntensity = .2;
GLint dirLightColorLoc, dirLightAmbientIntensityLoc, dirLightDirLoc, dirLightIntensityLoc;
DirectionalLight dirLight(dirLightColor, glm::vec3(0, -1, 0), dirLightDifIntensity, dirLightAmbIntensity);

GLint pointLightColorLoc, pointLightAmbientIntensityLoc, pointLightPosLoc, pointLightIntensityLoc, pointLightConstantLoc, pointLightLinearLoc, pointLightExpLoc;
glm::vec3 pointLightPos(0.0, 2.0, 10.0);
glm::vec3 pointLightColor(1.0, 1.0, 1.0);
float pointLightConstant = 1.0, pointLightLinear = 0.1, pointLightExp = 0.0;
float pointLightAmbIntensity = .1;
float pointLightDifIntensity = .8;
PointLight pointLight(pointLightColor, pointLightPos, pointLightDifIntensity, pointLightAmbIntensity);

//--Evil Global variables
//Just for this example!
SDL_Window *window;
SDL_GLContext gl_context;
int windowWidth = 1000, windowHeight = 1000;// Window size
float camY = 150.0f, camX = 0.0f, camZ = 0.01f;
float drawDistance = 1500.0f;
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry
Water* water;

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint loc_scalar;
GLint loc_sampler;

//transform matrices
glm::mat4 model;//obj->world each object should have its own model matrix
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
bool getShaderLoc(GLint var, const char* shaderName, ShaderVariable type, bool errorCheck);

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

void injectInput (bool & running) 
{
    SDL_Event e;
    /* go through all available events */
    while (SDL_PollEvent(&e)) {
        /* we use a switch to determine the event type */
        switch (e.type) {
            /* mouse motion handler */
            case SDL_MOUSEMOTION:
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
                    case SDLK_a:
                        camX -= 2;
                        view = glm::lookAt( glm::vec3(camX, camY, camZ), //Eye Position
                                    glm::vec3(0.0, 0.0, 0.0), //Focus point
                                    glm::vec3(0.0, 1.0, 0.0));
                        break;
                    case SDLK_d:
                        camX += 2;
                        view = glm::lookAt( glm::vec3(camX, camY, camZ), //Eye Position
                                    glm::vec3(0.0, 0.0, 0.0), //Focus point
                                    glm::vec3(0.0, 1.0, 0.0));
                        break;
                    case SDLK_w:
                        camZ -= 2;
                        view = glm::lookAt( glm::vec3(camX, camY, camZ), //Eye Position
                                    glm::vec3(0.0, 0.0, 0.0), //Focus point
                                    glm::vec3(0.0, 1.0, 0.0));
                        break;
                    case SDLK_s:
                        camZ += 2;
                        view = glm::lookAt( glm::vec3(camX, camY, camZ), //Eye Position
                                    glm::vec3(0.0, 0.0, 0.0), //Focus point
                                    glm::vec3(0.0, 1.0, 0.0));
                        break;
                    case SDLK_q:
                        camY += 2;
                        view = glm::lookAt( glm::vec3(camX, camY, camZ), //Eye Position
                                    glm::vec3(0.0, 0.0, 0.0), //Focus point
                                    glm::vec3(0.0, 1.0, 0.0));
                        break;
                    case SDLK_e:
                        camY -= 2;
                        view = glm::lookAt( glm::vec3(camX, camY, camZ), //Eye Position
                                    glm::vec3(0.0, 0.0, 0.0), //Focus point
                                    glm::vec3(0.0, 1.0, 0.0));
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
    //SDL_ShowCursor(SDL_DISABLE);

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
    window = SDL_CreateWindow("GDAL Terrain Color Map", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

}

void render(){
    // --Render the scene
    // clear the screen
    glClearColor(0.6, 0.6, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    // ambient light
    glUniform3f(ambLightColorLoc, ambLight.color[0], ambLight.color[1], ambLight.color[2]);
    glUniform1f(ambLightAmbientIntensityLoc, ambLight.ambientIntensity);    

    // directional light
    glUniform3f(dirLightColorLoc, dirLight.color[0], dirLight.color[1], dirLight.color[2]);
    glUniform1f(dirLightAmbientIntensityLoc, dirLight.ambientIntensity);    
    glUniform3f(dirLightDirLoc, dirLight.direction[0], dirLight.direction[1], dirLight.direction[2]);
    glUniform1f(dirLightIntensityLoc, dirLight.diffuseIntensity);   

    // point light
    glUniform3f(pointLightColorLoc, pointLight.color[0], pointLight.color[1], pointLight.color[2]);
    glUniform1f(pointLightAmbientIntensityLoc, pointLight.ambientIntensity);    
    glUniform3f(pointLightPosLoc, pointLight.pos[0], pointLight.pos[1], pointLight.pos[2]);
    glUniform1f(pointLightIntensityLoc, pointLight.diffuseIntensity);

    glUniform1f(pointLightConstantLoc, pointLightConstant);
    glUniform1f(pointLightLinearLoc, pointLightLinear);
    glUniform1f(pointLightExpLoc, pointLightExp);

    water->render(projection, view);

    // swap the render buffers
    SDL_GL_SwapWindow(window);
                           
}

bool initialize(bool errorChecks)
{
    //--Geometry done

    //Shader Sources
    // Load vertex and fragment shaders
    ShaderLoader shaderLoader;
    GLint frag, vert;
    frag = shaderLoader.loadShaderFromFile(GL_FRAGMENT_SHADER, "../bin/shaders/frag.fs");
    vert = shaderLoader.loadShaderFromFile(GL_VERTEX_SHADER, "../bin/shaders/vert.vs");

    // create colored program
    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    //check if everything linked ok
    GLint shader_status;
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
        return false;
    }

    // ambient light
    ambLightColorLoc = glGetUniformLocation(program, const_cast<const char*>("ambientLight.color"));
    if(ambLightColorLoc == -1)
    {
        std::cerr << "[F] AMB LIGHT COLOR NOT FOUND" << std::endl;
    }

    ambLightAmbientIntensityLoc = glGetUniformLocation(program, const_cast<const char*>("ambientLight.ambientIntensity"));
    if(ambLightColorLoc == -1)
    {
        std::cerr << "[F] AMB INTENSITY NOT FOUND" << std::endl;
    }

    // directional light
    dirLightColorLoc = glGetUniformLocation(program, const_cast<const char*>("directionalLight.base.color"));
    if(dirLightColorLoc == -1)
    {
        std::cerr << "[F] DIR LIGHT COLOR NOT FOUND" << std::endl;
    }

    dirLightAmbientIntensityLoc = glGetUniformLocation(program, const_cast<const char*>("directionalLight.base.ambientIntensity"));
    if(dirLightColorLoc == -1)
    {
        std::cerr << "[F] DIR AMB INTENSITY NOT FOUND" << std::endl;
    }

    dirLightDirLoc = glGetUniformLocation(program, const_cast<const char*>("directionalLight.direction"));
    if(dirLightDirLoc == -1)
    {
        std::cerr << "[F] DIR DIRECTION COLOR NOT FOUND" << std::endl;
    }

    dirLightIntensityLoc = glGetUniformLocation(program, const_cast<const char*>("directionalLight.base.diffuseIntensity"));
    if(dirLightIntensityLoc == -1)
    {
        std::cerr << "[F] DIR INTENSITY INTENSITY NOT FOUND" << std::endl;
    }

    // point light
    pointLightColorLoc = glGetUniformLocation(program, const_cast<const char*>("pointLight.base.color"));
    if(pointLightColorLoc == -1)
    {
        std::cerr << "[F] POINT LIGHT COLOR NOT FOUND" << std::endl;
        return false;
    }

    pointLightAmbientIntensityLoc = glGetUniformLocation(program, const_cast<const char*>("pointLight.base.ambientIntensity"));
    if(pointLightAmbientIntensityLoc == -1)
    {
        std::cerr << "[F] POINT AMB INTENSITY NOT FOUND" << std::endl;
        return false;
    }

    pointLightPosLoc = glGetUniformLocation(program, const_cast<const char*>("pointLight.pos"));
    if(pointLightPosLoc == -1)
    {
        std::cerr << "[F] POINT DIRECTION COLOR NOT FOUND" << std::endl;
        return false;
    }

    pointLightIntensityLoc = glGetUniformLocation(program, const_cast<const char*>("pointLight.base.diffuseIntensity"));
    if(pointLightIntensityLoc == -1)
    {
        std::cerr << "[F] POINT INTENSITY INTENSITY NOT FOUND" << std::endl;
        return false;
    }
    pointLightConstantLoc = glGetUniformLocation(program, const_cast<const char*>("pointLight.atten.constant"));
    if(pointLightConstantLoc == -1)
    {
        std::cerr << "[F] POINT CONST NOT FOUND" << std::endl;
        return false;
    }

    pointLightLinearLoc = glGetUniformLocation(program, const_cast<const char*>("pointLight.atten.linear"));
    if(pointLightLinearLoc == -1)
    {
        std::cerr << "[F] POINT LINEAR NOT FOUND" << std::endl;
        return false;
    }

    pointLightExpLoc = glGetUniformLocation(program, const_cast<const char*>("pointLight.atten.exp"));
    if(pointLightExpLoc == -1)
    {
        std::cerr << "[F] POINT EXP NOT FOUND" << std::endl;
        return false;
    }

    water = new Water(program);
    water->createWaterMesh(200, 200, .1);

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

    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry);
    //GDALClose( (GDALDatasetH) poDataset );

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