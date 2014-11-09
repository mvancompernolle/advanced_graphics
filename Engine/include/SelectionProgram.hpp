#ifndef SELECTION_PROGRAM_HPP
#define	SELECTION_PROGRAM_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ShaderProgram.hpp"

namespace Vancom{

class SelectionProgram : public ShaderProgram{

public:

    // public functions
    SelectionProgram();
    virtual bool init();
    void setMVP(const glm::mat4 mvp) const;
    void setObjectIndex(const uint objectIndex) const;
    
private:
    
    // public variables
    GLuint locMVP;
    GLuint locObjectIndex;

};

} // end namespace Vancom

#endif	// SELECTION_PROGRAM_HPP
