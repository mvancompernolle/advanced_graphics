#ifndef SHADOW_MAP_PROGRAM_HPP
#define	SHADOW_MAP_PROGRAM_HPP

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial23/tutorial23.html

#include "ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class ShadowMapProgram : public ShaderProgram{

public:

    // public functions
    ShadowMapProgram();
    bool init();    
    void setMVP(const glm::mat4 MVP);
    void setTextureUnit(unsigned int textureUnit);
    
private:

    // private variables
    GLuint locMVP;
    GLuint locTexture;
};

} // end namespace vancom

#endif	// SHADOW_MAP_PROGRAM_HPP 