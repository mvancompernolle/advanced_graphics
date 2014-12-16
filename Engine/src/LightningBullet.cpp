#include "LightningBullet.hpp"

#include <random>

#include "Engine.hpp"
#include "PhysicsManager.hpp"
#include "EntityManager.hpp"
#include "GameManager.hpp"

using namespace Vancom;

LightningBullet::LightningBullet(Engine *engine): engine(engine){
    speed = 5;
    timeElapsed = 0;
    specularIntensity = 100;
    specularPower = 5;  
}

LightningBullet::~LightningBullet(){
    delete mesh;
    //delete rigidBody;
}

bool LightningBullet::init(const char* fileName, int powerLevel, glm::vec3 pos, glm::vec3 dir, float speed, float scale){

    // load Model model from assimp
    this->powerLevel = powerLevel;
    vert.color = engine->gameManager->bulletColors[powerLevel];
    this->scale = scale;
    mesh = new Mesh(engine);
    mesh->disableMaterials();
    mesh->loadMesh(fileName, true);

    mesh->getCollisionShape()->setLocalScaling(btVector3(scale, scale, scale));
    rigidBody = engine->physics->addRigidBody(mesh->getCollisionShape(), btQuaternion(0,0,0,1), 
        btVector3(pos.x, pos.y, pos.z), 1.0f, 1.0f, btVector3(0,0,0));
    rigidBody->forceActivationState(DISABLE_DEACTIVATION);
    rigidBody->setLinearVelocity(btVector3(dir.x * speed, dir.y * speed, dir.z * speed));
    rigidBody->setGravity(btVector3(0,0,0));

    return true;
}

void LightningBullet::tick(float dt){

    timeElapsed += dt;

    // set model based on bt physics
    btTransform trans;
    btScalar m[16];

    rigidBody->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);
    model = glm::make_mat4(m);
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    vert.pos = getPos();
    //model = glm::translate(model, glm::vec3(0, -10, 0));
}

void LightningBullet::render(){

    // draw
    mesh->renderMesh();
}

btRigidBody* LightningBullet::getRigidBody(){
    return rigidBody;
}