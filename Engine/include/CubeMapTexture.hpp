#ifndef CUBE_MAP_TEXTURE
#define CUBE_MAP_TEXTURE

#include <GL/glew.h>
#include <string>

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html
namespace Vancom{

class CubeMapTexture{

public:

    // public functions
    CubeMapTexture(const std::string& directory,
        const std::string& posXFilename,
        const std::string& negXFilename,
        const std::string& posYFilename,
        const std::string& negYFilename,
        const std::string& posZFilename,
        const std::string& negZFilename);
    ~CubeMapTexture();
    bool load();
    void bind(GLenum textureUnit);

private:

    // private variables
    std::string fileNames[6];
    GLuint texture;

};

} // end namespace vancom



#endif // END CUBE_MAP_TEXTURE