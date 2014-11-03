#ifndef SHADOW_MAP_FBO_HPP
#define SHADOW_MAP_FBO_HPP

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial23/tutorial23.html

#include <GL/glew.h>

namespace Vancom{

class ShadowMapFBO{

public:

	// public functions
    ShadowMapFBO();
    ~ShadowMapFBO();
    bool init(unsigned int windowWidth, unsigned int windowHeight);
    void bindForWriting();
    void bindForReading(GLenum textureUnit);

private:

	// private variables
	GLuint fbo;
	GLuint shadowMapTexture;

};

} // end namespace Vancom

#endif // END SHADOW_MAP_FBO_HPP