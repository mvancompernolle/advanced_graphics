#ifndef GRASS_PROGRAM_HPP
#define	GRASS_PROGRAM_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ShaderProgram.hpp"

namespace Vancom{

class GrassProgram : public ShaderProgram{

public:

    // public functions
    GrassProgram();
    bool init();    
    void setMVP(const glm::mat4 MVP);
    void setModel(const glm::mat4 model) const;
    void setWindDir(const glm::vec3 windDir);
    void setCameraPos(const glm::vec3 pos);

 private:

    // private variables
    GLint locMVP;
    GLint locModel;
    GLint locWindDir;
    GLint locDrawDistance;
    GLint locCameraPos;

};

} // end namespace Vancom


#endif	/* GRASS_PROGRAM_HPP */