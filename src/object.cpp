#include <object.hpp>

//constructors
Tobject::Tobject(const string &name, const TdataFile &objData):name{thename}{ construct(objData); }
Tobject::Tobject(const TdataFile &objData):name{objData.strings["name"]}{ construct(objData); }
Tobject::~Tobject(){ destruct(); }

//private functions
void Tobject::construct(const TdataFile &objectData){
	type = objectData.strings["type"][0];
	shaderProg = objectData.strings["shaderProg"][0];
	mesh = objectData.strings["mesh"][0];
	if(objectData.floats.count("translation")) translation.xyz = objectData.floats["translation"];
	if(objectData.floats.count("rotation")) rotation.xyz = objectData.floats["rotation"];
	if(objectData.floats.count("scale")) scale.xyz = objectData.floats["scale"];
	//ShaderProgManager::bind(shaderProg);
	//MeshManager::bind(mesh);

	for(auto &obj : objectData.sons["objects"]){

	}
}
void Tobject::destruct(const TdataFile &objectData){
	//ShaderProgManager::unbind(shaderProg);
	//MeshManager::unbind(mesh);
}

//public functions
void Tobject::draw()const{
	//draw
	for(const auto &son : sons) son.draw();
}
void Tobject::update(const float &dTime){
	//update
	for(auto &son : sons) son.update(dTime);
}
