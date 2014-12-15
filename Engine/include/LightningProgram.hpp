#ifndef LIGHTNING_PROGRAM_HPP
#define LIGHTNING_PROGRAM_HPP

#include "ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

namespace Vancom{

class LightningProgram : public ShaderProgram{

public:

    // public functions
    LightningProgram();
    virtual bool init();    
    void setMVP(const glm::mat4 MVP);
    void setColor(const glm::vec3 color) const;
    void setPositions(const std::vector<glm::vec3> positions) const;
    void setRandomTextureUnit(unsigned int textureUnit) const;
    void setTime(float time) const;
    
 private:

    // private variables
    GLuint locMVP;
    GLuint locColor;
    GLuint locTime;
    GLuint locPositions;
    GLuint locRandomTexture;
    
};

}

#endif  /* LIGHTNING_PROGRAM_HPP */