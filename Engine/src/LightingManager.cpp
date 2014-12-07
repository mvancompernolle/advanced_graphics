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
	cameraLight.color = glm::vec3(1.0, 1.0, 1.0);
	cameraLight.diffuseIntensity = 1.0f;
	cameraLight.ambientIntensity = 1.0f;
	cameraLight.atten.constant = 1;
	cameraLight.atten.linear = .005;
	cameraLight.pos = engine->graphics->camera->getPos();
	cameraLight.direction = engine->graphics->camera->getCameraDirection();
	cameraLight.cutoff = .97;
	spotLights.push_back(cameraLight);

	/*// create a second spot light
	SpotLight spotLight2 = cameraLight;
	spotLight2.pos = glm::vec3(0, 400, 0);
	spotLight2.direction = glm::vec3(0, -1, 0);
	spotLights.push_back(spotLight2);*/

	// create a point light
	/*PointLight pLight;
	pLight.ambientIntensity = 0.5f;
	pLight.diffuseIntensity = 1.0f;
	pLight.color = glm::vec3(1.0, 1.0, 1.0);
	pLight.atten.constant = 1.0f;
	pLight.atten.linear = 0.00f;
	pLight.atten.exp = 0.001f;
	pLight.pos = glm::vec3(0, 100, 0);
	pointLights.push_back(pLight);*/

	/*// create a point light
	PointLight pLight2 = pLight;
	pLight2.pos = glm::vec3(200, 100, 0);
	pointLights.push_back(pLight2);*/
}

void LightingManager::tick(float dt){

	// tick all of the lights in the world
	// tick the cameraLight
	spotLights[0].pos = engine->graphics->camera->getPos();
	spotLights[0].direction = engine->graphics->camera->getCameraDirection();

	int counter = 0;
	std::vector<PointLight>::iterator it;
	for(it = pointLights.begin(); it != pointLights.end();){
		if(it->timeToLive > 0){
			it->dt += dt;

			// set intensity fo the light
			if(it->dt < it->timeToLive/2)
				it->diffuseIntensity = 1.0f;
			else
				//std::cout << (it->timeToLive - it->dt)/(it->timeToLive/2 << " " << it->timeToLive << " " << it->dt << std::endl;
				it->diffuseIntensity = (it->timeToLive - it->dt)/(it->timeToLive/2);

			if(it->dt > it->timeToLive)
				pointLights.erase(it);
			else
				it++;

		}
		else
			it++;
		
		counter++;
	}		
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
    dirLight.direction = glm::normalize(glm::vec3(x, y, 0));
}

void LightingManager::decreaseLightAngle(){

    dirLightAngle += 0.01;

    float y = -cos(dirLightAngle + 1.57);
    float x = -sin(dirLightAngle + 1.57);
    dirLight.direction = glm::normalize(glm::vec3(x, y, 0));
}

void LightingManager::addTempPointLight(glm::vec3 pos, glm::vec3 color, float lifeTime){

	PointLight pLight;
	pLight.diffuseIntensity = 1.0f;
	pLight.color = color;
	pLight.atten.constant = 1.0f;
	pLight.atten.linear = 0.00f;
	pLight.atten.exp = 0.00001f;
	pLight.timeToLive = lifeTime;
	pLight.pos = pos;
	pointLights.push_back(pLight);	
}