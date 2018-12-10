#include <scene.hpp>
#include <GLmanager.hpp>
#include <inputManager.hpp>
#include <shaderProgram.hpp>

//constructors
Tscene::Tscene(const string &n):name{n}{}
Tscene::~Tscene(){ destruct(); }

//private functions
void Tscene::destruct(){
	objects.clear();
}
void Tscene::construct(const string &filename){
	TdataFile scn(filename, "scene", ".scn");

	WARN(1);
	if(scn.floats.count("scene_scale"))
		the_scene_scale = glm::scale(glm::mat4(1.0f), makeVec3(scn.floats["scene_scale"]));

	WARN(2);
	if(scn.floats.count("scene_bg_color")){
		auto color = makeVec4(scn.floats["scene_bg_color"]);
		glClearColor(color.x, color.y, color.z, color.w);
	}

	WARN(3);
	the_time = rand() % 1000000;

	for(TdataFile &funcData : scn.sons["functions"]){
		auto &name = funcData.strings["name"][0];
		if(name == "time_control"){
			auto &strs = funcData.strings;

			if(strs["start"][0] == "rand") the_time = rand() % 1000000;

			auto foo = [](string s) -> pair<bool, string> {
				if(s.substr(0, 4) == "char") return make_pair(true,  s.substr(4));
				if(s.substr(0, 3) == "key")  return make_pair(false, s.substr(3));
				return make_pair(false, string());
			};
			auto pause = foo(strs["pause"][0]);
			auto unpause = foo(strs["unpause"][0]);
			auto speed_up = foo(strs["speed_up"][0]);
			auto speed_down = foo(strs["speed_down"][0]);

			bool display = (strs["display"][0] != "false");

			auto upd = [this, pause, unpause, speed_up, speed_down, display](Tscene &scn, GLfloat dTime){
				static bool time_paused = false;
				static int time_speed = 100;

				if(not time_paused and pause.ff and inputManager::onKeyClick(pause.ss[0])){
					time_paused = true;
				}
				else if(time_paused and unpause.ff and inputManager::onKeyClick(unpause.ss[0])){
					time_paused = false;
				}
				if(speed_up.ff and inputManager::isKeyDown(speed_up.ss[0])){
					time_speed = min(time_speed+5, 10000);
					time_paused = false;
				}
				if(speed_down.ff and inputManager::isKeyDown(speed_down.ss[0])){
					if(time_speed == 5) time_paused = true;
					time_speed = max(time_speed-5, 5);
				}
				if(not time_paused) the_time += (dTime * time_speed) / 100.0f;
			};
			addUpdateFunction(upd);
		}
	}

	WARN(4);
	for(TdataFile &objData : scn.sons["objects"]){
		objects.emplace(
			piecewise_construct,
			forward_as_tuple(objData.strings.at("name")[0]),
			forward_as_tuple(objData)
		);
	}

	WARN(5);
	for(auto &[name, obj] : objects){
		if(not obj.getFather().empty()){
			objects[obj.getFather()].insertSon(name);
		}
	}
	WARN(6);
}

//public functions
void Tscene::load(const string &filename){
	destruct();
	construct(filename);
}
void Tscene::draw(){
	for(const auto &[_, obj] : objects) if(obj.getFather().empty()) obj.draw();
}

void Tscene::update(const GLfloat &dTime){
	for(auto &foo : update_functions) foo(*this, dTime);
	for(auto &[_, obj] : objects) if(obj.getFather().empty()) obj.update(dTime);
	for(auto &light : lights) light.update(dTime);
	for(auto &[_, shader] : TshaderProgram::manager.resources){
		// FOR(i, 0, lights.size()) lights[i].set(*shader, "pLight");
		FOR(i, 0, lights.size()) lights[i].set(*shader, string("lights[") + to_string(i) + string("]"));
		shader->setUniform<int>("lightCount", lights.size());
		shader->setUniform("iTime", glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
		shader->setUniform("the_time", the_time);
	}
}

void Tscene::addUpdateFunction(const function<void(Tscene&, GLfloat)> &foo){ update_functions.pb(foo); }
Tlight& Tscene::addLight(const Tlight &light){ lights.push_back(light); return lights.back(); }
Tlight& Tscene::addLight(const TdataFile &lightData){ return lights.emplace_back(lightData); }

Tobject& Tscene::getObj(const string &objName){
	const auto &it = objects.find(objName);
	if(it == objects.end())
		throw invalid_argument("Invalid object name '" + objName + "' in Tscene::getObj");
	return it->ss;
}

const Tobject& Tscene::getObj(const string &objName) const{
	const auto &it = objects.find(objName);
	if(it == objects.end())
		throw invalid_argument("Invalid object name '" + objName + "' in Tscene::getObj");
	return it->ss;
}

map<string, Tobject>& Tscene::getObjs(){ return objects; }

const map<string, Tobject>& Tscene::getObjs() const{ return objects; }
