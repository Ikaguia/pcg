#ifndef HEADER_LIGHT
#define HEADER_LIGHT

#include <common.hpp>

class TshaderProgram;
class TdataFile;

class Tlight{
	vector<function<void(Tlight&, GLfloat)>> update_functions;
public:
	glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	GLfloat ambientStrength = 0.2f;
	GLfloat specularStrength = 0.75f;

	GLfloat linearDecay = 0.0f;
	GLfloat quadraticDecay = 0.0f;

	//constructors
	Tlight(const TdataFile&);

	//public functions
	void set(TshaderProgram&, const string &name) const;
	void addUpdateFunction(function<void(Tlight&, GLfloat)>);
	void update(GLfloat);
};

#endif//HEADER_LIGHT
