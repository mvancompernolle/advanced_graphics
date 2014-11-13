#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <vector>

namespace Vancom{

class Entity;
class Engine;
class TerrainBorder;
class Fireworks;

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
	std::vector<Entity*> guiEntities;
	std::vector<Fireworks*> explosions;
	TerrainBorder *border;

private:

	// private variables
	Engine *engine;
	unsigned int nextId;

};

} // end namespace Vancom

#endif // END ENTITYMANAGER_HPP