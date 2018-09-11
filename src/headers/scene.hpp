#ifndef HEADER_SCENE
#define HEADER_SCENE

#include <common.hpp>

#include <object.hpp>
#include <shader.hpp>
// #include <texture.hpp>

class Tscene{
	//private members
	map<string, Tobject> objects;
	// map<string, TshaderProgram> shPrograms;
	// map<string, Ttexture> textures;

	//private functions
	void destruct();
public:
	//public members
	const string name;
	//constructors
	Tscene(const string&);
	Tscene(const string&, const string&);
	~Tscene();
	//public functions
	void construct(const string&);
	void draw()const;
	void update(float);
};

#endif//HEADER_SCENE
