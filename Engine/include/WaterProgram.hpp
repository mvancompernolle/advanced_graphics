#ifndef WATER_PROGRAM_HPP
#define WATER_PROGRAM_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ShaderProgram.hpp"
#include "Lights.hpp"

namespace Vancom{

class WaterProgram : public ShaderProgram{

public:

    // public functions
    WaterProgram();
    bool init();    
    void setMVP(const glm::mat4 MVP) const;
    void setModel(const glm::mat4 model) const;
    void setTextureUnit(unsigned int textureUnit) const;
    void setTime(float time) const;

 private:

    // private variables
    GLint locMVP;
    GLint locModel;
    GLint locTime;
    GLint locTextureUnit;
    GLint locColor;
    GLint locDir;
    GLint locAmbient;
    GLint locDiffuse;
    GLint locCameraPos;

};

} // end namespace Vancom


#endif  /* WATER_PROGRAM_HPP */