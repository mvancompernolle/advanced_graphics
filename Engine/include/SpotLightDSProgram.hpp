#ifndef SPOT_LIGHT_DS_PROGRAM
#define	SPOT_LIGHT_DS_PROGRAM

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LightDSProgram.hpp"
#include "Lights.hpp"

namespace Vancom{

class SpotLightDSProgram : public LightDSProgram{

public:

	// public functions
    SpotLightDSProgram();
    bool init();
	void setSpotLight(const SpotLight& light) const;
    
private:

	// private variables
    GLuint locColor;
    GLuint locAmbient;
    GLuint locDiffuse;
    GLuint locPos;
    GLuint locDir;
    GLuint locAttenConst;
    GLuint locAttenLinear;
    GLuint locAttenExp;
    GLuint locCutoff;
};

} // end namespace Vancom


#endif // END SPOT_LIGHT_DS_PROGRAM