#include <iostream>
#include "Texture.h"

// REFERENCE: ogldev.atspace.co.uk/www/tutorial16/tutorial16.html

Texture::Texture(GLenum TextureTarget, const char* fileName)
{
    m_textureTarget = TextureTarget;
    m_fileName      = fileName;
    m_image = NULL;
}

fipImage Texture::getImage(){
    return m_image;
}

GDALDataset* Texture::getGDALImage(){
    return poDataset;
}

bool Texture::load()
{
    // create gdal variable
    poDataset = (GDALDataset *) GDALOpen( m_fileName, GA_ReadOnly );
    if( poDataset == NULL )
    {
        std::cout << "Texture failed to open!" << std::endl;
        return false;
    }

    // if texture found, load it
    if(m_image.load(m_fileName)) {

        // if unknown image type, return
        if(m_image.getImageType() == FIT_UNKNOWN) {
            std::cerr << "Unkown image type!" << std::endl;
            return false;
        }

        // convert image to 32 bit pixels
        m_image.convertTo32Bits();
        m_image.flipHorizontal();
    }
    else{
        return false;
    }

    /*if(m_textureTarget == GL_TEXTURE_1D){
     // get the heighmap image for the terrain
    RGBQUAD pixel;

    // loop through the image and get each pixel value
    std::cout << m_image.getWidth() << " " << m_image.getHeight() << std::endl;
    for(int x=0; x<m_image.getWidth(); x++){
        for(int y=m_image.getHeight()-1; y>=0; y--){
            m_image.getPixelColor(x, y, &pixel);
            std::cout << "Shade: " << (int)pixel.rgbRed << " " << (int)pixel.rgbBlue << " " <<(int)pixel.rgbGreen << " " << (int)pixel.rgbReserved << std::endl;
        }
    }       
    }*/

    
    return true;
}

bool Texture::create(){

    // if texture found, load it
    if(load()) {

        // generate OpenGL texture
        glGenTextures(1, &m_textureObj);

        // bind texture to textureID
        glBindTexture(m_textureTarget, m_textureObj);

        // set texture parameters
        glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load OpenGL texture
        if(m_textureTarget == GL_TEXTURE_2D){
            glTexImage2D(m_textureTarget, 0, GL_RGBA, m_image.getWidth(), m_image.getHeight(),
            0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) m_image.accessPixels());
        }
        else if(m_textureTarget == GL_TEXTURE_1D){
            glTexImage1D(m_textureTarget, 0, GL_RGBA, m_image.getWidth() , 0,
            GL_RGBA, GL_UNSIGNED_BYTE, (void*) m_image.accessPixels());
        }

    }
    else{
        return false;
    }
    
    return true;
}

void Texture::bind(GLenum TextureUnit)
{
    
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
    
}
