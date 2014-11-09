// Source : http://ogldev.atspace.co.uk/www/tutorial29/tutorial29.html

#ifndef SELECTION_TEXTURE_HPP
#define	SELECTION_TEXTURE_HPP

#include <GL/glew.h>

namespace Vancom{

class SelectionTexture{

public:

    // public functions
    SelectionTexture();
    ~SelectionTexture();
    bool init(unsigned int width, unsigned int height);
    void enableWriting();
    void disableWriting();
    
    struct PixelInfo {
        float objectId;
        float drawId;
        float primId;
        
        PixelInfo(){
            objectId = 0.0f;
            drawId = 0.0f;
            primId = 0.0f;
        }
    };

    PixelInfo readPixel(unsigned int x, unsigned int y);
    
private:

    // private variables
    GLuint fbo;
    GLuint selectionTexture;
    GLuint depthTexture;
};

} // end Vancom namespace



#endif	// END SELECTION_TEXTURE_HPP 
