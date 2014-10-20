#ifndef CAMERA_H
#define	CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Camera
{
public:
    Camera(glm::vec3 pos, glm::vec3 orientation, glm::vec3 up, float speed, float rotateSensitivity);
    ~Camera();
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void rotate(float x, float y);
    void update();

    glm::mat4 getView() const;

private:
    glm::vec3 pos, orientation, up;
    float speed, sensitivity;
    float rotateX, rotateY;
};


#endif	/* CAMERA_H */

