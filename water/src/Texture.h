#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>
#include <FreeImagePlus.h>

// REFERENCE: ogldev.atspace.co.uk/www/tutorial16/tutorial16.html

class Texture
{
public:
    Texture(GLenum TextureTarget, const char* fileName);
    ~Texture();
	bool create();
    bool load();
 	fipImage getImage();
    void bind(GLenum TextureUnit);

private:
    const char* m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    fipImage m_image;
};


#endif	/* TEXTURE_H */

