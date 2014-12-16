#ifndef GUI_PROGRAM2_HPP
#define GUI_PROGRAM2_HPP

#include "ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class GUIProgram2 : public ShaderProgram{

public:

    // public functions
    GUIProgram2();
    bool init();    
    void setColor(const glm::vec3 color) const;
    
private:

    // private variables
    GLuint locColor;

};

} // end namespace Vancom

#endif  // GUI_PROGRAM2_HPP 