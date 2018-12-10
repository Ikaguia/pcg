#include <shader.hpp>

TresourceManager<Tshader> Tshader::manager;
const map<string,GLenum> Tshader::shaderTypes{
	{"fshader",		GL_FRAGMENT_SHADER},
	{"fragment",	GL_FRAGMENT_SHADER},
	{"vshader",		GL_VERTEX_SHADER},
	{"vertex",		GL_VERTEX_SHADER},
	{"tess_control",GL_TESS_CONTROL_SHADER},
	{"tess_eval",	GL_TESS_EVALUATION_SHADER},
	{"geometry",	GL_GEOMETRY_SHADER},
	{"compute",		GL_COMPUTE_SHADER},
};


//constructors
Tshader::Tshader(const TdataFile &shaderData):
	type{getType(shaderData)},
	id{glCreateShader(type)}{
		construct(shaderData);
}
Tshader::~Tshader(){ destruct(); }

//private functions
GLuint Tshader::getType(const TdataFile &shaderData){ return shaderTypes.at(shaderData.strings.at("type")[0]); }
void Tshader::construct(const TdataFile &shaderData){
	string sourceFile = shaderData.strings.at("file")[0];
	string fileName = TdataFile::findFile(sourceFile, "shader");
	ifstream file(fileName);
	if(not file.is_open()) throw invalid_argument("Invalid shader source file '" + sourceFile + "'");
	string shader, s;
	while(getline(file, s)) shader += (s + "\n");
	const char *src = shader.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE){
		char buffer[512];
		glGetShaderInfoLog(id, 512, nullptr, buffer);
		throw logic_error("Error compiling shader: '" + string(buffer) + "'");
	}
}
void Tshader::destruct(){ glDeleteShader(id); }
