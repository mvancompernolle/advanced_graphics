#include "Graphics.hpp"

#include <iostream>

#include <ogrsf_frmts.h>
#include <ogr_spatialref.h>
#include <ogr_srs_api.h>
#include <list>
#include <random>

#include "gdal_priv.h"
#include "cpl_conv.h"

#include "Engine.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "EntityManager.hpp"
#include "TerrainBorder.hpp"
#include "Fireworks.hpp"
#include "Explosion.hpp"
#include "Grass.hpp"
#include "Water.hpp"
#include "LightingManager.hpp"
#include "SkyBox.hpp"
#include "Enemy.hpp"

using namespace Vancom;

Graphics::Graphics(Engine *engine) : engine(engine){

    // initialize light angle for direction lights
    isRaining = true;

	camera = new Camera(engine);
    windUpdateTime = 0.1f;
    windDT = 0.0f;
    windStrength = 5.0f;
}

Graphics::~Graphics(){

	if(camera)
		delete camera;
}

void Graphics::init(){

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// create SDL window
    window = SDL_CreateWindow("Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP);

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
    glEnable(GL_DEPTH_CLAMP);
    glDepthFunc(GL_LEQUAL);
    glLineWidth(5.0f);

    /*glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);*/

    updateView();
    windowResized();

    // initialize programs
    if(!defaultProgram.init())
        std::cout << "defaultProgram failed to init" << std::endl;

    if(!selectionProgram.init())
        std::cout << "selectionProgram failed to init" << std::endl;
    selectionTexture.init(width, height);

    if(!silhouetteProgram.init())
        std::cout << "silhouetteProgram failed to init" << std::endl;

    if(!guiProgram.init())
        std::cout << "guiProgram failed to init" << std::endl;

    if(!geometryProgram.init())
        std::cout << "geometryProgram failed to init" << std::endl;
    geometryProgram.enable();
    geometryProgram.setColorTextureUnit(0);

    if(!stencilProgram.init())
        std::cout << "stencilProgram failed to init" << std::endl;

    // deffered rendering components
    if(!dirLightProgram.init())
        std::cout << "dirLightProgram failed to init" << std::endl;
    dirLightProgram.enable();
    dirLightProgram.setPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    dirLightProgram.setColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    dirLightProgram.setNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    dirLightProgram.setSpecularTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
    dirLightProgram.setScreenSize(width, height);

    if(!pointLightProgram.init())
        std::cout << "pointLightProgram failed to init" << std::endl;
    pointLightProgram.enable();
    pointLightProgram.setPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    pointLightProgram.setColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    pointLightProgram.setNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    pointLightProgram.setSpecularTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
    pointLightProgram.setScreenSize(width, height);

    if(!spotLightProgram.init())
        std::cout << "spotLightProgram failed to init" << std::endl;
    spotLightProgram.enable();
    spotLightProgram.setPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    spotLightProgram.setColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    spotLightProgram.setNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    spotLightProgram.setSpecularTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
    spotLightProgram.setScreenSize(width, height);

    // initialize gbuffer, dirLight quad, and point light sphere  
    buffer.init(width, height);

    dirLightRenderQuad = new Model(engine);
    dirLightRenderQuad->init("../assets/models/quad.obj", false);
    pointLightRenderSphere = new Model(engine);
    pointLightRenderSphere->init("../assets/models/sphere.obj", false);

    // initialize shadow program
    if(!shadowProgram.init())
        std::cout << "shadowProgram failed to init" << std::endl;
}

void Graphics::tick(float dt){

    // update the camera and view
    updateCamera();
    updateView();

    // update wind direction
    windDT += dt;
    if(windDT > windUpdateTime){
        std::random_device rd;
        std::default_random_engine rand(rd());
        std::uniform_real_distribution<float> distr(-windStrength, windStrength);
        windDir = windDir * glm::vec3(.95, .95, .95) + glm::vec3(distr(rand), distr(rand), distr(rand)) * glm::vec3(.05, .05, .05);   
        windDT = 0;
    } 
}

void Graphics::render(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // render selectable entities
    selectionTexture.enableWriting();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    selectionProgram.enable();
    for(Entity* entity : engine->entityManager->enemyEntities){
        selectionProgram.setObjectIndex(entity->id);
        selectionProgram.setMVP(projection * view * entity->getModel());
        entity->render();
    }

    selectionTexture.disableWriting();
    // check to see if object is in middle of screen
    SelectionTexture::PixelInfo pixel = selectionTexture.readPixel(width/2, height/2);

    buffer.startFrame();
    geometryPassDS();

    glEnable(GL_STENCIL_TEST);
    shadowVolumePass();

    // Draw only if the corresponding stencil value is zero
    glStencilFunc(GL_EQUAL, 0x0, 0xFF);

    // prevent update to the stencil buffer
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);

    directionalLightPassDS();
    glDisable(GL_STENCIL_TEST);

    pointLightPassDS();
    spotLightPassDS();
    glDisable(GL_BLEND);

    // render border of selected
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    bool found = false;
    silhouetteProgram.enable();
    for(Enemy* entity : engine->entityManager->enemyEntities){
        if(entity->id != 0 && entity->id == (unsigned int)pixel.objectId && entity->updating){
            // if entity not already in selected add it
            for(Entity *ent : engine->input->selected){
                if(ent->id == entity->id){
                    found = true;
                    break;
                }
            }
            if(!found){
                engine->input->selected.push_back(entity);
                //std::cout << engine->input->selected.size() << std::endl;
            }

            break;
        }
    }


    // render outline of selected entities
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

    // render sky box
    engine->entityManager->skyBox->render(projection, view);

    buffer.bindForFinalPass();
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	SDL_GL_SwapWindow(window);
}

void Graphics::depthPass(){

    glDrawBuffer(GL_NONE);
    stencilProgram.enable();


}

void Graphics::shadowVolumePass(){

    buffer.bindForStencilPass();
    glDepthMask(GL_FALSE);
    glEnable(GL_DEPTH_CLAMP);
    glDisable(GL_CULL_FACE);

    // enable stencil test but have it always succeed
    glStencilFunc(GL_ALWAYS, 0, 0xff);

    // set the stencil test per the depth fail algorithm
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP); 

    // render an object
    for(int i=1; i<engine->entityManager->enemyEntities.size(); i++){
        Entity* entity = engine->entityManager->enemyEntities[i];
        shadowProgram.enable();
        shadowProgram.setLightPosition(engine->lightingManager->dirLight.direction * glm::vec3(-100000, -100000, -100000));
        shadowProgram.setVP(projection * view);
        shadowProgram.setModel(entity->getModel());
        entity->render();
    }

    glDisable(GL_DEPTH_CLAMP);
}

void Graphics::geometryPassDS(){

    // geometry pass
    geometryProgram.enable();
    buffer.bindForGeometryPass();
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // render terrain
    for(Entity* entity : engine->entityManager->terrainEntities){
        geometryProgram.setMVP(projection * view * entity->getModel());
        geometryProgram.setModelPos(entity->getModel());
        geometryProgram.setSpecularPower(entity->specularPower);
        geometryProgram.setSpecularIntensity(entity->specularIntensity);
        entity->render();
    }

    // render enemies
    engine->entityManager->enemyTexture->bind(GL_TEXTURE0);
    for(Entity* entity : engine->entityManager->enemyEntities){
        geometryProgram.setMVP(projection * view * entity->getModel());
        geometryProgram.setModelPos(entity->getModel());
        geometryProgram.setSpecularPower(entity->specularPower);
        geometryProgram.setSpecularIntensity(entity->specularIntensity);
        entity->render();
    }

    engine->entityManager->grass->render(projection, view);
    engine->entityManager->water->render(projection, view);

    glDepthMask(GL_FALSE);
}

void Graphics::pointLightPassDS(){

    // point lights
    for(PointLight pLight : engine->lightingManager->pointLights){

        // determine scale of point light
        float scale = calcPointLightSphere(pLight);
        glm::mat4 matrix = glm::translate(pointLightRenderSphere->getModel(), pLight.pos);

        // stencil pass
        stencilProgram.enable();
        buffer.bindForStencilPass();

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glClear(GL_STENCIL_BUFFER_BIT);

        glStencilFunc(GL_ALWAYS, 0, 0);
        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
        stencilProgram.setMVP(projection * view * glm::scale(matrix, glm::vec3(scale, scale, scale)));
        pointLightRenderSphere->render();

        // point light pass
        pointLightProgram.enable();
        buffer.bindForLightPass();

        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        pointLightProgram.setPointLight(pLight);
        pointLightProgram.setCameraPosition(camera->getPos());
        pointLightProgram.setMVP(projection * view * glm::scale(matrix, glm::vec3(scale, scale, scale)));
        pointLightRenderSphere->render();

        glCullFace(GL_BACK);
        glDisable(GL_BLEND);    
    }
    glDisable(GL_CULL_FACE);
}

void Graphics::directionalLightPassDS(){

    // directional light
    dirLightProgram.enable();
    buffer.bindForLightPass();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    dirLightProgram.setDirLight(engine->lightingManager->dirLight);
    dirLightProgram.setCameraPosition(camera->getPos());
    dirLightProgram.setMVP(dirLightRenderQuad->getModel());
    dirLightRenderQuad->render();
}

void Graphics::spotLightPassDS(){

    // spot lights
    spotLightProgram.enable();
    buffer.bindForLightPass();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    for(SpotLight spotLight : engine->lightingManager->spotLights){
        spotLightProgram.setSpotLight(spotLight);
        spotLightProgram.setCameraPosition(camera->getPos());
        spotLightProgram.setMVP(dirLightRenderQuad->getModel());
        dirLightRenderQuad->render();
    }
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
    SDL_GetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	projection = glm::perspective(45.0f, float(width) / float(height), 0.01f, 100000.0f);
}

void Graphics::setClearColor(glm::vec3 color){

	glClearColor(color.x, color.y, color.z, 1);
}

float Graphics::calcPointLightSphere(const PointLight& light) const{

    float maxColor = fmax(fmax(light.color.x, light.color.y), light.color.z);


    float returnVal = sqrtf((maxColor * 256 * light.diffuseIntensity / light.atten.exp) + light.atten.constant); // exp and constant
    //float returnVal = maxColor * (1/light.atten.linear) * 256; // linear
    /*float returnVal = (-light.atten.linear + sqrtf(light.atten.linear * light.atten.linear - 4 * light.atten.exp 
        * (light.atten.constant - 256 * maxColor * light.diffuseIntensity))) / 2 * light.atten.exp;   */
    //std::cout << maxColor << std::endl;
    return returnVal;
}