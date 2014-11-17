#ifndef SHADOW_VOLUME_PROGRAM_HPP
#define	SHADOW_VOLUME_PROGRAM_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ShaderProgram.hpp"

namespace Vancom{

class ShadowVolumeProgram : public ShaderProgram{

public:

    // public functions
    ShadowVolumeProgram();
    virtual bool init();
    void setVP(const glm::mat4 vp) const;
    void setModel(const glm::mat4 model) const;
    void setLightPosition(const glm::vec3 pos) const;
    
private:

    // private variables
    GLuint locVP;
    GLuint locModel;
    GLuint locLightPos;

};

} // end namespace Vancom

#endif	// SHADOW_VOLUME_PROGRAM_HPP
