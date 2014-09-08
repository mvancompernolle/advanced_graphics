
#include <fstream>
#include <iostream>
#include <string>
#include <GL/glew.h>


class ShaderLoader {

	public:
		ShaderLoader(GLenum);
		bool loadShader(const char*);
		GLuint getShader() const;
 
		
	private:
		GLenum shaderType;
		GLuint shader;


};
