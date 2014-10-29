#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

// Source : http://ogldev.atspace.co.uk/www/tutorial28/tutorial28.html
#include <list>
#include <GL/glew.h>

namespace Vancom{

class ShaderProgram{

public:

	// public functioons
	ShaderProgram();
	virtual ~ShaderProgram();
	virtual bool init();
	void enable();
	
protected:

	// protected functions
	bool addShader(GLenum shaderType, const char* fileName);
	bool finalize();
	GLint getUniformLocation(const char* uniformName);
	GLint getProgramParam(GLint param);

	// protected variables
	GLuint program;

private:

	// private variables
	std::list<GLuint> shaderObjList;

};

} // end Vancom namespcae

#endif // END SHADERPROGRAM_H 