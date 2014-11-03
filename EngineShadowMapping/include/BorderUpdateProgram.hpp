#ifndef BORDER_UPDATE_PROGRAM_HPP
#define	BORDER_UPDATE_PROGRAM_HPP

#include "ShaderProgram.hpp"

namespace Vancom{

class BorderUpdateProgram : public ShaderProgram{

public:

    // public functions
    BorderUpdateProgram();
    bool init();    
    void setDt(float dt);
    void setSpeed(float speed);
    void setGeneratorTime(float time);
    void setXBorderTime(float time);
    void setZBorderTime(float time);

    
 private:

    // private variables
    GLint locDt;
    GLint locSpeed;
    GLint locGeneratorTime;
    GLint locXBorderTime;
    GLint locZBorderTime;

};

} // end namespace Vancom


#endif	/* BORDER_UPDATE_PROGRAM_HPP */