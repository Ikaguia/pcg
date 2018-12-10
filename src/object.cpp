#include <object.hpp>
#include <GLmanager.hpp>
#include <texture.hpp>
#include <mesh.hpp>
#include <scene.hpp>

//constructors
Tobject::Tobject(const string &thename, const TdataFile &objData):name{thename}{
	construct(objData);
}
Tobject::Tobject(const TdataFile &objData):name{objData.strings.at("name")[0]}{
	construct(objData);
}
Tobject::Tobject(const string &thename):name{thename}{}
Tobject::~Tobject(){ destruct(); }

//private functions
void Tobject::construct(const TdataFile &objectData){
	if(objectData.strings.count("father")){
		const string &fatherName = objectData.strings.at("father")[0];
		if(fatherName == name) setFather("");
		else setFather(fatherName);
	}
	else setFather("");

	if(objectData.floats.count("translation"))
		setTranslation(makeVec3(objectData.floats.at("translation")));
	if(objectData.floats.count("rotation"))
		setRotation(glm::vec3(
			glm::radians(objectData.floats.at("rotation")[0]),
			glm::radians(objectData.floats.at("rotation")[1]),
			glm::radians(objectData.floats.at("rotation")[2])
		));
	if(objectData.floats.count("scale")){
		if(objectData.floats.at("scale").size() == 1){
			GLfloat val = objectData.floats.at("scale")[0];
			setScale(glm::vec3(val, val, val));
		}
		else setScale(makeVec3(objectData.floats.at("scale")));
	}
	if(objectData.floats.count("color"))
		setColor(makeVec3(objectData.floats.at("color")));

	if(objectData.strings.count("mesh"))
		mesh = Tmesh::manager.get(objectData.strings.at("mesh")[0]);
	if(objectData.strings.count("shaderProg"))
		shaderProgram = TshaderProgram::manager.get(objectData.strings.at("shaderProg")[0]);
	if(objectData.sons.count("material"))
		material = unique_ptr<Tmaterial>(new Tmaterial(objectData.sons.at("material")[0]));

	if(objectData.sons.count("functions")){
		for(auto &funcData : objectData.sons.at("functions")){
			if(funcData.strings.at("name")[0] == "orbit"){
				GLfloat period = 1.0f, distance = 1.0f, day = 0.0f, offset = 0.0f;
				glm::vec3 orbitRotation;

				if(funcData.floats.count("period")) period = funcData.floats.at("period")[0];
				if(funcData.floats.count("distance")) distance = funcData.floats.at("distance")[0];
				if(funcData.floats.count("rotation")) orbitRotation.x = funcData.floats.at("rotation")[0];
				if(funcData.floats.count("rotation")) orbitRotation.y = funcData.floats.at("rotation")[1];
				if(funcData.floats.count("rotation")) orbitRotation.z = funcData.floats.at("rotation")[2];
				if(funcData.floats.count("day")) day = funcData.floats.at("day")[0];
				if(funcData.floats.count("offset")) offset = funcData.floats.at("offset")[0];

				auto foo = [period, distance, day, orbitRotation, offset](Tobject& obj, GLfloat){
					const auto &scene = *GLmanager::scene;
					const auto curTime = scene.time / 1000;
					if(not obj.getFather().empty()){
						try{

							obj.setRotation(glm::vec3(0.0f, 360.0f * fmod((curTime / period) + offset, 1.0f), 0.0f));

							glm::mat4 curOrbitPos = glm::mat4(1.0f);

							curOrbitPos = glm::rotate(curOrbitPos, glm::radians(orbitRotation.x), glm::vec3(1, 0, 0) );
							curOrbitPos = glm::rotate(curOrbitPos, glm::radians(orbitRotation.y), glm::vec3(0, 1, 0) );
							curOrbitPos = glm::rotate(curOrbitPos, glm::radians(orbitRotation.z), glm::vec3(0, 0, 1) );

							curOrbitPos = glm::rotate(curOrbitPos, obj.getRotation().y, glm::vec3(0, 1, 0));

							// W(orbitRotation);

							curOrbitPos = glm::translate(curOrbitPos, glm::vec3(distance, 0.0f, 0.0f));

							obj.setTranslation((curOrbitPos*glm::vec4(0,0,0,1)).xyz());

							// if(day){
							// 	obj.setRotation(glm::vec3(
							// 		0.0f,
							// 		obj.getRotation().y + glm::radians(360.0f) * fmod(curTime, day) / day,
							// 		0.0f
							// 	));
							// }
						}
						catch(exception &e){ (void)e; }
					}
					else if(day) obj.setRotation(glm::vec3(0.0f, glm::radians(360.0f) * fmod(curTime, day) / day, 0.0f));
				};
				addUpdateFunction(foo);
				update(0);
			}
			if(funcData.strings.at("name")[0] == "point-light"){
				Tlight light(funcData);
				auto target = this;
				light.addUpdateFunction([target](Tlight &l, GLfloat dTime){
					l.position = glm::vec4(target->getAbsolutePosition(), 1.0f);
				});
				light.update(0);
				GLmanager::scene->addLight(light);
			}
		}
	}

	calcRelativeModelMatrix();
	calcAbsoluteModelMatrix();
}
void Tobject::destruct(){
}

//public functions
void Tobject::setFather(const string &f){
	if(f == father) return;
	needs_recalc = true;
	father = f;
}
void Tobject::setSons(const set<string> &s){ needs_recalc = true; sons = s; }
void Tobject::insertSon(const string &s){ needs_recalc = true; sons.insert(s); }

void Tobject::setTranslation(const glm::vec3 &t){ needs_recalc = true; translation = t; }
void Tobject::setRotation(const glm::vec3 &r){ needs_recalc = true; rotation = r; }
void Tobject::setScale(const glm::vec3 &s){ needs_recalc = true; scale = s; }
void Tobject::setColor(const glm::vec3 &c){ objectColor = c; }

// void Tobject::setMesh();
// void Tobject::setShaderProgram();
// void Tobject::setMaterial();

void Tobject::addUpdateFunction(const function<void(Tobject&, GLfloat)> &foo){ update_functions.pb(foo); }
// void Tobject::clearUpdateFunctions();


void Tobject::draw() const{
	if(mesh){
		if(not shaderProgram){
			W(name);
			W(shaderProgram);
			return;
		}

		//draw
		shaderProgram->use();
		shaderProgram->setUniform("modelMatrix", getAbsoluteModelMatrix());
		shaderProgram->setUniform("objectColor", getColor());
		if(material) shaderProgram->setMaterial(*material.get());
		mesh->draw();
		glUseProgram(0);
	}
	auto &objs = GLmanager::scene->getObjs();
	for(const string &son : sons) objs[son].draw();

}
void Tobject::update(const GLfloat &dTime){
	//update
	for(auto foo : update_functions) foo(*this, dTime);

	auto &objs = GLmanager::scene->getObjs();

	if(needs_recalc){
		auto old = getAbsoluteModelMatrix();
		calcRelativeModelMatrix();
		calcAbsoluteModelMatrix();
		if(old != getAbsoluteModelMatrix()){
			for(const string &son : sons) objs[son].needs_recalc = true;
		}
		needs_recalc = false;
	}

	for(const string &son : sons) objs[son].update(dTime);
}

void Tobject::calcAbsoluteModelMatrix(){
	if(getFather() == name or getFather() == "") absoluteModelMatrix = relativeModelMatrix;
	else{
		const auto &fatherObj = GLmanager::scene->getObj(getFather());

		if(fatherModelMatrix == fatherObj.getAbsoluteModelMatrix()) return;
		fatherModelMatrix = fatherObj.getAbsoluteModelMatrix();
		absoluteModelMatrix = fatherModelMatrix * relativeModelMatrix;
		// absoluteModelMatrix = relativeModelMatrix * fatherModelMatrix;
	}
}
void Tobject::calcRelativeModelMatrix(){
	relativeModelMatrix = glm::mat4(1.0f);
	//translation
	relativeModelMatrix = glm::translate(relativeModelMatrix, translation);
	//rotation
	relativeModelMatrix = glm::rotate(relativeModelMatrix, rotation.x, glm::vec3(1, 0, 0));
	relativeModelMatrix = glm::rotate(relativeModelMatrix, rotation.y, glm::vec3(0, 1, 0));
	relativeModelMatrix = glm::rotate(relativeModelMatrix, rotation.z, glm::vec3(0, 0, 1));
	//scale
	relativeModelMatrix = glm::scale(relativeModelMatrix, scale);
}


const string& Tobject::getFather() const{ return father; }
const set<string>& Tobject::getSons() const{ return sons; }

const glm::vec3& Tobject::getTranslation() const{ return translation; }
const glm::vec3& Tobject::getRotation() const{ return rotation; }
const glm::vec3& Tobject::getScale() const{ return scale; }
const glm::vec3& Tobject::getColor() const{ return objectColor; }

const glm::mat4& Tobject::getAbsoluteModelMatrix() const{ return absoluteModelMatrix; }
const glm::mat4& Tobject::getRelativeModelMatrix() const{ return relativeModelMatrix; }
glm::vec3 Tobject::getAbsolutePosition() const{ return (getAbsoluteModelMatrix() * glm::vec4(0, 0, 0, 1)).xyz(); }
