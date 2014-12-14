#ifndef LIGHTNING_BULLET_HPP
#define LIGHTNING_BULLET_HPP

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Vertex.hpp"
#include "Entity.hpp"
#include "Mesh.hpp"

namespace Vancom{

class Engine;

class LightningBullet : public Entity{

public:
    
    // private variables
    LightningBullet(Engine *engine);
    ~LightningBullet();
    bool init(const char* fileName, glm::vec3 pos, glm::vec3 dir, float speed, float scale);
    void tick(float dt);
    void render();
    btRigidBody* getRigidBody();

    // public variables
    float timeElapsed;

private:

    // private variables
    Engine *engine;
    Mesh *mesh;
    float speed, scale;
    btRigidBody* rigidBody;
};

}

#endif // END LIGHTNING_BULLET_HPP