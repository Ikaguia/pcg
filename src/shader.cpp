#include <shader.hpp>

#include <dataFile.hpp>

GLuint glShaderProgram::create(const vector<GLuint> &shaders){
	GLuint program = glCreateProgram();
	glShaderProgram::bind(program, shaders);
	return program;
}
void glShaderProgram::bind(GLuint program, const vector<GLuint> &shaders){
	for(const GLuint &shader : shaders) glAttachShader(program, shader);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status != GL_TRUE){
		char buffer[512];
		glGetProgramInfoLog(program, 512, nullptr, buffer);
		cerr << "Error linking shader program: " << buffer << endl;
		exit(1);
	}

	for(const GLuint &shader : shaders) glDetachShader(program, shader);
}
void glShaderProgram::use(GLuint program){ glUseProgram(program); }
void glShaderProgram::destroy(GLuint program){ glDeleteProgram(program); }




GLuint glShader::createF(const string &sourceFile, GLenum type){
	ifstream file(TdataFile::findFile(sourceFile, "shader"));
	if(!file.is_open()){
		cerr << "Error opening shader file '" << sourceFile << "'" << endl;
		exit(1);
	}
	string shader, s;
	while(getline(file, s)) shader += s + "\n";
	const char *src = shader.c_str();
	return glShader::create(src, type);
}

GLuint glShader::create(const char* source, GLenum type){
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE){
		char buffer[512];
		glGetShaderInfoLog(shader, 512, nullptr, buffer);
		cerr << "Error compiling shader: " << buffer << endl;
		exit(1);
	}

	return shader;
}
