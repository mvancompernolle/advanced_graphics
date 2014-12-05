#ifndef PHYSICS_MANAGER_HPP
#define PHYSICS_MANAGER_HPP

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <btBulletDynamicsCommon.h>

namespace Vancom{

class Engine;

class PhysicsManager {

public:

    // public functions
    PhysicsManager(Engine *engine);
    ~PhysicsManager();

    void init();
    void tick(float dt);
    void stop();

    static btVector3 convertTobtVector(glm::vec3 arr);
    btRigidBody* addRigidBody(btCollisionShape* shape,  
        btQuaternion orientation, btVector3 position, int mass, float friction, btVector3 inertia);

    // public variables

private:

    // private variables
    Engine *engine;
    std::vector<btRigidBody*> rigidBodies;
    btBroadphaseInterface* broadphase; 
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btDiscreteDynamicsWorld* dynamicsWorld;

};

} // end namespace Vancom

#endif // END PHYSICS_MANAGER_HPP