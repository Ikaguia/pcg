#include <light.hpp>
#include <shaderProgram.hpp>
#include <dataFile.hpp>

//constructors
Tlight::Tlight(const TdataFile &lightData){
	if(lightData.floats.count("position")) position = makeVec4(lightData.floats.at("position"));
	if(lightData.floats.count("color"))       color = makeVec4(lightData.floats.at("color"));

	if(lightData.floats.count("ambientStrength"))   ambientStrength = lightData.floats.at("ambientStrength")[0];
	if(lightData.floats.count("specularStrength")) specularStrength = lightData.floats.at("specularStrength")[0];

	if(lightData.floats.count("linearDecay"))       linearDecay = lightData.floats.at("linearDecay")[0];
	if(lightData.floats.count("quadraticDecay")) quadraticDecay = lightData.floats.at("quadraticDecay")[0];
}


//public functions
void Tlight::set(TshaderProgram &prog, const string &name) const{
	prog.setUniform(name + string(".position"), position);
	prog.setUniform(name + string(".color"), color);

	prog.setUniform(name + string(".ambientStrength"), ambientStrength);
	prog.setUniform(name + string(".specularStrength"), specularStrength);

	prog.setUniform(name + string(".linearDecay"), linearDecay);
	prog.setUniform(name + string(".quadraticDecay"), quadraticDecay);
}

void Tlight::addUpdateFunction(function<void(Tlight&, GLfloat)> foo){ update_functions.pb(foo); }
void Tlight::update(GLfloat dTime){ for(auto &foo : update_functions) foo(*this, dTime); }
