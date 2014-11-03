#ifndef TEXTURE_HPP
#define	TEXTURE_HPP

#include <GL/glew.h>
#include <FreeImagePlus.h>

// REFERENCE: ogldev.atspace.co.uk/www/tutorial16/tutorial16.html

namespace Vancom{

class Texture{

public:

	// public functions
    Texture(GLenum TextureTarget, const char* fileName);
	bool create();
    bool load();
 	fipImage getImage();
    void bind(GLenum TextureUnit);

private:

	// private variables
    GLenum textureTarget;
    const char* fileName;
    GLuint textureObj;
    fipImage image;

};

}




#endif	/* TEXTURE_HPP */