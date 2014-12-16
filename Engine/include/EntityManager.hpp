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
class Grass;
class Water;
class Texture;
class Enemy;
class LightningBullet;
class Fireworks;
class SplashScreen;
class ColoredGUIRect;

class EntityManager {

public:

	// public functions
	EntityManager(Engine *engine);
	~EntityManager();

	void init();
	void tick(float dt);
	void stop();
	void createExplosion(glm::vec3 pos);
	void createBullet(glm::vec3 pos, glm::vec3 dir);

	// public variables
	std::vector<Entity*> terrainEntities;
	std::vector<Enemy*> enemyEntities;
	std::vector<Entity*> guiEntities;
	std::vector<ColoredGUIRect*> guiEntities2;
	std::vector<SplashScreen*> splashScreens;
	std::vector<Explosion*> explosions;
	std::vector<LightningBullet*> bullets;
	std::vector<Fireworks*> fireworks;
	Texture* enemyTexture, *bulletTexture;
	TerrainBorder *border;
	SkyBox* skyBox;
	Grass* grass;
	Water* water;

	float minX, maxX, minZ, maxZ;

private:

	// private functions
	unsigned int assignId();
	void addEnemy();

	// private variables
	Engine *engine;
	unsigned int nextId;

};

} // end namespace Vancom

#endif // END ENTITYMANAGER_HPP