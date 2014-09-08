#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>
#include <FreeImagePlus.h>
#include "gdal_priv.h"

// REFERENCE: ogldev.atspace.co.uk/www/tutorial16/tutorial16.html

class Texture
{
public:
    Texture(GLenum TextureTarget, const char* fileName);
	bool create();
    bool load();
 	fipImage getImage();
 	GDALDataset* getGDALImage();
    void bind(GLenum TextureUnit);

private:
    const char* m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
	GDALDataset *poDataset;
    fipImage m_image;
};


#endif	/* TEXTURE_H */

