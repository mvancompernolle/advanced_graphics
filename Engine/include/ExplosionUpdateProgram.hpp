#ifndef EXPLOSION_UPDATE_PROGRAM_HPP
#define	EXPLOSION_UPDATE_PROGRAM_HPP

#include "ShaderProgram.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class ExplosionUpdateProgram : public ShaderProgram{

public:

    // public functions
    ExplosionUpdateProgram();
    virtual bool init();    
    void setDt(unsigned int dt) const;
    void setCenter(const glm::vec3& center) const;
    
 private:

    GLuint locDt;
    GLuint locCenter;

};

}


#endif	/* EXPLOSION_UPDATE_PROGRAM_HPP */