#include "Engine.hpp"

#include <SDL.h>
#include <iostream>

#include "Clock.hpp"
#include "Graphics.hpp"
#include "Input.hpp"
#include "EntityManager.hpp"
#include "LightingManager.hpp"
#include "PhysicsManager.hpp"
#include "GameManager.hpp"

using namespace Vancom;

Engine::Engine(int argc, char **argv): argc(argc), argv(argv){
	init();
}

Engine::~Engine(){

	stop();
}

void Engine::init(){

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cerr << "Unable to initilize SDL" << std::endl;
        exit(1);
    }

    // create and init lighting manager
    gameManager = new GameManager(this);
    gameManager->init();

    // create and init physics manager
    physics = new PhysicsManager(this);
    physics->init();

    // create and init graphics component
    graphics = new Graphics(this);
    graphics->init();

    // create and init input component
    input = new Input(this);
    input->init();

    // create and init entity manager component
    entityManager = new EntityManager(this);
    entityManager->init();

    // create and init lighting manager
    lightingManager = new LightingManager(this);
    lightingManager->init();
}

int Engine::run(){

	Clock clock;
	float dt;

	// run the program
	clock.init();
	while(true){
		// get change in time since last tick
		dt = clock.tick();

		// call tick on components
		input->tick(dt);
        gameManager->tick(dt);
        physics->tick(dt);
		graphics->tick(dt);
		entityManager->tick(dt);
		lightingManager->tick(dt);
		graphics->render();
	}

	return 0;
}

void Engine::stop(int exit_code){

	// stop graphics component
	graphics->stop();
	delete graphics;

	// stop input component
	input->stop();
	delete input;

	// stop entityManager component
	entityManager->stop();
	delete entityManager;

	// stop lightingManager component
	lightingManager->stop();
	delete lightingManager;

    // stop physics component
    physics->stop();
    delete physics;

    // stop game manager component
    gameManager->stop();
    delete gameManager;

	SDL_Quit();

	exit(exit_code);
}
