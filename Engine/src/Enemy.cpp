#include "Enemy.hpp"

#include <random>

#include "Engine.hpp"
#include "PhysicsManager.hpp"
#include "EntityManager.hpp"
#include "LightingManager.hpp"
#include "GameManager.hpp"

using namespace Vancom;

Enemy::Enemy(Engine *engine) : engine(engine){

    speed = 5;
    timeElapsed = 10;
    decisionTime = 10;
    specularIntensity = 0;
    specularPower = 0;
}

Enemy::Enemy(Engine *engine, glm::vec3 pos, float scale, float power, float intensity): engine(engine){

    health = 100;
    moving = true;
    selected = false;
    speed = 5;
    timeElapsed = 3;
    decisionTime = 3;
    specularIntensity = intensity;
    specularPower = power;  
    startingPos = pos;
    this->scale = scale;
}

Enemy::~Enemy(){

}

bool Enemy::init(const char* fileName){

    // load Model model from assimp
    mesh = new Mesh(engine);
    mesh->disableMaterials();
    mesh->loadMesh(fileName, true);

    mesh->getCollisionShape()->setLocalScaling(btVector3(scale/10, scale/10, scale/10));
    rigidBody = engine->physics->addRigidBody(mesh->getCollisionShape(), btQuaternion(0,0,0,1), 
        btVector3(startingPos.x,startingPos.y,startingPos.z), 1.0f, 1.0f, btVector3(0,0,0));
    rigidBody->forceActivationState(DISABLE_DEACTIVATION);

    return true;
}

void Enemy::tick(float dt){

    if(engine->gameManager->running){
        // set model based on bt physics
        btTransform trans;
        btScalar m[16];

        rigidBody->getMotionState()->getWorldTransform(trans);
        trans.getOpenGLMatrix(m);
        model = glm::make_mat4(m);
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        //model = glm::translate(model, glm::vec3(0, -10, 0));

        if(health > 0){
            // get current model position
            glm::vec3 pos = glm::vec3(model[3][0], model[3][1], model[3][2]);

            // update target
            timeElapsed += dt;
            if(timeElapsed >= decisionTime || glm::distance(pos, target) < 10.0f){
                timeElapsed = 0;

                // pick a random location to move to 
                std::random_device rd;
                std::default_random_engine gen(rd());
                std::uniform_real_distribution<float> distX(engine->entityManager->minX, engine->entityManager->maxX);
                std::uniform_real_distribution<float> distZ(engine->entityManager->minZ, engine->entityManager->maxZ);
                std::uniform_real_distribution<float> distSpeed(150.0f, 300.0f);

                target = glm::vec3(distX(gen), pos.y, distZ(gen));
                speed = distSpeed(gen);
                    //std::cout << target.x << " " << target.y << " " << target.z << std::endl;
            }

            glm::vec3 dir = glm::normalize(target - pos) * speed;
            rigidBody->setLinearVelocity(btVector3(dir.x, dir.y, dir.z));
        }
        else{
            selected = false;
            if(moving){
                moving = false;
                // spawn explosion
                engine->entityManager->createExplosion(getPos());
                engine->lightingManager->addTempPointLight(getPos(), glm::vec3(1.0, 0.0, 0.0), 3.1);
                // increase bullet power level
                engine->gameManager->bulletPowerLevel += .2;
                engine->gameManager->score += 10;
            }
        }
    }
}

void Enemy::render(){

    // draw
    mesh->renderMesh();
}

btRigidBody* Enemy::getRigidBody(){
    return rigidBody;
}