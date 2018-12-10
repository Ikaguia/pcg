#ifndef HEADER_MATERIAL
#define HEADER_MATERIAL

#include <common.hpp>

class Ttexture;
class TdataFile;

class Tmaterial{
public:
	shared_ptr<Ttexture> ambient, diffuse, specular;
	GLfloat shinyness;
	//constructors
	Tmaterial(const TdataFile&);
	~Tmaterial();
};

#endif//HEADER_MATERIAL
