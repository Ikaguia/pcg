#include <material.hpp>
#include <dataFile.hpp>
#include <texture.hpp>

//constructors
Tmaterial::Tmaterial(const TdataFile &data){
	if(data.strings.count("diffuse")){ diffuse = Ttexture::manager.get(data.strings.at("diffuse")[0]); }
	else if(data.floats.count("diffuse")){ diffuse = shared_ptr<Ttexture>(new Ttexture{data.floats.at("diffuse")}); }
	else throw invalid_argument("Failed to load material, no 'diffuse' field found.");

	if(data.strings.count("ambient")){ ambient = Ttexture::manager.get(data.strings.at("ambient")[0]); }
	else if(data.floats.count("ambient")){ ambient = shared_ptr<Ttexture>(new Ttexture{data.floats.at("ambient")}); }
	else ambient = diffuse;

	if(data.strings.count("specular")){ specular = Ttexture::manager.get(data.strings.at("specular")[0]); }
	else if(data.floats.count("specular")){ specular = shared_ptr<Ttexture>(new Ttexture{data.floats.at("specular")}); }
	else if(data.floats.count("specular")){ specular = shared_ptr<Ttexture>(new Ttexture{vector<GLfloat>{0.5f, 0.5f, 0.5f}}); }

	if(data.floats.count("shinyness")) shinyness = data.floats.at("shinyness")[0];
}
Tmaterial::~Tmaterial(){}
