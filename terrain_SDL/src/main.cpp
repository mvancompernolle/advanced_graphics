#include <GL/glew.h> // glew must be included before the main gl libs
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include <btBulletDynamicsCommon.h>

#include "ShaderLoader.h"
#include "Mesh.h"
#include "Terrain.h"
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>


//--Data types
//This object will define the attributes of a vertex(position, color, etc...)


// LIGHT REFERENCES: http://ogldev.atspace.co.uk/www/tutorial18/tutorial18.html (Tutorials 17 - 21)
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
		this->direction = dir;
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

struct SpotLight : public PointLight{

	public:
	glm::vec3 direction;
	float radius;

	SpotLight(glm::vec3 col, glm::vec3 pos, glm::vec3 dir, float rad, float diffIntensity, float ambIntensity) : PointLight(col, pos, ambIntensity, diffIntensity){
		this->pos = pos;
		this->radius = rad;
	}
};

//--Evil Global variables
//Just for this example!
int w = 0, h = 0;// Window size
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry
Terrain *terrain;
Mesh  *floorMesh, *sphereMesh, *puckMesh, *paddleMesh1, *paddleMesh2;
float zoomScale = -50, cameraX = 0, cameraY = 50, resetCounter = 0.0f;
char *filename;
float scaleFactor = 1.0f, terrainScale = 1.0f;

// SDL globals
SDL_Window *window;
SDL_GLContext gl_context;

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader
GLint loc_planeWidth, loc_planeHeight;

// setup lights
bool ambLightOn = true, dirLightOn = false, pointLightOn = false, spotLightOn = false;
GLint loc_specularStrength, loc_matSpecularIntensity;

glm::vec3 ambientLightColor(1.0, 1.0, 1.0);
float ambientLightIntensity = 1.0;
GLint ambLightColorLoc, ambLightAmbientIntensityLoc; 
BaseLight ambLight(ambientLightColor, ambientLightIntensity, 1.0f);

glm::vec3 dirLightColor(1.0, 1.0, 1.0);
float dirLightDifIntensity = 0.4, dirLightAmbIntensity = 0.0;
GLint dirLightColorLoc, dirLightAmbientIntensityLoc, dirLightDirLoc, dirLightIntensityLoc;
DirectionalLight dirLight(dirLightColor, glm::vec3(1.0, -0.5, 0.0), dirLightDifIntensity, dirLightAmbIntensity);

GLint pointLightColorLoc, pointLightAmbientIntensityLoc, pointLightPosLoc, pointLightIntensityLoc, pointLightConstantLoc, pointLightLinearLoc, pointLightExpLoc;
glm::vec3 pointLightPos(0.0, 12.0, 0.0);
glm::vec3 pointLightColor(1.0, 1.0, 1.0);
float pointLightConstant = 1.0, pointLightLinear = 0.1, pointLightExp = 0.0;
float pointLightAmbIntensity = 0.0;
float pointLightDifIntensity = 0.5;
PointLight pointLight(pointLightColor, pointLightPos, pointLightDifIntensity, pointLightAmbIntensity);

GLint spotLightColorLoc, spotLightAmbientIntensityLoc, spotLightPosLoc, spotLightIntensityLoc, spotLightConstantLoc, spotLightLinearLoc, spotLightExpLoc, spotLightRadiusLoc, spotLightDirLoc;
glm::vec3 spotLightPos(8.0, 8.0, 0.0);
glm::vec3 spotLightColor(1.0, 0.0, 1.0);
float spotLightConstant = 1.0, spotLightLinear = 0.1, spotLightExp = 0.0;
float spotLightAmbIntensity = 0.0;
float spotLightDifIntensity = 0.9;
float spotLightRadius = 180.0f;
glm::vec3 spotLightDir = glm::vec3(1.0, -1.0, 0.0);
SpotLight spotLight(spotLightColor, spotLightPos, spotLightDir, spotLightRadius, spotLightAmbIntensity, spotLightDifIntensity);

//attribute locations
GLint loc_position;
GLint loc_color;
GLint loc_normal;
GLint loc_texture;
GLint loc_world;
GLint loc_vertScalar;
GLint loc_terrainScalar;
GLint gSampler;

//transform matrices
glm::mat4 floorModel;
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

// bullet variables
btCollisionShape* collisionShape;
btBroadphaseInterface* broadphase; 
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btDefaultCollisionConfiguration* collisionConfiguration;
btDiscreteDynamicsWorld* dynamicsWorld;

//--GLUT Callbacks
void SDLMainLoop();
void createWindow(int width, int height);
void pollEvents(bool &running);
void render();
void update();
void keyboard(unsigned char key, int x_pos, int y_pos);
void arrowKeyboard(int key, int x, int y);

//--Resource management
glm::mat4 convertFArrayToMat4(float[]);
void InitBulletPhysics();
void StepBulletPhysics(float dt);
bool initialize();
void cleanUp();

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;


//--Main
int main(int argc, char **argv)
{
	if(argc > 2){
		// read in scaling factor for terrain and file name
		filename = argv[1];
		scaleFactor = atof(argv[2]);
		if(argc > 3){
			terrainScale = atof(argv[3]);
		}
	}
	else{
		std::cout << "You must pass in a file name for a first argument and a scale factor for the terrain height as a second argument." << std::endl;
		exit(1);
	}

	// Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Cannot Initialize SDL" << std::endl;
        exit(1);
    }

    createWindow(1000, 1000);

    // Now that the window is created the GL context is fully set up
    // Because of that we can now initialize GLEW to prepare work with shaders
    GLenum status = glewInit();
    if( status != GLEW_OK)
    {
        std::cerr << "[F] GLEW NOT INITIALIZED: ";
        std::cerr << glewGetErrorString(status) << std::endl;
        return -1;
    }

    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    // Initialize all of our resources(shaders, geometry)
    bool init = initialize();
    if(init)
    {
        t1 = std::chrono::high_resolution_clock::now();
        SDLMainLoop();
    }

    // Clean up after ourselves
    cleanUp();
    return 0;
}

void SDLMainLoop()
{
	// While program is running check for events and render
	bool running = true;
    while(running) 
	{
        pollEvents(running);
		update();
		render();
    }
}

void createWindow(int width, int height)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    window = SDL_CreateWindow("CS 480/680 - Terrain HeighMap", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	//void SDL_GetWindowSize(window, w, h);

    if(!window) {
        std::cerr << "Unable to create GL Window" << std::endl;
        exit(0);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetSwapInterval(1);

    gl_context = SDL_GL_CreateContext(window);
}

void pollEvents(bool &running)
{
    SDL_Event event;
    char key;
	
	// Loop through all events that have occurred
    while(SDL_PollEvent(&event)) 
	{
        switch(event.type) 
		{
			// Check to see if a key was pressed down
            case SDL_KEYDOWN:
				// Get the type of key that was pressed
                key = (char)event.key.keysym.sym;

				// If escape was pressed, cleanup program and exit
                if(key == SDLK_ESCAPE) 
				{
					running = false;
		    		cleanUp();
                    exit(0);
                }
				break;
        }
    }
}

//--Implementations
void render()
{
    //--Render the scene
    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //enable the shader program
    glUseProgram(program);

	// render floor
	mvp = projection * view * floorModel;

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(loc_world, 1, GL_FALSE, glm::value_ptr(floorModel));
    glUniform1i(gSampler, 0);
	glUniform1i(loc_planeWidth, terrain->getWidth());
	glUniform1i(loc_planeHeight, terrain->getHeight());
	glUniform1f(loc_terrainScalar, terrainScale);
	glUniform1f(loc_vertScalar, scaleFactor);

	//upload lights to the shader

	// ambient light
	glUniform3f(ambLightColorLoc, ambLight.color[0], ambLight.color[1], ambLight.color[2]);
	glUniform1f(ambLightAmbientIntensityLoc, ambLight.ambientIntensity);	

	// diffuse light
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

	// spot light
	glUniform3f(spotLightColorLoc, spotLight.color[0], spotLight.color[1], spotLight.color[2]);
	glUniform1f(spotLightAmbientIntensityLoc, spotLight.ambientIntensity);	
	glUniform3f(spotLightPosLoc, spotLight.pos[0], spotLight.pos[1], spotLight.pos[2]);
	glUniform3f(spotLightDirLoc, spotLight.direction[0], spotLight.direction[1], spotLight.direction[2]);
	glUniform1f(spotLightIntensityLoc, spotLight.diffuseIntensity);
	glUniform1f(spotLightRadiusLoc, cosf((spotLight.radius)));

	glUniform1f(spotLightConstantLoc, spotLightConstant);
	glUniform1f(spotLightLinearLoc, spotLightLinear);
	glUniform1f(spotLightExpLoc, spotLightExp);
			
	// render meshes
	terrain->render(loc_position, loc_normal);

	// swap the render buffers
	SDL_GL_SwapWindow(window);
}

void update()
{
	// update physics
	float dt = getDT();
	StepBulletPhysics(dt);
}


/*void keyboard(unsigned char key, int x_pos, int y_pos)
{
		// Handle keyboard input
		switch(key)
		{
			case 27: //ESC
				exit(0);
				break;
			case '5':
				if(ambLightOn){
					ambLightOn = false;
					ambLight.ambientIntensity = 0.0f;
				}
				else{
					ambLightOn = true;
					ambLight.ambientIntensity = ambientLightIntensity;
				}
				break;
			case '6':
				if(dirLightOn){
					dirLightOn = false;
					dirLight.ambientIntensity = 0.0f;
					dirLight.diffuseIntensity = 0.0f;
				}
				else{
					dirLightOn = true;
					dirLight.ambientIntensity = dirLightAmbIntensity;
					dirLight.diffuseIntensity = dirLightDifIntensity;
				}
				break;
			case '7':
				if(pointLightOn){
					pointLightOn = false;
					pointLight.ambientIntensity = 0.0f;
					pointLight.diffuseIntensity = 0.0f;
				}
				else{
					pointLightOn = true;
					pointLight.ambientIntensity = pointLightAmbIntensity;
					pointLight.diffuseIntensity = pointLightDifIntensity;
				}
				break;
			case '8':
				if(spotLightOn){
					spotLightOn = false;
					spotLight.ambientIntensity = 0.0f;
					spotLight.diffuseIntensity = 0.0f;
				}
				else{
					spotLightOn = true;
					spotLight.ambientIntensity = spotLightAmbIntensity;
					spotLight.diffuseIntensity = spotLightDifIntensity;
				}
				break;
		}
	

}

void arrowKeyboard(int key, int x, int y)
{
	    // Handle keyboard input
	switch(key)
	{
		
		case GLUT_KEY_LEFT: // left arrow
			if(cameraX < 10 && zoomScale != -0.01){
				if(zoomScale > -18){
					zoomScale = -18;
				}
					cameraX += 2;
					view = glm::lookAt( glm::vec3(cameraX, cameraY, zoomScale), //Eye Position
					            glm::vec3(0.0, 0.0, 0.0), //Focus point
					            glm::vec3(0.0, 1.0, 0.0));
			}
			break;
		case GLUT_KEY_RIGHT: // right arrow
			if(cameraX > -10 && zoomScale != -0.01){
				if(zoomScale > -18){
					zoomScale = -18;
				}
				cameraX -= 2;
				view = glm::lookAt( glm::vec3(cameraX, cameraY, zoomScale), //Eye Position
			                glm::vec3(0.0, 0.0, 0.0), //Focus point
			                glm::vec3(0.0, 1.0, 0.0));
			}

			break;
	
		case GLUT_KEY_UP:
			zoomScale+=3;
			if(zoomScale >= 0){
				zoomScale = -.01;
				cameraX = 0;
			}
			view = glm::lookAt( glm::vec3(cameraX, cameraY, zoomScale), //Eye Position
		                glm::vec3(0.0, 0.0, 0.0), //Focus point
		                glm::vec3(0.0, 1.0, 0.0));
			break;
		case GLUT_KEY_DOWN:
			zoomScale-= 3;
			if(zoomScale < -50){
				zoomScale = -50;
			}
			view = glm::lookAt( glm::vec3(cameraX, cameraY, zoomScale), //Eye Position
	                    glm::vec3(0.0, 0.0, 0.0), //Focus point
	                    glm::vec3(0.0, 1.0, 0.0));
			break;
		
	}
}*/

bool initialize()
{
	GLint shader_status;

	// get terrain texture
	terrain = new Terrain();
	terrain->loadHeightMap(filename, terrainScale);

    //--Geometry done

    ShaderLoader vertShader(GL_VERTEX_SHADER);
	ShaderLoader fragShader(GL_FRAGMENT_SHADER);

    //Shader Sources
    // Put these into files and write a loader in the future
    // Note the added uniform!
    

	if(!vertShader.loadShader("../bin/shaders/vertShader.txt") || !
	fragShader.loadShader("../bin/shaders/fragShader.txt")){
		std::cout<<"ERROR: Load Shader Issue" << std::endl;
		return false;
	}
	

    //Now we link the 2 shader objects into a program
    //This program is what is run on the GPU
    program = glCreateProgram();
    glAttachShader(program, vertShader.getShader());
    glAttachShader(program, fragShader.getShader());
    glLinkProgram(program);
    //check if everything linked ok
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
        return false;
    }

    //Now we set the locations of the attributes and uniforms
    //this allows us to access them easily while rendering
    loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
        return false;
    }

    loc_normal = glGetAttribLocation(program,
                    const_cast<const char*>("v_normal"));
    if(loc_normal == -1)
    {
        std::cerr << "[F] NORMAL NOT FOUND" << std::endl;
        return false;
    }

    loc_terrainScalar = glGetUniformLocation(program,
                    const_cast<const char*>("terrainDimenScale"));
    if(loc_terrainScalar == -1)
    {
        /*std::cerr << "[F] TERRAIN SCALAR NOT FOUND" << std::endl;
        return false;*/
    }

    loc_vertScalar = glGetUniformLocation(program,
                    const_cast<const char*>("verticalScalar"));
    if(loc_vertScalar == -1)
    {
        std::cerr << "[F] VERTICAL SCALAR NOT FOUND" << std::endl;
        return false;
    }


    /*loc_texture = glGetAttribLocation(program,
                    const_cast<const char*>("texCoord"));
    if(loc_texture == -1)
    {
        std::cerr << "[F] TEXTURE NOT FOUND" << std::endl;
        return false;
    }*/


    loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
        return false;
    }

    loc_planeWidth = glGetUniformLocation(program,
                    const_cast<const char*>("width"));
    if(loc_planeWidth == -1)
    {
        std::cerr << "[F] PLANE WIDTH NOT FOUND" << std::endl;
        return false;
    }

    loc_planeHeight = glGetUniformLocation(program,
                    const_cast<const char*>("height"));
    if(loc_planeHeight == -1)
    {
        std::cerr << "[F] PLANE HEIGHT NOT FOUND" << std::endl;
        return false;
    }

    loc_world = glGetUniformLocation(program,
                    const_cast<const char*>("world"));
    if(loc_world == -1)
    {
        std::cerr << "[F] WORLD NOT FOUND" << std::endl;
        return false;
    }

    gSampler = glGetUniformLocation(program,
                    const_cast<const char*>("gSampler"));
    if(gSampler == -1)
    {
        std::cerr << "[F] SAMPLER NOT FOUND" << std::endl;
        return false;
    }

	// ambient light
	ambLightColorLoc = glGetUniformLocation(program, const_cast<const char*>("ambientLight.color"));
    if(ambLightColorLoc == -1)
    {
        std::cerr << "[F] AMB LIGHT COLOR NOT FOUND" << std::endl;
        return false;
    }

	ambLightAmbientIntensityLoc = glGetUniformLocation(program, const_cast<const char*>("ambientLight.ambientIntensity"));
    if(ambLightColorLoc == -1)
    {
        std::cerr << "[F] AMB INTENSITY NOT FOUND" << std::endl;
        return false;
    }

	// directional light
	dirLightColorLoc = glGetUniformLocation(program, const_cast<const char*>("directionalLight.base.color"));
    if(dirLightColorLoc == -1)
    {
        std::cerr << "[F] DIR LIGHT COLOR NOT FOUND" << std::endl;
        return false;
    }

	dirLightAmbientIntensityLoc = glGetUniformLocation(program, const_cast<const char*>("directionalLight.base.ambientIntensity"));
    if(dirLightColorLoc == -1)
    {
        std::cerr << "[F] DIR AMB INTENSITY NOT FOUND" << std::endl;
        return false;
    }

	dirLightDirLoc = glGetUniformLocation(program, const_cast<const char*>("directionalLight.direction"));
    if(dirLightDirLoc == -1)
    {
        std::cerr << "[F] DIR DIRECTION COLOR NOT FOUND" << std::endl;
        return false;
    }

	dirLightIntensityLoc = glGetUniformLocation(program, const_cast<const char*>("directionalLight.base.diffuseIntensity"));
    if(dirLightIntensityLoc == -1)
    {
        std::cerr << "[F] DIR INTENSITY INTENSITY NOT FOUND" << std::endl;
        return false;
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


	// spot light
	spotLightRadiusLoc = glGetUniformLocation(program, const_cast<const char*>("spotLight.radius"));
    if(spotLightRadiusLoc == -1)
    {
        std::cerr << "[F] SPOT LIGHT RADIUS NOT FOUND" << std::endl;
        return false;
    }

	spotLightDirLoc = glGetUniformLocation(program, const_cast<const char*>("spotLight.direction"));
    if(spotLightDirLoc == -1)
    {
        std::cerr << "[F] SPOT LIGHT DIRECTION NOT FOUND" << std::endl;
        return false;
    }

	spotLightColorLoc = glGetUniformLocation(program, const_cast<const char*>("spotLight.base.base.color"));
    if(spotLightColorLoc == -1)
    {
        std::cerr << "[F] SPOT LIGHT COLOR NOT FOUND" << std::endl;
        return false;
    }

	spotLightAmbientIntensityLoc = glGetUniformLocation(program, const_cast<const char*>("spotLight.base.base.ambientIntensity"));
    if(spotLightAmbientIntensityLoc == -1)
    {
        std::cerr << "[F] SPOT AMB INTENSITY NOT FOUND" << std::endl;
        return false;
    }

	spotLightPosLoc = glGetUniformLocation(program, const_cast<const char*>("spotLight.base.pos"));
    if(spotLightPosLoc == -1)
    {
        std::cerr << "[F] SPOT DIRECTION COLOR NOT FOUND" << std::endl;
        return false;
    }

	spotLightIntensityLoc = glGetUniformLocation(program, const_cast<const char*>("spotLight.base.base.diffuseIntensity"));
    if(spotLightIntensityLoc == -1)
    {
        std::cerr << "[F] SPOT INTENSITY INTENSITY NOT FOUND" << std::endl;
        return false;
    }
	spotLightConstantLoc = glGetUniformLocation(program, const_cast<const char*>("spotLight.base.atten.constant"));
    if(spotLightConstantLoc == -1)
    {
        std::cerr << "[F] SPOT CONST NOT FOUND" << std::endl;
        return false;
    }

	spotLightLinearLoc = glGetUniformLocation(program, const_cast<const char*>("spotLight.base.atten.linear"));
    if(spotLightLinearLoc == -1)
    {
        std::cerr << "[F] SPOT LINEAR NOT FOUND" << std::endl;
        return false;
    }

	spotLightExpLoc = glGetUniformLocation(program, const_cast<const char*>("spotLight.base.atten.exp"));
    if(spotLightExpLoc == -1)
    {
        std::cerr << "[F] SPOT EXP NOT FOUND" << std::endl;
        return false;
    }
	


    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    view = glm::lookAt( glm::vec3(-0.0, 80.0, -100.0), //Eye Position
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   300.0f); //Distance to the far plane, 

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // render in wire frame mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.0, 0.0, 1.0, 1.0);

	// initialize bullet
	InitBulletPhysics();

    return true;
}

void InitBulletPhysics(){
	// initialize bullet
    broadphase = new btDbvtBroadphase();

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-20,0));

	// create a ground shape
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);
}

void StepBulletPhysics(float dt)
{
	if(dynamicsWorld)//step the simulation
		dynamicsWorld->stepSimulation(dt, 10);

}

glm::mat4 convertFArrayToMat4(float m[]){
	glm::mat4 returnMatrix;

	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			returnMatrix[i][j] = m[4*i + j];
		}
	}
			
	return returnMatrix;
}

void cleanUp()
{
	// SDL clean up 
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Clean up, Clean up
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry);
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
