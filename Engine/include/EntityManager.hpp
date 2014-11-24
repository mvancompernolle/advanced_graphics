#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class Entity;
class Engine;
class TerrainBorder;
class Explosion;
class SkyBox;

class EntityManager {

public:

	// public functions
	EntityManager(Engine *engine);
	~EntityManager();

	void init();
	void tick(float dt);
	void stop();
	void createExplosion(glm::vec3 pos);

	// public variables
	std::vector<Entity*> entities;
	std::vector<Entity*> defaultEntities;
	std::vector<Entity*> guiEntities;
	std::vector<Explosion*> explosions;
	TerrainBorder *border;
	SkyBox* skyBox;

	float minX, maxX, minZ, maxZ;

private:

	// private functions
	unsigned int assignId();

	// private variables
	Engine *engine;
	unsigned int nextId;

};

} // end namespace Vancom

#endif // END ENTITYMANAGER_HPP