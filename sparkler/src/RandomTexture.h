#ifndef RANDOM_TEXTURE_H
#define	RANDOM_TEXTURE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial28.html

class RandomTexture
{
public:
    RandomTexture();
    ~RandomTexture();
    void initRandomTexture(unsigned int size);
    void bind(GLenum TextureUnit);

private:
    GLuint textureObj;
};



#endif	/* RANDOM_TEXTURE_H */