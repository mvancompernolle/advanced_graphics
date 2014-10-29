#ifndef DEFAULTPROGRAM_HPP
#define	DEFAULTPROGRAM_HPP

#include "ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class DefaultProgram : public ShaderProgram{

public:

    // public functions
    DefaultProgram();
    bool init();    
    void setMVP(const glm::mat4 MVP);

    // public variables
    GLuint locMVP;
    GLuint locPos;
    GLuint locColor;
    
private:

};

}

#endif	// DEFAULTPROGRAM_HPP 