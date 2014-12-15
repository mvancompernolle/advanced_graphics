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
#include "LightningBullet.hpp"
#include "Fireworks.hpp"
#include "SplashScreen.hpp"

using namespace Vancom;

EntityManager::EntityManager(Engine *engine) : engine(engine){

	nextId = 1;
}

EntityManager::~EntityManager(){

}

void EntityManager::init(){

	// add the terrain
	Terrain* terrain = new Terrain(engine, "../assets/DCEWsqrExtent.tif", 300, 1);
	terrain->init();
	terrain->setTexture(GL_TEXTURE_2D, "../assets/mud.png");
	terrainEntities.push_back(terrain);
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
	for(int i=0; i<10; i++){
		// add a ball
		addEnemy();	
	}

	// add terrain border
	border = new TerrainBorder(engine, 0.1f);
	border->init(100, glm::vec2(-width/2, height/2), glm::vec2(width/2, height/2), glm::vec2(-width/2, -height/2), 
		glm::vec2(width/2, -height/2));

	// add a crosshair
	float ratio = (float) engine->graphics->width/engine->graphics->height;
	std::cout << ratio << std::endl;
	CrossHair *crossHair = new CrossHair(glm::vec3(0, 0, 0));
	crossHair->init("../assets/models/crosshair.png", .05 / ratio, .05);
	guiEntities.push_back(crossHair);

	// setup splash screens
	SplashScreen* splashScreen = new SplashScreen();
	splashScreen->init("../assets/titleScreen.jpg", 1, 1);
	splashScreens.push_back(splashScreen);
	SplashScreen* splashScreen2 = new SplashScreen();
	splashScreen2->init("../assets/mud.png", 1, 1);
	splashScreens.push_back(splashScreen2);

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
	water = new Water(engine, height * 20, height * 20);
	water->init();
	water->setTexture(GL_TEXTURE_2D, "../assets/reflection.jpg");

	// add fireworks
	Fireworks* firework = new Fireworks();
	firework->init(glm::vec3(-5000, 0, -5000));
	fireworks.push_back(firework);
	Fireworks* firework2 = new Fireworks();
	firework2->init(glm::vec3(-5000, 0, -4500));
	fireworks.push_back(firework2);
	Fireworks* firework3 = new Fireworks();
	firework3->init(glm::vec3(-4500, 0, -5000));
	fireworks.push_back(firework3);

	// load enemy and bullet textures
	enemyTexture = new Texture(GL_TEXTURE_2D, "../assets/models/silver.jpg");
	bulletTexture = new Texture(GL_TEXTURE_2D, "../assets/energy_ball.jpg");

	if(!enemyTexture->create()){
		std::cerr << "There was an error creating the enemy texture" << std::endl;
	}
	if(!bulletTexture->create()){
		std::cerr << "There was an error creating the bullet texture" << std::endl;
	}
}

void EntityManager::tick(float dt){

	// apply damage to enemies
	for(LightningBullet* bullet: bullets){
		for(Enemy* enemy: enemyEntities){
			if(glm::distance(bullet->getPos(), enemy->getPos()) <= 250){
				enemy->health -= 1;
			}
		}
	}

	/*std::list<Enemy*>::iterator selectedIt;
	for(selectedIt = engine->input->selected.begin(); selectedIt != engine->input->selected.end();){
		if((*selectedIt) != NULL){
			if((*selectedIt)->health <= 0){

				engine->input->selected.erase(selectedIt);
			}
			else{
				selectedIt++;
			}
		}
	}*/

	// loop through entities and delete any that have fallen into the water
	int destroyHeight = -100;	
	std::vector<Enemy*>::iterator itEnemy;
	for(itEnemy = enemyEntities.begin(); itEnemy != enemyEntities.end();){
		if((*itEnemy)->getModel()[3][1] < destroyHeight){
			delete (*itEnemy);
			enemyEntities.erase(itEnemy);
		}
		else{
			itEnemy++;
		}
	}	

	// remove expired explosions
	std::vector<Explosion*>::iterator explosionIt;
	for(explosionIt = explosions.begin(); explosionIt != explosions.end();){
		if((*explosionIt)->timeElapsed > 3100){
			delete *explosionIt;
			explosions.erase(explosionIt);
		}
		else{
			explosionIt++;
		}
	}

	// remove expired bullets		
	std::vector<LightningBullet*>::iterator bulletIt;
	for(bulletIt = bullets.begin(); bulletIt != bullets.end();){
		if((*bulletIt)->timeElapsed > 5){
			delete *bulletIt;
			bullets.erase(bulletIt);
		}
		else{
			bulletIt++;
		}
	}

	// remove expired splash screens		
	std::vector<SplashScreen*>::iterator splashIt;
	for(splashIt = splashScreens.begin(); splashIt != splashScreens.end();){
		if((*splashIt)->transVal > 2){
			delete *splashIt;
			splashScreens.erase(splashIt);
		}
		else{
			splashIt++;
		}
	}

	// tick enemies
	for(Enemy *enemy : enemyEntities)
		enemy->tick(dt);

	// tick explosions
	for(Explosion *explosion : explosions)
		explosion->tick(dt);

	for(Fireworks *firework : fireworks)
		firework->tick(dt);

	// tick bullets
	for(LightningBullet *bullet : bullets)
		bullet->tick(dt);

	for(SplashScreen *splash : splashScreens)
		splash->tick(dt);

	// add another enemy if less than 20
	if(enemyEntities.size() < 10){
		addEnemy();
	}

	// update border
	border->tick(dt);

	// update water
	water->tick(dt);
}

void EntityManager::stop(){

}

unsigned int EntityManager::assignId(){

	return nextId++;
}

void EntityManager::createExplosion(glm::vec3 pos){
	
    Explosion* explosion = new Explosion(this);
    explosion->init(pos);
    explosions.push_back(explosion);
}

void EntityManager::createBullet(glm::vec3 pos, glm::vec3 dir){
	// add a bullet
	LightningBullet* bullet = new LightningBullet(engine);
	bullet->init("../assets/models/ballSml.obj", pos, dir, 1000, 10);
	bullets.push_back(bullet);
}

void EntityManager::addEnemy(){

    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distZ(minZ, maxZ);
    std::uniform_real_distribution<float> distY(300, 500);
    std::uniform_real_distribution<float> distSize(10, 50);
	// add a ball
	Enemy* ball = new Enemy(engine, glm::vec3(distX(gen), distY(gen), distZ(gen)), distSize(gen), 32, 1);
	ball->init("../assets/models/ballSml.obj");
	ball->id = assignId();
	enemyEntities.push_back(ball);
}