#ifndef DIRECTIONAL_LIGHT_DS_PROGRAM
#define	DIRECTIONAL_LIGHT_DS_PROGRAM

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LightDSProgram.hpp"

namespace Vancom{

class DirectionalLightDSProgram : public LightDSProgram{

public:

	// public functions
    DirectionalLightDSProgram();
    bool init();
	void setLightDirection(const glm::vec3 direction) const;
    
private:

	// private functions
    GLuint locLightDir;
};

} // end namespace Vancom


#endif // END DIRECTIONAL_LIGHT_DS_PROGRAM