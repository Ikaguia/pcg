#include <shaderProgram.hpp>
#include <shader.hpp>
#include <GLmanager.hpp>
#include <texture.hpp>

TresourceManager<TshaderProgram> TshaderProgram::manager;

//constructors
// TshaderProgram::TshaderProgram():id{0},name{"invalid"}{}
TshaderProgram::TshaderProgram(const string &shaderFile):
	id{glCreateProgram()},
	name{"shaderFile"}{
		TdataFile shaderProgData(shaderFile, "shader", ".prog");
		construct(shaderProgData);
}
TshaderProgram::TshaderProgram(const TdataFile &shaderProgData):
	id{glCreateProgram()},
	name{shaderProgData.strings.at("name")[0]}{
		construct(shaderProgData);
}
TshaderProgram::~TshaderProgram(){ destruct(); }

//private functions
void TshaderProgram::construct(const TdataFile &shaderProgData){
	vector<Tshader> shaders;
	shaders.reserve(shaderProgData.sons.at("shaders").size());
	for(auto &shData : shaderProgData.sons.at("shaders")) shaders.emplace_back(shData);

	for(auto &shader : shaders) glAttachShader(id, shader.id);

	glLinkProgram(id);

	for(auto &shader : shaders) glDetachShader(id, shader.id);

	GLint status;
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if(status != GL_TRUE){
		char buffer[512];
		glGetProgramInfoLog(id, 512, nullptr, buffer);
		throw invalid_argument("Error linking shader program: '" + string(buffer) + "'\n");
	}

	setUniform("viewMatrix", GLmanager::calcViewMatrix());
	setUniform("clipMatrix", GLmanager::calcClipMatrix());
}
void TshaderProgram::destruct(){ glDeleteProgram(id); }

//public functions
void TshaderProgram::use() const{ glUseProgram(id); }

void TshaderProgram::setUniform(GLuint &u, GLfloat v){ glUniform1f(u, v); }
void TshaderProgram::setUniform(GLuint &u, GLint v){ glUniform1i(u, v); }
void TshaderProgram::setUniform(GLuint &u, GLuint v){ glUniform1ui(u, v); }
void TshaderProgram::setUniform(GLuint &u, array<GLfloat, 1> v){ glUniform1fv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, array<GLfloat, 2> v){ glUniform2fv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, array<GLfloat, 3> v){ glUniform3fv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, array<GLfloat, 4> v){ glUniform4fv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, array<GLint, 1> v){ glUniform1iv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, array<GLint, 2> v){ glUniform2iv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, array<GLint, 3> v){ glUniform3iv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, array<GLint, 4> v){ glUniform4iv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, array<GLuint, 1> v){ glUniform1uiv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, array<GLuint, 2> v){ glUniform2uiv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, array<GLuint, 3> v){ glUniform3uiv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, array<GLuint, 4> v){ glUniform4uiv(u, 1, v.data()); }
void TshaderProgram::setUniform(GLuint &u, glm::vec2 v){ glUniform2fv(u, 1, glm::value_ptr(v)); }
void TshaderProgram::setUniform(GLuint &u, glm::vec3 v){ glUniform3fv(u, 1, glm::value_ptr(v)); }
void TshaderProgram::setUniform(GLuint &u, glm::vec4 v){ glUniform4fv(u, 1, glm::value_ptr(v)); }
void TshaderProgram::setUniform(GLuint &u, glm::mat2 v){ glUniformMatrix2fv(u, 1, false, glm::value_ptr(v)); }
void TshaderProgram::setUniform(GLuint &u, glm::mat3 v){ glUniformMatrix3fv(u, 1, false, glm::value_ptr(v)); }
void TshaderProgram::setUniform(GLuint &u, glm::mat4 v){ glUniformMatrix4fv(u, 1, false, glm::value_ptr(v)); }
void TshaderProgram::setMaterial(const Tmaterial &material){
	setUniform("material.ambientTexture", 0);
	setUniform("material.diffuseTexture", 1);
	setUniform("material.specularTexture", 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.ambient->id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material.diffuse->id);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, material.specular->id);
	setUniform("material.shinyness", material.shinyness);
}

