#include <GL/glew.h> // glew must be included before the main gl libs
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "CEGUI/CEGUI.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#include "ShaderLoader.h"
#include "Terrain.h"

//--Data types
//This object will define the attributes of a vertex(position, color, etc...)
enum ShaderVariable {Attribute, Uniform};

//--Evil Global variables
//Just for this example!
SDL_Window *window;
SDL_GLContext gl_context;
int windowWidth = 1000, windowHeight = 1000;// Window size
float camY = 50.0f, camX = 0.0f, camZ = 50.0f;
float drawDistance = 500.0f;
GLuint program, grayscaleProgram;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint loc_scalar;
GLint loc_sampler;
Terrain *terrain, *terrain2;
Texture *colorMap;

// CEGUI variables
CEGUI::Window *gJumpBtnWindow = NULL;

//transform matrices
glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojections

// SDL related functions
void createWindow(int width, int height);
void SDLMainLoop(float scaleFactor);
void update();
void render(float scaleFactor);

// Resource Management
void cleanup();
bool initialize(bool errorChecks, const char* fileName, float scaleFactor, float terrainScale);
bool getShaderLoc(GLint var, const char* shaderName, ShaderVariable type, bool errorCheck);

//--Random time things
void CreateJumpButton()
{
  gJumpBtnWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button","JumpPushButton");  // Create Window
  gJumpBtnWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.9,0),CEGUI::UDim(0.1, 0.0)));
  gJumpBtnWindow->setSize(CEGUI::USize(CEGUI::UDim(0,150),CEGUI::UDim(0,50)));
  gJumpBtnWindow->setText("Jump!");
  CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(gJumpBtnWindow);  
}

std::map<SDL_Keycode, CEGUI::Key::Scan> m_keymap;
 
void initializeKeyMap() {
  m_keymap[SDLK_1] = CEGUI::Key::One;
  m_keymap[SDLK_2] = CEGUI::Key::Two;
  m_keymap[SDLK_3] = CEGUI::Key::Three;
  m_keymap[SDLK_4] = CEGUI::Key::Four;
  m_keymap[SDLK_5] = CEGUI::Key::Five;
  m_keymap[SDLK_6] = CEGUI::Key::Six;
  m_keymap[SDLK_7] = CEGUI::Key::Seven;
  m_keymap[SDLK_8] = CEGUI::Key::Eight;
  m_keymap[SDLK_9] = CEGUI::Key::Nine;
  m_keymap[SDLK_0] = CEGUI::Key::Zero;
 
  m_keymap[SDLK_q] = CEGUI::Key::Q;
  m_keymap[SDLK_w] = CEGUI::Key::W;
  m_keymap[SDLK_e] = CEGUI::Key::E;
  m_keymap[SDLK_r] = CEGUI::Key::R;
  m_keymap[SDLK_t] = CEGUI::Key::T;
  m_keymap[SDLK_y] = CEGUI::Key::Y;
  m_keymap[SDLK_u] = CEGUI::Key::U;
  m_keymap[SDLK_i] = CEGUI::Key::I;
  m_keymap[SDLK_o] = CEGUI::Key::O;
  m_keymap[SDLK_p] = CEGUI::Key::P;
  m_keymap[SDLK_a] = CEGUI::Key::A;
  m_keymap[SDLK_s] = CEGUI::Key::S;
  m_keymap[SDLK_d] = CEGUI::Key::D;
  m_keymap[SDLK_f] = CEGUI::Key::F;
  m_keymap[SDLK_g] = CEGUI::Key::G;
  m_keymap[SDLK_h] = CEGUI::Key::H;
  m_keymap[SDLK_j] = CEGUI::Key::J;
  m_keymap[SDLK_k] = CEGUI::Key::K;
  m_keymap[SDLK_l] = CEGUI::Key::L;
  m_keymap[SDLK_z] = CEGUI::Key::Z;
  m_keymap[SDLK_x] = CEGUI::Key::X;
  m_keymap[SDLK_c] = CEGUI::Key::C;
  m_keymap[SDLK_v] = CEGUI::Key::V;
  m_keymap[SDLK_b] = CEGUI::Key::B;
  m_keymap[SDLK_n] = CEGUI::Key::N;
  m_keymap[SDLK_m] = CEGUI::Key::M;
 
  m_keymap[SDLK_COMMA] = CEGUI::Key::Comma;
  m_keymap[SDLK_PERIOD] = CEGUI::Key::Period;
  m_keymap[SDLK_SLASH] = CEGUI::Key::Slash;
  m_keymap[SDLK_BACKSLASH] = CEGUI::Key::Backslash;
  m_keymap[SDLK_MINUS] = CEGUI::Key::Minus;
  m_keymap[SDLK_EQUALS] = CEGUI::Key::Equals;
  m_keymap[SDLK_SEMICOLON] = CEGUI::Key::Semicolon;
  m_keymap[SDLK_LEFTBRACKET] = CEGUI::Key::LeftBracket;
  m_keymap[SDLK_RIGHTBRACKET] = CEGUI::Key::RightBracket;
  m_keymap[SDLK_QUOTE] = CEGUI::Key::Apostrophe;
  m_keymap[SDLK_BACKQUOTE] = CEGUI::Key::Grave;
 
  m_keymap[SDLK_RETURN] = CEGUI::Key::Return;
  m_keymap[SDLK_SPACE] = CEGUI::Key::Space;
  m_keymap[SDLK_BACKSPACE] = CEGUI::Key::Backspace;
  m_keymap[SDLK_TAB] = CEGUI::Key::Tab;
 
  m_keymap[SDLK_ESCAPE] = CEGUI::Key::Escape;
  m_keymap[SDLK_PAUSE] = CEGUI::Key::Pause;
  m_keymap[SDLK_SYSREQ] = CEGUI::Key::SysRq;
  m_keymap[SDLK_POWER] = CEGUI::Key::Power;
 
  m_keymap[SDLK_NUMLOCKCLEAR] = CEGUI::Key::NumLock;
  m_keymap[SDLK_SCROLLLOCK] = CEGUI::Key::ScrollLock;
 
  m_keymap[SDLK_F1] = CEGUI::Key::F1;
  m_keymap[SDLK_F2] = CEGUI::Key::F2;
  m_keymap[SDLK_F3] = CEGUI::Key::F3;
  m_keymap[SDLK_F4] = CEGUI::Key::F4;
  m_keymap[SDLK_F5] = CEGUI::Key::F5;
  m_keymap[SDLK_F6] = CEGUI::Key::F6;
  m_keymap[SDLK_F7] = CEGUI::Key::F7;
  m_keymap[SDLK_F8] = CEGUI::Key::F8;
  m_keymap[SDLK_F9] = CEGUI::Key::F9;
  m_keymap[SDLK_F10] = CEGUI::Key::F10;
  m_keymap[SDLK_F11] = CEGUI::Key::F11;
  m_keymap[SDLK_F12] = CEGUI::Key::F12;
  m_keymap[SDLK_F13] = CEGUI::Key::F13;
  m_keymap[SDLK_F14] = CEGUI::Key::F14;
  m_keymap[SDLK_F15] = CEGUI::Key::F15;
 
  m_keymap[SDLK_LCTRL] = CEGUI::Key::LeftControl;
  m_keymap[SDLK_LALT] = CEGUI::Key::LeftAlt;
  m_keymap[SDLK_LSHIFT] = CEGUI::Key::LeftShift;
  m_keymap[SDLK_LGUI] = CEGUI::Key::LeftWindows;
  m_keymap[SDLK_RCTRL] = CEGUI::Key::RightControl;
  m_keymap[SDLK_RALT] = CEGUI::Key::RightAlt;
  m_keymap[SDLK_RSHIFT] = CEGUI::Key::RightShift;
  m_keymap[SDLK_RGUI] = CEGUI::Key::RightWindows;
  m_keymap[SDLK_MENU] = CEGUI::Key::AppMenu;
 
  m_keymap[SDLK_KP_0] = CEGUI::Key::Numpad0;
  m_keymap[SDLK_KP_1] = CEGUI::Key::Numpad1;
  m_keymap[SDLK_KP_2] = CEGUI::Key::Numpad2;
  m_keymap[SDLK_KP_3] = CEGUI::Key::Numpad3;
  m_keymap[SDLK_KP_4] = CEGUI::Key::Numpad4;
  m_keymap[SDLK_KP_5] = CEGUI::Key::Numpad5;
  m_keymap[SDLK_KP_6] = CEGUI::Key::Numpad6;
  m_keymap[SDLK_KP_7] = CEGUI::Key::Numpad7;
  m_keymap[SDLK_KP_8] = CEGUI::Key::Numpad8;
  m_keymap[SDLK_KP_9] = CEGUI::Key::Numpad9;
  m_keymap[SDLK_KP_PERIOD] = CEGUI::Key::Decimal;
  m_keymap[SDLK_KP_PLUS] = CEGUI::Key::Add;
  m_keymap[SDLK_KP_MINUS] = CEGUI::Key::Subtract;
  m_keymap[SDLK_KP_MULTIPLY] = CEGUI::Key::Multiply;
  m_keymap[SDLK_KP_DIVIDE] = CEGUI::Key::Divide;
  m_keymap[SDLK_KP_ENTER] = CEGUI::Key::NumpadEnter;
 
  m_keymap[SDLK_UP] = CEGUI::Key::ArrowUp;
  m_keymap[SDLK_LEFT] = CEGUI::Key::ArrowLeft;
  m_keymap[SDLK_RIGHT] = CEGUI::Key::ArrowRight;
  m_keymap[SDLK_DOWN] = CEGUI::Key::ArrowDown;
 
  m_keymap[SDLK_HOME] = CEGUI::Key::Home;
  m_keymap[SDLK_END] = CEGUI::Key::End;
  m_keymap[SDLK_PAGEUP] = CEGUI::Key::PageUp;
  m_keymap[SDLK_PAGEDOWN] = CEGUI::Key::PageDown;
  m_keymap[SDLK_INSERT] = CEGUI::Key::Insert;
  m_keymap[SDLK_DELETE] = CEGUI::Key::Delete;
}

void handle_mouse_down(Uint8 button)
{
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext(); 
    switch ( button ) {
        case SDL_BUTTON_LEFT:
            context.injectMouseButtonDown(CEGUI::LeftButton);
            break;
        case SDL_BUTTON_MIDDLE:
            context.injectMouseButtonDown(CEGUI::MiddleButton);
            break;
        case SDL_BUTTON_RIGHT:
            context.injectMouseButtonDown(CEGUI::RightButton);
            break;
    }
}

void handle_mouse_up(Uint8 button)
{
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext(); 
    switch ( button )
    {
        case SDL_BUTTON_LEFT:
            context.injectMouseButtonUp(CEGUI::LeftButton);
            break;
        case SDL_BUTTON_MIDDLE:
            context.injectMouseButtonUp(CEGUI::MiddleButton);
            break;
        case SDL_BUTTON_RIGHT:
            context.injectMouseButtonUp(CEGUI::RightButton);
            break;
    }
}

void injectInput (bool & running) 
{
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext(); 
 
    initializeKeyMap();
 
    SDL_Event e;
    /* go through all available events */
    while (SDL_PollEvent(&e)) {
        /* we use a switch to determine the event type */
        switch (e.type) {
            /* mouse motion handler */
            case SDL_MOUSEMOTION:
                /* we inject the mouse position directly. */
              //                CEGUI::System::getSingleton().injectMousePosition(static_cast<float>(e.motion.x),static_cast<float>(e.motion.y));
              context.injectMousePosition( static_cast<float>(e.motion.x),static_cast<float>(e.motion.y) );
                break;
 
            /* mouse down handler */
            case SDL_MOUSEBUTTONDOWN:
                /* let a special function handle the mouse button down event */
                handle_mouse_down (e.button.button);
                break;
 
            /* mouse up handler */
            case SDL_MOUSEBUTTONUP:
                /* let a special function handle the mouse button up event */
                handle_mouse_up (e.button.button);
                break;
 
            /* key down */
            case SDL_KEYDOWN:
                // Get the type of key that was pressed
                switch( e.key.keysym.sym ){
                    case SDLK_w:
                        terrain->showNextDataBand();
                        break;
                    case SDLK_s:
                        terrain->showPrevDataBand();
                        break;
                    case SDLK_a:
                        terrain->showPrevDataTime();
                        break;
                    case SDLK_d:
                        terrain->showNextDataTime();
                        break;
                    case SDLK_1:
                        terrain->setDataZoneData("isnobaloutput/em.1000.tif", false);
                        break;
                    case SDLK_2:
                        terrain->setDataZoneData("isnobaloutput/snow.1000.tif", false);
                        break;
                    case SDLK_LEFT:
                        camX -= 2;
                        view = glm::lookAt( glm::vec3(camX, camY, camZ), //Eye Position
                                    glm::vec3(0.0, 0.0, 0.0), //Focus point
                                    glm::vec3(0.0, 1.0, 0.0));
                        break;
                    case SDLK_RIGHT:
                        camX += 2;
                        view = glm::lookAt( glm::vec3(camX, camY, camZ), //Eye Position
                                    glm::vec3(0.0, 0.0, 0.0), //Focus point
                                    glm::vec3(0.0, 1.0, 0.0));
                        break;
                    case SDLK_UP:
                        camZ -= 2;
                        view = glm::lookAt( glm::vec3(camX, camY, camZ), //Eye Position
                                    glm::vec3(0.0, 0.0, 0.0), //Focus point
                                    glm::vec3(0.0, 1.0, 0.0));
                        break;
                    case SDLK_DOWN:
                        camZ += 2;
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

                /* to tell CEGUI that a key was pressed, we inject the scancode. */
              //e.key.keysym.scancode
              context.injectKeyDown(m_keymap[e.key.keysym.sym]);
                /* as for the character it's a litte more complicated.
                 * we'll use for translated unicode value.
                 * this is described in more detail below.
                 */
                if ((e.key.keysym.sym & 0xFF80) == 0) {
                    context.injectChar(e.key.keysym.sym & 0x7F);
                }
                break;
 
            /* key up */
            case SDL_KEYUP:
                /* like before we inject the scancode directly. */
              //e.key.keysym.scancode
                context.injectKeyUp(m_keymap[e.key.keysym.sym]);
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
                    CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(windowWidth,windowHeight));                  
                    break;
                }
                break;
        }
    }
}

void injectTimePulse(double& last_time_pulse)
{
    /* get current "run-time" in seconds */
    double t = 0.001*SDL_GetTicks();
    /* inject the time that passed since the last call */
    CEGUI::System::getSingleton().injectTimePulse( float(t-last_time_pulse) );
    /* store the new time as the last time */
    last_time_pulse = t;
}

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
    //SDL_ShowCursor(SDL_DISABLE);

    // setup cegui
    CEGUI::OpenGL3Renderer::bootstrapSystem();

    // initialise the required dirs for the DefaultResourceProvider
    CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
        (CEGUI::System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
    rp->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
    rp->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
    rp->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
    rp->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");
    // This is only really needed if you are using Xerces and need to
    // specify the schemas location
    rp->setResourceGroupDirectory("schemas", "../datafiles/xml_schemas/");

    // set the default resource groups to be used
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
    // setup default group for validation schemas
    CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
    if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
        parser->setProperty("SchemaDefaultResourceGroup", "schemas");

    using namespace CEGUI;
    SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );
    FontManager::getSingleton().createFromFile( "DejaVuSans-10.font" );
 
    System::getSingleton().getDefaultGUIContext().setDefaultFont( "DejaVuSans-10" );
    System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage( "TaharezLook/MouseArrow" );
    System::getSingleton().getDefaultGUIContext().setDefaultTooltipType( "TaharezLook/Tooltip" );
 
    WindowManager& wmgr = WindowManager::getSingleton();
    Window* myRoot = wmgr.createWindow( "DefaultWindow", "root" );
    System::getSingleton().getDefaultGUIContext().setRootWindow( myRoot );
    FrameWindow* fWnd = static_cast<FrameWindow*>(
                              wmgr.createWindow( "TaharezLook/FrameWindow", "testWindow" ));
    myRoot->addChild( fWnd );
    // position a quarter of the way in from the top-left of parent.
    fWnd->setPosition( UVector2( UDim( 0.9f, 0.0f ), UDim( 0.0f, 0.1f ) ) );
    // set size to be half the size of the parent
    fWnd->setSize( USize( UDim( 0.1f, 0.0f ), UDim( 0.1f, 0.0f ) ) );
    fWnd->setText( "Dry Creek" );

    CreateJumpButton();

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
    bool init = initialize(false, fileName, scaleFactor, terrainScale);
    if(init)
    {
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

void SDLMainLoop(float scaleFactor)
{
	// While program is running check for events and render
    bool running = true;
    double last_time_pulse = 0.001*static_cast<double>(SDL_GetTicks());
    while(running) {
    injectInput(running);
    injectTimePulse(last_time_pulse);
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
    glUseProgram(grayscaleProgram);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform1i(loc_sampler, 0);
    glUniform1f(loc_scalar, scaleFactor);

    terrain->render(projection, view);
    //terrain2->render(projection, view);

    glUseProgram(0);

    // render the gui
    CEGUI::System::getSingleton().renderAllGUIContexts();

    // swap the render buffers
    SDL_GL_SwapWindow(window);
                           
}

bool initialize(bool errorChecks, const char* fileName, float scaleFactor, float terrainScale)
{
    //--Geometry done

    //Shader Sources
    // Load vertex and fragment shaders
    ShaderLoader shaderLoader;
    GLint frag, vert, fragShaderGrayscale, vertShaderGrayscale;
    frag = shaderLoader.loadShaderFromFile(GL_FRAGMENT_SHADER, "../bin/shaders/fragShader.fs");
    vert = shaderLoader.loadShaderFromFile(GL_VERTEX_SHADER, "../bin/shaders/vertShader.vs");

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

    // create gray scale shader
    fragShaderGrayscale = shaderLoader.loadShaderFromFile(GL_FRAGMENT_SHADER, "../bin/shaders/fragShaderGrayscale.fs");
    vertShaderGrayscale = shaderLoader.loadShaderFromFile(GL_VERTEX_SHADER, "../bin/shaders/vertShaderGrayscale.vs");

    grayscaleProgram = glCreateProgram();
    glAttachShader(grayscaleProgram, vertShaderGrayscale);
    glAttachShader(grayscaleProgram, fragShaderGrayscale);
    glLinkProgram(grayscaleProgram);
    std::cout << grayscaleProgram << std::endl;
    //check if everything linked ok
    glGetProgramiv(grayscaleProgram, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] THE SHADER PROGRAM GRAYSCALE FAILED TO LINK" << std::endl;
        return false;
    }

    // Initialize basic geometry and shaders for this example
    // get terrain texture
    terrain = new Terrain(grayscaleProgram);
    terrain->setDataZone("tl2p5mask.ipw.tif", program);
    terrain->loadHeightMap(fileName, scaleFactor, terrainScale);
    terrain->setDataZoneData("isnobaloutput/em.1000.tif", false);

    //terrain2 = new Terrain(program);
    //terrain2->loadHeightMap(fileName, scaleFactor, terrainScale);

    colorMap = new Texture(GL_TEXTURE_1D, "colorMap.png");
    colorMap->create();

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
    // Clean up, Clean up
    delete terrain;
    delete terrain2;
    delete colorMap;

    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry);
    //GDALClose( (GDALDatasetH) poDataset );

	// Clean up SDL
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}