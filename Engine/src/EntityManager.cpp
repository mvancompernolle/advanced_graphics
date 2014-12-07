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
#include "Water.hpp"
#include "Enemy.hpp"

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
	terrainEntities.push_back(terrain);
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
	for(int i=0; i<20; i++){
		// add a ball
		addEnemy();	
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
	crossHair->init("../assets/models/crosshair.png", .05 / ratio, .05);
	guiEntities.push_back(crossHair);
	entities.push_back(crossHair);

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

	// add water
	water = new Water(engine, height * 50, height * 50);
	water->init();
	water->setTexture(GL_TEXTURE_2D, "../assets/reflection.jpg");

	// load enemy texture
	enemyTexture = new Texture(GL_TEXTURE_2D, "../assets/models/silver.jpg");

	if(!enemyTexture->create()){
		std::cerr << "There was an error creating the enemy texture" << std::endl;
	}
}

void EntityManager::tick(float dt){

	// tick all of the entities in the world
	for(Entity *entity : entities)
		entity->tick(dt);

	// tick explosions
	for(Explosion *explosion : explosions)
		explosion->tick(dt);

	// loop through entities and delete any that have fallen into the water
	int destroyHeight = -100;
	std::vector<Entity*>::iterator it;
	for(it = entities.begin(); it != entities.end();){
		if((*it)->getModel()[3][1] < destroyHeight){
			entities.erase(it);
		}
		else{
			it++;
		}
	}		
	std::vector<Enemy*>::iterator itEnemy;
	for(itEnemy = enemyEntities.begin(); itEnemy != enemyEntities.end();){
		if((*itEnemy)->getModel()[3][1] < destroyHeight){
			enemyEntities.erase(itEnemy);
		}
		else{
			itEnemy++;
		}
	}	
	std::list<Entity*>::iterator it2;
	for(it2 = engine->input->selected.begin(); it2 != engine->input->selected.end();){
		if((*it2)->getModel()[3][1] < destroyHeight){
			engine->input->selected.erase(it2);
			delete (*it2);
		}
		else{
			it2++;
		}
	}	

	// add another enemy if less than 20
	if(enemyEntities.size() < 20){
		addEnemy();
	}

	// update water
	water->tick(dt);
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
	
    Explosion* explosion = new Explosion(this);
    explosion->init(pos);
    explosions.push_back(explosion);
}

void EntityManager::addEnemy(){

    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distZ(minZ, maxZ);
    std::uniform_real_distribution<float> distY(150, 500);
    std::uniform_real_distribution<float> distSize(10, 50);
	// add a ball
	Enemy* ball = new Enemy(engine, glm::vec3(distX(gen), distY(gen), distZ(gen)), distSize(gen), 32, 1);
	ball->init("../assets/models/ballSml.obj");
	ball->id = assignId();
	enemyEntities.push_back(ball);
	entities.push_back(ball);		
}