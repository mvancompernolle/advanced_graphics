#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <vector>

namespace Vancom{

class Entity;
class Engine;

class EntityManager {

public:

	// public functions
	EntityManager(Engine *engine);
	~EntityManager();

	void init();
	void tick(float dt);
	void stop();

	// public variables
	std::vector<Entity*> entities;

private:

	// private variables
	Engine *engine;

};

} // end namespace Vancom

#endif // END ENTITYMANAGER_HPP