#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class Engine;

class GameManager {

public:

    // public functions
    GameManager(Engine *engine);
    ~GameManager();

    void init();
    void tick(float dt);
    void stop();

    // public variables
    bool running;
    float score;
    float bulletPowerLevel;
    glm::vec3 bulletColors[5];

private:

    // private functions

    // private variables
    Engine *engine;

};

} // end namespace Vancom

#endif // END GAME_MANAGER_HPP