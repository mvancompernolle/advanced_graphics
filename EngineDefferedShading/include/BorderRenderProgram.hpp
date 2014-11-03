#ifndef BORDER_RENDER_PROGRAM_HPP
#define	BORDER_RENDER_PROGRAM_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ShaderProgram.hpp"

namespace Vancom{

class BorderRenderProgram : public ShaderProgram{

public:

    // public functions
    BorderRenderProgram();
    bool init();    
    void setMVP(const glm::mat4 MVP);
    void setCameraPos(const glm::vec3& pos);
    void setParticleSize(float width, float height);
    void setMixVal(float mixVal);

 private:

    // private variables
    GLint locMVP;
    GLint locCameraPos;
    GLint locWidth;
    GLint locHeight;
    GLint locMixVal;

};

} // end namespace Vancom


#endif	/* BORDER_RENDER_PROGRAM_HPP */