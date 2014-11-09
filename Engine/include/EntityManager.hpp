#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <vector>

namespace Vancom{

class Entity;
class Engine;
class TerrainBorder;

class EntityManager {

public:

	// public functions
	EntityManager(Engine *engine);
	~EntityManager();

	void init();
	void tick(float dt);
	void stop();
	unsigned int assignId();

	// public variables
	std::vector<Entity*> entities;
	std::vector<Entity*> defaultEntities;
	TerrainBorder *border;

private:

	// private variables
	Engine *engine;
	unsigned int nextId;

};

} // end namespace Vancom

#endif // END ENTITYMANAGER_HPP