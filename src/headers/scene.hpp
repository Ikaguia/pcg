#ifndef HEADER_SCENE
#define HEADER_SCENE

#include <common.hpp>

#include <object.hpp>
#include <shader.hpp>
#include <light.hpp>
#include <dataFile.hpp>
// #include <texture.hpp>

class Tscene{
	//private members
	map<string, Tobject> objects;
	vector<Tlight> lights;

	GLfloat the_time = 0.0f;

	glm::mat4 the_scene_scale = glm::mat4(1.0f);
	vector<function<void(Tscene&, GLfloat)>> update_functions;

	//private functions
	void construct(const string&);
public:
	//public members
	const string name;
	const map<string, Tobject> &the_objects = objects;
	const GLfloat& time = the_time;
	const glm::mat4 &scene_scale = the_scene_scale;
	//constructors
	Tscene(const string&);
	~Tscene();
	//public functions
	void load(const string&);
	void destruct();
	void draw();
	void update(const GLfloat&);
	void addUpdateFunction(const function<void(Tscene&, GLfloat)> &foo);
	Tlight& addLight(const Tlight&);
	Tlight& addLight(const TdataFile&);

	Tobject& getObj(const string&);
	const Tobject& getObj(const string&) const;
	map<string, Tobject>& getObjs();
	const map<string, Tobject>& getObjs() const;
};

#endif//HEADER_SCENE
