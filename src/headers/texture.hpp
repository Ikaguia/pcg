#ifndef HEADER_TEXTURE
#define HEADER_TEXTURE

#include <common.hpp>
#include <dataFile.hpp>
#include <resourceManager.hpp>

class Ttexture{
	//private functions
	GLuint construct(tuple<vector<unsigned char>, int, int> args);
	void destruct();
public:
	//public members
	const GLuint id;
	//constructors
	Ttexture(const string &file);
	Ttexture(const vector<GLfloat> &v);
	~Ttexture();

	static TresourceManager<Ttexture> manager;
};

#endif//HEADER_TEXTURE
