#include <iostream>
#include "Texture.h"

// REFERENCE: ogldev.atspace.co.uk/www/tutorial16/tutorial16.html

Texture::Texture(GLenum TextureTarget, const char* fileName)
{
    m_textureTarget = TextureTarget;
    m_fileName      = fileName;
    m_image = NULL;
    pixels = NULL;
}

Texture::~Texture(){
    GDALClose( (GDALDatasetH) poDataset ); 
    if(pixels != NULL)
        delete pixels;   
}

fipImage Texture::getImage(){
    return m_image;
}

GDALDataset* Texture::getGDALImage(){
    return poDataset;
}

bool Texture::load()
{
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
        height = m_image.getHeight();
        width = m_image.getWidth();
    }
    else{
        return false;
    }

    // create an edittable data version to be used with certain files
    RGBQUAD pixel = {0xff, 0xff, 0xff, 0xff};
    pixels = new RGBQUAD[width * height];
    for(int i=0; i<width * height; i++){
        //std::cout << m_image.getPixelColor(i%m_image.getWidth(), i/m_image.getWidth(), &pixel) << std::endl;
        //std::cout << "Shade: " << (int)pixel.rgbRed << " " << (int)pixel.rgbBlue << " " <<(int)pixel.rgbGreen << " " << (int)pixel.rgbReserved << std::endl;
        pixels[i] = pixel;
    }

    // create gdal variable
    poDataset = (GDALDataset *) GDALOpen( m_fileName, GA_ReadOnly );
    if( poDataset == NULL )
    {
        std::cout << "Texture failed to open!" << std::endl;
        return false;
    }

    /*if(m_textureTarget == GL_TEXTURE_1D){
         // get the heighmap image for the terrain
        RGBQUAD pixel = {0xff, 0xff, 0x00, 0x00};

        // loop through the image and get each pixel value
        std::cout << m_image.getWidth() << " " << m_image.getHeight() << std::endl;
        for(int x=0; x<m_image.getWidth(); x++){
            for(int y=m_image.getHeight()-1; y>=0; y--){
                bool worked = m_image.getPixelColor(x, y, &pixel);
                std::cout << worked << ' ' << &pixel << ' ' <<  std::endl;
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

        setTextureFromFileInfo();
    }
    else{
        return false;
    }
    
    return true;
}

void Texture::setTextureFromFileInfo(){
     // load OpenGL texture
    if(m_textureTarget == GL_TEXTURE_2D){
        glTexImage2D(m_textureTarget, 0, GL_RGBA, width, height,
        0, GL_BGRA, GL_UNSIGNED_BYTE, (void*) m_image.accessPixels());
    }
    else if(m_textureTarget == GL_TEXTURE_1D){
        glTexImage1D(m_textureTarget, 0, GL_RGBA, width , 0,
        GL_BGRA, GL_UNSIGNED_BYTE, (void*) m_image.accessPixels());
    }   
}

void Texture::setTextureFromArrayInfo(){
     // load OpenGL texture
    if(m_textureTarget == GL_TEXTURE_2D){
        glTexImage2D(m_textureTarget, 0, GL_RGBA, width, height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) pixels);
    }
    else if(m_textureTarget == GL_TEXTURE_1D){
        glTexImage1D(m_textureTarget, 0, GL_RGBA, width , 0,
        GL_RGBA, GL_UNSIGNED_BYTE, (void*) pixels);
    }   
}

void Texture::bind(GLenum TextureUnit)
{
    
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
    
}

void Texture::setPixelColor(int x, int y, glm::vec3 color){
    if(x < width && y < height){
        RGBQUAD rgb = {0xff, 0xff, 0xff, 0xff};
        rgb.rgbBlue = color[0];
        rgb.rgbGreen = color[1];
        rgb.rgbRed = color[2];
        pixels[y*width + x] = rgb;
    }
}

glm::vec3 Texture::getPixelColor(int x, int y){
    return glm::vec3(1, 1, 1);
}

int Texture::getWidth(){
    return width;
}

int Texture::getHeight(){
    return height;
}