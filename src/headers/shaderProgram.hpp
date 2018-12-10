#ifndef HEADER_SHADERPROGRAM
#define HEADER_SHADERPROGRAM

#include <common.hpp>
#include <dataFile.hpp>
#include <resourceManager.hpp>
#include <material.hpp>

class TshaderProgram{
	//private members
	const GLuint id;
	map<string, GLuint> uniforms;
	//private functions
	void construct(const TdataFile &shaderProgData);
	void destruct();

	void setUniform(GLuint &u, GLfloat v);
	void setUniform(GLuint &u, GLint v);
	void setUniform(GLuint &u, GLuint v);
	void setUniform(GLuint &u, array<GLfloat, 1> v);
	void setUniform(GLuint &u, array<GLfloat, 2> v);
	void setUniform(GLuint &u, array<GLfloat, 3> v);
	void setUniform(GLuint &u, array<GLfloat, 4> v);
	void setUniform(GLuint &u, array<GLint, 1> v);
	void setUniform(GLuint &u, array<GLint, 2> v);
	void setUniform(GLuint &u, array<GLint, 3> v);
	void setUniform(GLuint &u, array<GLint, 4> v);
	void setUniform(GLuint &u, array<GLuint, 1> v);
	void setUniform(GLuint &u, array<GLuint, 2> v);
	void setUniform(GLuint &u, array<GLuint, 3> v);
	void setUniform(GLuint &u, array<GLuint, 4> v);
	void setUniform(GLuint &u, glm::vec2 v);
	void setUniform(GLuint &u, glm::vec3 v);
	void setUniform(GLuint &u, glm::vec4 v);
	void setUniform(GLuint &u, glm::mat2 v);
	void setUniform(GLuint &u, glm::mat3 v);
	void setUniform(GLuint &u, glm::mat4 v);
public:
	//public members
	const string name;
	//constructors
	// TshaderProgram();
	TshaderProgram(const string&);
	TshaderProgram(const TdataFile&);
	~TshaderProgram();
	//public functions
	void use() const;

	template<typename T> void setUniform(const string &u, T v){
		use();
		if(not uniforms.count(u)){
			uniforms[u] = glGetUniformLocation(id, u.data());
		}
		setUniform(uniforms[u], v);
	}
	void setMaterial(const Tmaterial &material);
	static TresourceManager<TshaderProgram> manager;
};

#endif//HEADER_SHADERPROGRAM
