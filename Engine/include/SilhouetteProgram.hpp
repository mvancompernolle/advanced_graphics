#ifndef SILHOUETTE_PROGRAM_HPP
#define	SILHOUETTE_PROGRAM_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ShaderProgram.hpp"

namespace Vancom{

class SilhouetteProgram : public ShaderProgram{

public:

    // public functions
    SilhouetteProgram();
    virtual bool init();
    void setMVP(const glm::mat4 mvp) const;
    void setModelPos(const glm::mat4 model) const;
    void setCameraPosition(const glm::vec3 pos) const;
    
private:
    
    // public variables
    GLuint locMVP;
    GLuint locModel;
    GLuint locCameraPos;

};

} // end namespace Vancom

#endif	// SILHOUETTE_PROGRAM_HPP
