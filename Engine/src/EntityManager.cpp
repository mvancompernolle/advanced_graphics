#include "EntityManager.hpp"

#include <iostream>
#include <random>

#include "Engine.hpp"
#include "Input.hpp"
#include "Model.hpp"
#include "Entity.hpp"
#include "Terrain.hpp"
#include "TerrainBorder.hpp"
#include "CrossHair.hpp"
#include "Graphics.hpp"
#include "Explosion.hpp"
#include "Grass.hpp"
#include "SkyBox.hpp"

using namespace Vancom;

EntityManager::EntityManager(Engine *engine) : engine(engine){

	nextId = 1;
}

EntityManager::~EntityManager(){

}

void EntityManager::init(){

	// add the terrain
	Terrain* terrain = new Terrain(engine, "../assets/DCEWsqrExtent.tif");
	terrain->init();
	terrain->setTexture(GL_TEXTURE_2D, "../assets/mud.png");
	defaultEntities.push_back(terrain);
	entities.push_back(terrain);
	int width, height;
	terrain->getDimensions(width, height);

	// add grass on the terrain
	grass = new Grass(engine, terrain->getModel());
	grass->init();
	grass->generateBlades(terrain->getHeightMap(), width, height);

	// set minimum and maximum values 
	minX = -width/2;
	maxX = width/2;
	minZ = -height/2;
	maxZ = height/2;

	// randomly generate 10 balls
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distZ(minZ, maxZ);
    std::uniform_real_distribution<float> distSize(10, 50);
	for(int i=0; i<10; i++){
		// add a ball
		Model* ball = new Model(this, glm::vec3(distX(gen), i*30 + 180, distZ(gen)), distSize(gen), 32, 1);
		ball->init("../assets/models/ballSml.obj");
		ball->id = assignId();
		defaultEntities.push_back(ball);
		entities.push_back(ball);		
	}

	// add terrain border
	border = new TerrainBorder(engine, 0.1f);
	border->init(100, glm::vec2(-width/2, height/2), glm::vec2(width/2, height/2), glm::vec2(-width/2, -height/2), 
		glm::vec2(width/2, -height/2));
	entities.push_back(border);

	// add a crosshair
	float ratio = (float) engine->graphics->width/engine->graphics->height;
	std::cout << ratio << std::endl;
	CrossHair *crossHair = new CrossHair(glm::vec3(0, 0, 0));
	crossHair->init("../assets/models/crosshair.png", .03 / ratio, .03);
	guiEntities.push_back(crossHair);
	entities.push_back(crossHair);

	// add an explosion
	Explosion* explosion = new Explosion(engine->entityManager);
	explosion->init(glm::vec3(0, 100, 0));
	explosions.push_back(explosion);

	// add skybox
	skyBox = new SkyBox(engine);
	if(!skyBox->init("../assets/models",
			"calm_right.jpg",
			"calm_left.jpg",
			"calm_top.jpg",
			"calm_top.jpg",
			"calm_front.jpg",
			"calm_back.jpg")){
		std::cout << "failed to init skybox" << std::endl;
	}
}

void EntityManager::tick(float dt){

	// tick all of the entities in the world
	for(Entity *entity : entities)
		entity->tick(dt);

	// tick explosions
	for(Explosion *explosion : explosions)
		explosion->tick(dt);
}

void EntityManager::stop(){

	// delete all entities
	for(Entity *entity : entities)
		delete entity;
}

unsigned int EntityManager::assignId(){

	return nextId++;
}

void EntityManager::createExplosion(glm::vec3 pos){
	std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
    Explosion* explosion = new Explosion(this);
    explosion->init(pos);
    explosions.push_back(explosion);
}