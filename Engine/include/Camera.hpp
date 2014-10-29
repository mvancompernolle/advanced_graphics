#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

namespace Vancom{

class Engine;

class Camera{

public:

	// public functions
	Camera(Engine *engine);

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void rotate(float x, float y);
    void update();

    void applyBoost(float newBoost);

    glm::mat4 getView() const;
    glm::vec3 getPos() const;

private:

	// private variables
    Engine *engine;
    glm::vec3 pos, orientation, up;
    float rotateX, rotateY, speed, sensitivity, boost;
};

} // end Vancom namespace



#endif // END CAMERA_HPP