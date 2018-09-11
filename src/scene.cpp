#include <scene.hpp>

//constructors
Tscene::Tscene(const string& n):name{n}{}
Tscene::Tscene(const string& n, const string& file):name{n}{ construct(file); }
Tscene::~Tscene(){ destruct(); }

//private functions
void Tscene::destruct(){
	vertices.clear();
	objects.clear();
	// shPrograms.clear();
	// textures.clear();
}

//public functions
void Tscene::construct(const string& filename){
	destruct();

	TdataFile scn(filename, "scene", ".scn");

	// for(const auto &shaderData : scn.sons["shaderProgs"]){
	// 	shaders.emplace_back(shaderData);
	// }

	for(const auto &objectData : scn.sons["objects"]){
		objects.emplace_back(objectData);
	}
}

void Tscene::draw()const{
	//set vertices
	for(const auto &obj : objects) obj.ss.draw();
}

void Tscene::update(const float &dTime){
	for(auto &obj : objects) obj.update(dTime);
}
