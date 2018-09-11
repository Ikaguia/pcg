#ifndef HEADER_OBJECT
#define HEADER_OBJECT

#include <common.hpp>
#include <dataFile.hpp>

class Tobject{
	//private members
	string type, shaderProg, mesh;
	glm::vec4 translation = { 0.0f, 0.0f, 0.0f };
	glm::vec4 rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec4 scale = { 1.0f, 1.0f, 1.0f };
	map<string, Tobject> sons;
	//private functions
	void construct(const TdataFile &objectData);
	void destruct();
public:
	//public members
	const string name;
	//constructors
	Tobject(const string &thename, const TdataFile &objectData);
	Tobject(const TdataFile &objectData);
	~Tobject();
	//public functions
	void draw()const;
	void update(const float &dTime);
};

#endif//HEADER_OBJECT
