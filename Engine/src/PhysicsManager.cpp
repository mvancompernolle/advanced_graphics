#include "PhysicsManager.hpp"

#include <iostream>

#include "Engine.hpp"

using namespace Vancom;

PhysicsManager::PhysicsManager(Engine *engine) : engine(engine){

}

PhysicsManager::~PhysicsManager(){

}

void PhysicsManager::init(){

    // initialize bullet
    broadphase = new btDbvtBroadphase();

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-100,0));

}

void PhysicsManager::tick(float dt){
    
    if(dynamicsWorld)//step the simulation
        dynamicsWorld->stepSimulation(dt, 10);
}

void PhysicsManager::stop(){

}

btRigidBody* PhysicsManager::addRigidBody(btCollisionShape* shape, 
    btQuaternion orientation, btVector3 position, int mass, float friction, btVector3 inertia){

    btRigidBody* rbody;

    btDefaultMotionState* motionState =
            new btDefaultMotionState(btTransform(orientation, position));
    shape->calculateLocalInertia(mass,inertia);

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionState,shape,inertia);
    rigidBodyCI.m_restitution = .1f;
    rigidBodyCI.m_friction = friction;
    rbody = new btRigidBody(rigidBodyCI);
    dynamicsWorld->addRigidBody(rbody);

    // add rigid body into rigid body array
    rigidBodies.push_back(rbody);  

    return rbody;  
}

btVector3 PhysicsManager::convertTobtVector(glm::vec3 arr){
    btVector3 vert;

    vert[0] = arr.x;
    vert[1] = arr.y;
    vert[2] = arr.z;

    return vert;    
}