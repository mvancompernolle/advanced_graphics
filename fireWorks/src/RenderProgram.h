#ifndef RENDERPROGRAM_H
#define RENDERPROGRAM_H

// Source : http://ogldev.atspace.co.uk/www/tutorial28/tutorial28.html
#include <list>
#include <GL/glew.h>
#include <fstream>
#include <streambuf>

class RenderProgram{
public:
	RenderProgram();
	virtual ~RenderProgram();
	virtual bool init();
	void enable();
	
protected:
	bool addShader(GLenum shaderType, const char* fileName);
	bool finalize();
	GLint getUniformLocation(const char* uniformName);
	GLint getProgramParam(GLint param);

	GLuint program;

private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList shaderObjList;
};

#endif /* RENDERPROGRAM_H */