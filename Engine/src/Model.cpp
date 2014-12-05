#include "Model.hpp"

#include <random>

#include "Engine.hpp"
#include "PhysicsManager.hpp"
#include "EntityManager.hpp"

using namespace Vancom;

Model::Model(Engine *engine) : engine(engine){

	speed = 5;
	timeElapsed = 10;
	decisionTime = 10;
	specularIntensity = 0;
	specularPower = 0;
}

Model::Model(Engine *engine, glm::vec3 pos, float scale, float power, float intensity): engine(engine){

	moving = true;
	speed = 5;
	timeElapsed = 3;
	decisionTime = 3;
	specularIntensity = intensity;
	specularPower = power;	
	startingPos = pos;
	this->scale = scale;
}

Model::~Model(){

}

bool Model::init(const char* fileName){

	// load Model model from assimp
	mesh = new Mesh(engine);
	mesh->loadMesh(fileName, true);

	mesh->getCollisionShape()->setLocalScaling(btVector3(scale, scale, scale));
    rigidBody = engine->physics->addRigidBody(mesh->getCollisionShape(), btQuaternion(0,0,0,1), 
    	btVector3(startingPos.x,startingPos.y,startingPos.z), 1.0f, 1.0f, btVector3(0,0,0));
	rigidBody->forceActivationState(DISABLE_DEACTIVATION);

	return true;
}

void Model::tick(float dt){

	// set model based on bt physics
    btTransform trans;
	btScalar m[16];

	rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	model = glm::make_mat4(m);
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	//model = glm::translate(model, glm::vec3(0, -10, 0));

	if(updating){
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

}

void Model::render(){

	// draw
	mesh->renderMesh();
}

btRigidBody* Model::getRigidBody(){
    return rigidBody;
}