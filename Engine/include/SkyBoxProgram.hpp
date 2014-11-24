#ifndef SKY_BOX_PROGRAM_HPP
#define SKY_BOX_PROGRAM_HPP

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ShaderProgram.hpp"

namespace Vancom{

class SkyBoxProgram : public ShaderProgram{

public:

    // public functions
    SkyBoxProgram();
    virtual bool init();
    void setMVP(const glm::mat4 mvp) const;
    void setTextureUnit(unsigned int textureUnit) const;
    
private:

    // private variables
    GLuint locMVP;
    GLuint locTexture;

};

} // end namespace Vancom

#endif  // SKY_BOX_PROGRAM_HPP
