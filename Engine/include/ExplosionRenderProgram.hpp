#ifndef EXPLOSION_RENDER_PROGRAM_HPP
#define	EXPLOSION_RENDER_PROGRAM_HPP

#include "ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class ExplosionRenderProgram : public ShaderProgram{

public:

    // public functions
    ExplosionRenderProgram();
    virtual bool init();    
    void setMVP(const glm::mat4 MVP);
    void setCameraPos(const glm::vec3& Pos);
    void setColorTextureUnit(unsigned int textureUnit);
    void setBillboardSize(float billboardSize);
    
 private:

    // private variables
    GLuint locMVP;
    GLuint locCameraPos;
    GLuint locColorMap;
    GLuint locSize;
    
};

}

#endif	/* EXPLOSION_RENDER_PROGRAM_HPP */