#ifndef STENCIL_PROGRAM_HPP
#define	STENCIL_PROGRAM_HPP

#include "ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class StencilProgram : public ShaderProgram{

public:

    // public functions
    StencilProgram();
    bool init();    
    void setMVP(const glm::mat4 MVP);

    // public variables
    GLuint locMVP;
    
private:

};

}

#endif	// STENCIL_PROGRAM_HPP 