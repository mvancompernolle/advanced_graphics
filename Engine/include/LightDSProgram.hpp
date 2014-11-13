#ifndef LIGHT_DS_PROGRAM
#define LIGHT_DS_PROGRAM

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial36/tutorial36.html

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.hpp"

namespace Vancom{

class LightDSProgram : public ShaderProgram{

public:

    // public functions
    LightDSProgram();
    virtual bool init();

    void setMVP(const glm::mat4 MVP) const;
    void setPositionTextureUnit(unsigned int textureUnit);
    void setColorTextureUnit(unsigned int textureUnit);
    void setNormalTextureUnit(unsigned int textureUnit);
    void setSpecularTextureUnit(unsigned int textureUnit);
    void setScreenSize(unsigned int width, unsigned int height);
    void setCameraPosition(const glm::vec3 pos) const;
    
private:

    // private functions
    GLuint locMVP;
    GLuint locPosTexture;
    GLuint locNormalTexture;
    GLuint locColorTexture;
    GLuint locSpecularTexture;
    GLuint locCameraPos;
    GLuint locScreenSize;
};

} // end namespace Vancom


#endif // END LIGHT_DS_PROGRAM