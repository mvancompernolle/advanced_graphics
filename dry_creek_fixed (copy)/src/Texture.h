#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>
#include <FreeImagePlus.h>
#include "gdal_priv.h"
#include <glm/glm.hpp>

// REFERENCE: ogldev.atspace.co.uk/www/tutorial16/tutorial16.html

class Texture
{
public:
    Texture(GLenum TextureTarget, const char* fileName);
    ~Texture();
	bool create();
    bool load();
 	fipImage getImage();
 	GDALDataset* getGDALImage();
    void bind(GLenum TextureUnit);
    void setTextureFromArrayInfo();
    void setTextureFromFileInfo();
    void setPixelColor(int x, int y, glm::vec3 color);
    glm::vec3 getPixelColor(int x, int y);
    int getWidth();
    int getHeight();

private:
    const char* m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    int width, height;
	GDALDataset *poDataset;
    fipImage m_image;
    RGBQUAD* pixels;
};


#endif	/* TEXTURE_H */

