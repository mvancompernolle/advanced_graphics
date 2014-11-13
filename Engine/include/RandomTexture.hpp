#ifndef RANDOM_TEXTURE_HPP
#define	RANDOM_TEXTURE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial28.html

namespace Vancom{

class RandomTexture{

public:

	// public variables
    RandomTexture();
    ~RandomTexture();
    void initRandomTexture(unsigned int size);
    void bind(GLenum TextureUnit);

private:

	// private variables
    GLuint textureObj;

};

} // end namespace Vancom

#endif	/* RANDOM_TEXTURE_HPP */