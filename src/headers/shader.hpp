#ifndef HEADER_SHADER
#define HEADER_SHADER

#include <common.hpp>
#include <dataFile.hpp>
#include <resourceManager.hpp>

class Tshader{
	//private members
	static const map<string,GLenum> shaderTypes;
	//private functions
	GLuint getType(const TdataFile &shaderData);
	void construct(const TdataFile &shaderData);
	void destruct();
public:
	//public members
	const GLenum type;
	const GLuint id;
	//constructors
	Tshader(const TdataFile &shaderData);
	~Tshader();
	static TresourceManager<Tshader> manager;
};

#endif//HEADER_SHADER
