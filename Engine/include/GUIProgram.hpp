#ifndef GUI_PROGRAM_HPP
#define	GUI_PROGRAM_HPP

#include "ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class GUIProgram : public ShaderProgram{

public:

    // public functions
    GUIProgram();
    bool init();    
    void setSampler(const int sampler) const;
    
private:

    // private variables
    GLuint locSampler;

};

} // end namespace Vancom

#endif	// GUI_PROGRAM_HPP 