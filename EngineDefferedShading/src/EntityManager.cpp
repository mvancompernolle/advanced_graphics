#include "EntityManager.hpp"

#include <iostream>

#include "Engine.hpp"
#include "Cube.hpp"
#include "Entity.hpp"
#include "Terrain.hpp"
#include "TerrainBorder.hpp"

using namespace Vancom;

EntityManager::EntityManager(Engine *engine) : engine(engine){
}

EntityManager::~EntityManager(){

}

void EntityManager::init(){

	// add the terrain
	Terrain* terrain = new Terrain(engine, "../assets/DCEWsqrExtent.tif");
	terrain->init();
	terrain->setTexture(GL_TEXTURE_2D, "../assets/terrain_dirt.jpg");
	entities.push_back(terrain);

	/*// add terrain border
	int width, height;
	TerrainBorder* border = new TerrainBorder(engine, 0.1f);
	terrain->getDimensions(width, height);
	border->init(100, glm::vec2(-width/2, height/2), glm::vec2(width/2, height/2), glm::vec2(-width/2, -height/2), 
		glm::vec2(width/2, -height/2));
	entities.push_back(border);

	// add a cube
	Cube* cube = new Cube();
	cube->init();
	entities.push_back(cube);*/
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