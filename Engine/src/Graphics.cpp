#include "Graphics.hpp"

#include <iostream>

#include <ogrsf_frmts.h>
#include <ogr_spatialref.h>
#include <ogr_srs_api.h>

#include "gdal_priv.h"
#include "cpl_conv.h"

#include "Engine.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "EntityManager.hpp"
#include "TerrainBorder.hpp"
#include "Explosion.hpp"
#include "LightingManager.hpp"

using namespace Vancom;

Graphics::Graphics(Engine *engine) : engine(engine){

    // initialize light angle for direction lights
    isRaining = true;

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
    glClearColor(0.0, 0.0, 0.0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glLineWidth(5.0f);

    updateView();
    windowResized();

    // initialize programs
    if(!defaultProgram.init())
        std::cout << "defaultProgram failed to init" << std::endl;

    if(!selectionProgram.init())
        std::cout << "selectionProgram failed to init" << std::endl;
    selectionTexture.init(1800, 1000);

    if(!silhouetteProgram.init())
        std::cout << "silhouetteProgram failed to init" << std::endl;

    if(!guiProgram.init())
        std::cout << "guiProgram failed to init" << std::endl;

    if(!geometryProgram.init())
        std::cout << "geometryProgram failed to init" << std::endl;
    geometryProgram.enable();
    geometryProgram.setColorTextureUnit(0);

    // deffered rendering components
    if(!dirLightProgram.init())
        std::cout << "dirLightProgram failed to init" << std::endl;
    dirLightProgram.enable();
    dirLightProgram.setPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    dirLightProgram.setColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    dirLightProgram.setNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    dirLightProgram.setSpecularTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
    dirLightProgram.setScreenSize(w, h);

    if(!pointLightProgram.init())
        std::cout << "pointLightProgram failed to init" << std::endl;
    pointLightProgram.enable();
    pointLightProgram.setPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    pointLightProgram.setColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    pointLightProgram.setNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    pointLightProgram.setSpecularTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
    pointLightProgram.setScreenSize(w, h);

    if(!spotLightProgram.init())
        std::cout << "spotLightProgram failed to init" << std::endl;
    spotLightProgram.enable();
    spotLightProgram.setPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    spotLightProgram.setColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    spotLightProgram.setNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    spotLightProgram.setSpecularTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
    spotLightProgram.setScreenSize(w, h);

    // initialize gbuffer and dirLight quad  
    buffer.init(w, h);
    dirLightRenderQuad = new Model();
    dirLightRenderQuad->init("../assets/models/quad.obj");
}

void Graphics::tick(float dt){
}

void Graphics::render(){

	// update the camera and view
	updateCamera();
	updateView();

    /// render selectable entities
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

    // render entities using defferred shading
    // geometry pass
    geometryProgram.enable();
    buffer.bindForWriting();
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    for(Entity* entity : engine->entityManager->defaultEntities){
        geometryProgram.setMVP(projection * view * entity->getModel());
        geometryProgram.setModelPos(entity->getModel());
        geometryProgram.setSpecularPower(entity->specularPower);
        geometryProgram.setSpecularIntensity(entity->specularIntensity);
        entity->render();
    }

    // render border of selected
    silhouetteProgram.enable();
    for(Entity* entity : engine->entityManager->defaultEntities){
        if(entity->id == (unsigned int)pixel.objectId){
            silhouetteProgram.setMVP(projection * view * entity->getModel());
            silhouetteProgram.setModelPos(entity->getModel());    
            silhouetteProgram.setCameraPosition(camera->getPos());
            entity->render();
            break;
        }
    }

    for(Entity* entity : engine->input->selected){
        silhouetteProgram.setMVP(projection * view * entity->getModel());
        silhouetteProgram.setModelPos(entity->getModel());    
        silhouetteProgram.setCameraPosition(camera->getPos());
        entity->render();        
    }

    // render explosions
    for(Explosion *explosion : engine->entityManager->explosions){
        explosion->render(projection, view, camera->getPos());
    }

    // render terrain border
    engine->entityManager->border->render(projection, view);

    // render gui elements
    guiProgram.enable();
    guiProgram.setSampler(0);
    for(Entity* entity : engine->entityManager->guiEntities){
        entity->render();
    }
    
    // light passes
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    buffer.bindForReading();
    glClear(GL_COLOR_BUFFER_BIT);

    // directional light
    dirLightProgram.enable();
    dirLightProgram.setDirLight(engine->lightingManager->dirLight);
    dirLightProgram.setCameraPosition(camera->getPos());
    dirLightProgram.setMVP(dirLightRenderQuad->getModel());
    dirLightRenderQuad->render();

    // point lights
    pointLightProgram.enable();
    for(PointLight pLight : engine->lightingManager->pointLights){
        pointLightProgram.setPointLight(pLight);
        dirLightProgram.setCameraPosition(camera->getPos());
        pointLightProgram.setMVP(dirLightRenderQuad->getModel());
        dirLightRenderQuad->render();    
    }

    // spot lights
    spotLightProgram.enable();
    for(SpotLight spotLight : engine->lightingManager->spotLights){
        spotLightProgram.setSpotLight(spotLight);
        dirLightProgram.setCameraPosition(camera->getPos());
        spotLightProgram.setMVP(dirLightRenderQuad->getModel());
        dirLightRenderQuad->render();
    }

    // bind default fbo for other rendering
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

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
