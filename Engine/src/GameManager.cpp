#include "GameManager.hpp"

#include <iostream>
#include <random>

#include "Engine.hpp"

using namespace Vancom;

GameManager::GameManager(Engine *engine) : engine(engine){

    score = 500.0f;
    running = false;
    bulletPowerLevel = 0;
    bulletColors[0] = glm::vec3(1,1,0);
    bulletColors[1] = glm::vec3(1,.5,0);
    bulletColors[2] = glm::vec3(0,1,0);
    bulletColors[3] = glm::vec3(.5,0,1);
    bulletColors[4] = glm::vec3(1,1,1);
}

GameManager::~GameManager(){

}

void GameManager::init(){

}

void GameManager::tick(float dt){

    bulletPowerLevel -= dt/(8 - bulletPowerLevel);

    // slowly weaken power level
    if(bulletPowerLevel > 5)
        bulletPowerLevel = 5;
    else if(bulletPowerLevel < 0)
        bulletPowerLevel = 0;

    score -= dt * 10;
    if(score > 1000)
        score = 1000;
    else if(score < 0)
        score = 0;

}

void GameManager::stop(){

}