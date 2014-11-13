#ifndef GEOMETRY_PROGRAM_HPP
#define	GEOMETRY_PROGRAM_HPP

#include "ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class GeometryProgram : public ShaderProgram {

public:

    // public functions
    GeometryProgram();
    virtual bool init();
    void setMVP(const glm::mat4 MVP) const;
    void setModelPos(const glm::mat4 model) const;
    void setColorTextureUnit(const int textureUnit) const;
    void setSpecularPower(const float power) const;
    void setSpecularIntensity(const float intensity) const;

private:

    // private variables
    GLuint locMVP;
    GLuint locModel;
    GLuint locSampler;
    GLuint locSpecularPower;
    GLuint locSpecularIntensity;

};

} // end namespace Vancom

#endif // end GEOMETRY_PROGRAM_HPP