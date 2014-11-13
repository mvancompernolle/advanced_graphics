#include "EntityManager.hpp"

#include <iostream>

#include "Engine.hpp"
#include "Input.hpp"
#include "Model.hpp"
#include "Entity.hpp"
#include "Terrain.hpp"
#include "TerrainBorder.hpp"
#include "CrossHair.hpp"
#include "Graphics.hpp"
#include "Explosion.hpp"

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
	terrain->setTexture(GL_TEXTURE_2D, "../assets/terrain_dirt.jpg");
	defaultEntities.push_back(terrain);
	entities.push_back(terrain);

	// add a ball
	Model* ball = new Model(glm::vec3(0, 100, 0), 10, 1, 200);
	ball->init("../assets/models/ballSml.obj");
	ball->id = assignId();
	defaultEntities.push_back(ball);
	entities.push_back(ball);

	// add a ball
	Model* ball1 = new Model(glm::vec3(40, 100, 0), 15, 1, 100);
	ball1->init("../assets/models/ballSml.obj");
	ball1->id = assignId();
	defaultEntities.push_back(ball1);
	entities.push_back(ball1);
	engine->input->selected.push_back(ball);

	// add a ball
	Model* ball2 = new Model(glm::vec3(-40, 100, 0), 20, 1, 200);
	ball2->init("../assets/models/ballSml.obj");
	ball2->id = assignId();
	defaultEntities.push_back(ball2);
	entities.push_back(ball2);
	engine->input->selected.push_back(ball2);

	// add cube
	Model* cube = new Model(glm::vec3(-80, 100, 0), 3, .1, 50);
	cube->init("../assets/models/woodCube.obj");
	cube->id = assignId();
	defaultEntities.push_back(cube);
	entities.push_back(cube);

	// add a buddha
	Model* buddha = new Model(glm::vec3(80, 100, 0), 5, 50, 5);
	buddha->init("../assets/models/buddha.obj");
	buddha->id = assignId();
	defaultEntities.push_back(buddha);
	entities.push_back(buddha);

	// add terrain border
	int width, height;
	border = new TerrainBorder(engine, 0.1f);
	terrain->getDimensions(width, height);
	border->init(100, glm::vec2(-width/2, height/2), glm::vec2(width/2, height/2), glm::vec2(-width/2, -height/2), 
		glm::vec2(width/2, -height/2));
	entities.push_back(border);

	// add a crosshair
	engine->graphics->getWindowSize(width, height);
	float ratio = (float) width/height;
	std::cout << ratio << std::endl;
	CrossHair *crossHair = new CrossHair(glm::vec3(0, 0, 0));
	crossHair->init("../assets/models/crosshair.png", .03 / ratio, .03);
	guiEntities.push_back(crossHair);
	entities.push_back(crossHair);

	// add an explosion
	Explosion* explosion = new Explosion();
	explosion->init(glm::vec3(0, 100, 0));
	entities.push_back(explosion);
	explosions.push_back(explosion);
}

void EntityManager::tick(float dt){

	// tick all of the entities in the world
	for(Entity *entity : entities)
		entity->tick(dt);
}

void EntityManager::stop(){

	// delete all entities
	for(Entity *entity : entities)
		delete entity;
}

unsigned int EntityManager::assignId(){

	return nextId++;
}