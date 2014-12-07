#include "Model.hpp"

#include <random>

#include "Engine.hpp"
#include "PhysicsManager.hpp"
#include "EntityManager.hpp"

using namespace Vancom;

Model::Model(Engine *engine) : engine(engine){

	specularIntensity = 0;
	specularPower = 0;
}

Model::Model(Engine *engine, glm::vec3 pos, float scale, float power, float intensity): engine(engine){

	specularIntensity = intensity;
	specularPower = power;	
	startingPos = pos;
	this->scale = scale;
}

Model::~Model(){

}

bool Model::init(const char* fileName, bool loadMaterials){

	// load Model model from assimp
	mesh = new Mesh(engine);
	if(!loadMaterials)
		mesh->disableMaterials();
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

}

void Model::render(){

	// draw
	mesh->renderMesh();
}

btRigidBody* Model::getRigidBody(){
    return rigidBody;
}