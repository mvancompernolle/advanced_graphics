#ifndef TEXT_PROGRAM_HPP
#define TEXT_PROGRAM_HPP

#include "ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class TextProgram : public ShaderProgram{

public:

    // public functions
    TextProgram();
    bool init();    
    void setSampler(const int sampler) const;
    void setColor(glm::vec4 color) const;
    
private:

    // private variables
    GLuint locSampler;
    GLuint locColor;

};

} // end namespace Vancom

#endif  // TEXT_PROGRAM_HPP 