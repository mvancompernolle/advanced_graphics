#include "LightingManager.hpp"

#include <iostream>

#include "Engine.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"

using namespace Vancom;

LightingManager::LightingManager(Engine *engine) : engine(engine){

	dirLightAngle = -0.75f;
	increaseLightAngle();
}

LightingManager::~LightingManager(){

}

void LightingManager::init(){

	// init directional light
	dirLight.diffuseIntensity = 1.0f;

	// init spot lights

	// init spot lights
	// first spot lights is camera flashlight
	SpotLight cameraLight;
	cameraLight.diffuseIntensity = 2.0f;
	cameraLight.atten.constant = 1;
	cameraLight.atten.linear = .005;
	cameraLight.pos = engine->graphics->camera->getPos();
	cameraLight.direction = engine->graphics->camera->getCameraDirection();
	cameraLight.cutoff = .99;
	spotLights.push_back(cameraLight);

	// create a second spot light
	SpotLight spotLight2 = cameraLight;
	spotLight2.pos = glm::vec3(0, 400, 0);
	spotLight2.direction = glm::vec3(0, -1, 0);
	spotLights.push_back(spotLight2);

	// create a point light
	PointLight pLight;
	pLight.diffuseIntensity = 1.0f;
	pLight.atten.constant = 1;
	pLight.atten.linear = .05;
	pLight.atten.exp = .01;
	pLight.pos = glm::vec3(200, 100, 200);
	pointLights.push_back(pLight);

	// create a point light
	PointLight pLight2 = pLight;
	pLight2.pos = glm::vec3(200, 100, 0);

}

void LightingManager::tick(float dt){

	// tick all of the lights in the world
	// tick the cameraLight
	spotLights[0].pos = engine->graphics->camera->getPos();
	spotLights[0].direction = engine->graphics->camera->getCameraDirection();
}

void LightingManager::stop(){

}

void LightingManager::setDirectionalLight(DirectionalLight dirLight){

	this->dirLight = dirLight; 
}

void LightingManager::addSpotLight(SpotLight spotLight){

	spotLights.push_back(spotLight);
}

void LightingManager::addPointLight(PointLight pointLight){

	pointLights.push_back(pointLight);
}

void LightingManager::increaseLightAngle(){


    dirLightAngle -= 0.01;

    float y = -cos(dirLightAngle + 1.57);
    float x = -sin(dirLightAngle + 1.57);
    dirLight.direction = glm::vec3(x, y, 0);
}

void LightingManager::decreaseLightAngle(){

    dirLightAngle += 0.01;

    float y = -cos(dirLightAngle + 1.57);
    float x = -sin(dirLightAngle + 1.57);
    dirLight.direction = glm::vec3(x, y, 0);
}