#ifndef FIREWORKS_RENDER_PROGRAM_HPP
#define	FIREWORKS_RENDER_PROGRAM_HPP

#include "ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class FireworksRenderProgram : public ShaderProgram{

public:

    // public functions
    FireworksRenderProgram();
    virtual bool init();    
    void setMVP(const glm::mat4 MVP);
    void setCameraPos(const glm::vec3& Pos);
    void setColor(const glm::vec3& color);
    void setColorTextureUnit(unsigned int textureUnit);
    void setBillboardSize(float billboardSize);
    
 private:

    // private variables
    GLuint locMVP;
    GLuint locCameraPos;
    GLuint locColorMap;
    GLuint locSize;
    GLuint locColor;
    
};

}

#endif	/* FIREWORKS_RENDER_PROGRAM_HPP */