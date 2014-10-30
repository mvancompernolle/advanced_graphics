#include "EntityManager.hpp"

#include "Engine.hpp"
#include "Cube.hpp"
#include "Entity.hpp"
#include "Terrain.hpp"

using namespace Vancom;

EntityManager::EntityManager(Engine *engine) : engine(engine){
}

EntityManager::~EntityManager(){

}

void EntityManager::init(){

	// add a cube
	Cube* cube = new Cube();
	cube->init();
	entities.push_back(cube);

	// add the terrain
	Terrain* terrain = new Terrain(engine, "../assets/DCEWsqrExtent.tif");
	terrain->init();
	terrain->setTexture(GL_TEXTURE_2D, "../assets/terrain_dirt.jpg");
	entities.push_back(terrain);
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