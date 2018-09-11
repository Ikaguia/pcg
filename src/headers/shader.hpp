#ifndef HEADER_SHADER
#define HEADER_SHADER

#include <common.hpp>

namespace glShader{
	GLuint createF(const string &sourceFile, GLenum type);
	GLuint create(const char* source, GLenum type);
};

namespace glShaderProgram{
	GLuint create(const vector<GLuint> &shaders = vector<GLuint>{});
	void bind(GLuint program, const vector<GLuint> &shaders = vector<GLuint>{});
	void use(GLuint program = 0);
	void destroy(GLuint program);
};

#endif//HEADER_SHADER
