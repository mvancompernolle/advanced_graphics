#ifndef DIR_LIGHT_DS_PROGRAM
#define	DIR_LIGHT_DS_PROGRAM

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LightDSProgram.hpp"
#include "Lights.hpp"

namespace Vancom{

class DirLightDSProgram : public LightDSProgram{

public:

	// public functions
    DirLightDSProgram();
    bool init();
	void setDirLight(const DirectionalLight& light) const;
    
private:

	// private functions
    GLuint locColor;
    GLuint locAmbient;
    GLuint locDiffuse;
    GLuint locDir;

};

} // end namespace Vancom


#endif // END DIR_LIGHT_DS_PROGRAM