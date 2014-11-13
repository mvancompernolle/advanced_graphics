#ifndef POINT_LIGHT_DS_PROGRAM
#define	POINT_LIGHT_DS_PROGRAM

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "LightDSProgram.hpp"
#include "Lights.hpp"

namespace Vancom{

class PointLightDSProgram : public LightDSProgram{

public:

	// public functions
    PointLightDSProgram();
    bool init();
	void setPointLight(const PointLight& light) const;
    
private:

	// private variables
    GLuint locColor;
    GLuint locAmbient;
    GLuint locDiffuse;
    GLuint locPos;
    GLuint locAttenConst;
    GLuint locAttenLinear;
    GLuint locAttenExp;
};

} // end namespace Vancom


#endif // END POINT_LIGHT_DS_PROGRAM