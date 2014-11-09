#include "EntityManager.hpp"

#include <iostream>

#include "Engine.hpp"
#include "Model.hpp"
#include "Entity.hpp"
#include "Terrain.hpp"
#include "TerrainBorder.hpp"

using namespace Vancom;

EntityManager::EntityManager(Engine *engine) : engine(engine){

	nextId = 1;
}

EntityManager::~EntityManager(){

}

void EntityManager::init(){

	// add a cube
	Model* ball = new Model(glm::vec3(0, 100, 0), 10);
	ball->init("../assets/models/ballSml.obj");
	ball->id = assignId();
	defaultEntities.push_back(ball);
	entities.push_back(ball);

	// add labyrinth
	Model* lab = new Model(glm::vec3(30, 100, 0), 1);
	lab->init("../assets/models/LabyrinthMaze.obj");
	lab->id = assignId();
	defaultEntities.push_back(lab);
	entities.push_back(lab);

	// add a puck
	Model* puck = new Model(glm::vec3(50, 100, 0), 5);
	puck->init("../assets/models/puck.obj");
	puck->id = assignId();
	defaultEntities.push_back(puck);
	entities.push_back(puck);

	// add the terrain
	Terrain* terrain = new Terrain(engine, "../assets/DCEWsqrExtent.tif");
	terrain->init();
	terrain->setTexture(GL_TEXTURE_2D, "../assets/terrain_dirt.jpg");
	terrain->id = assignId();
	defaultEntities.push_back(terrain);
	entities.push_back(terrain);

	// add terrain border
	int width, height;
	border = new TerrainBorder(engine, 0.1f);
	terrain->getDimensions(width, height);
	border->init(100, glm::vec2(-width/2, height/2), glm::vec2(width/2, height/2), glm::vec2(-width/2, -height/2), 
		glm::vec2(width/2, -height/2));
	entities.push_back(border);
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