#ifndef SKY_BOX_HPP
#define SKY_BOX_HPP

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Vertex.hpp"
#include "Entity.hpp"

namespace Vancom{

class Camera;
class CubeMapTexture;
class Model;
class SkyBoxProgram;

class SkyBox{

public:

    SkyBox(const Camera* camera);
    ~SkyBox();
    bool init(const std::string& directory,
              const std::string& posXFilename,
              const std::string& negXFilename,
              const std::string& posYFilename,
              const std::string& negYFilename,
              const std::string& posZFilename,
              const std::string& negZFilename);
    void render(glm::mat4 projection, glm::mat4 view);

private:

    // private variables
    SkyBoxProgram* program;
    const Camera* camera;
    CubeMapTexture* texture;
    Model* box;

};

}

#endif // END SKY_BOX_HPP