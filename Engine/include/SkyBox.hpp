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

class Engine;
class CubeMapTexture;
class Model;
class SkyBoxProgram;

class SkyBox{

public:

    SkyBox(Engine* engine);
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
    Engine* engine;
    CubeMapTexture* texture;
    Model* box;

};

}

#endif // END SKY_BOX_HPP