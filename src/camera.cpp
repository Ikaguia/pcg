#include <camera.hpp>
#include <GLmanager.hpp>
#include <object.hpp>
#include <scene.hpp>
#include <inputManager.hpp>

Tobject& Tcamera::getTarget(){
	auto &objs = GLmanager::scene->getObjs();
	if(not objs.size()) throw exception();
	if(not objs.count(target)) the_target = objs.begin()->ff;
	return objs[target];
}

void Tcamera::setMode(Emode m){ the_mode = m; }
void Tcamera::update(GLfloat dTime){
	bool change = false;
	// auto &objs = GLmanager::scene->getObjs();
	if(inputManager::onKeyClick('\t')){
		if     (mode == Emode::follow_mode) the_mode = Emode::free_mode;
		else if(mode == Emode::free_mode)   the_mode = Emode::follow_mode;
		change = true;
	}
	else if(mode == Emode::follow_mode){
		const auto &objs = GLmanager::scene->getObjs();
		try{
			getTarget();

			if(inputManager::onKeyClick('o')){
				auto nxt = next(objs.find(target));
				if(nxt == objs.end()) the_target = objs.begin()->ff;
				else the_target = nxt->ff;
				W(target);
			}
			if(inputManager::isKeyDown(GLUT_KEY_UP))   setLatitude(latitude + 10.0_to_rad * dTime / 100.0f);
			if(inputManager::isKeyDown(GLUT_KEY_DOWN)) setLatitude(latitude - 10.0_to_rad * dTime / 100.0f);
			if(inputManager::isKeyDown(GLUT_KEY_LEFT))  setLongitude(longitude - 10.0_to_rad * dTime / 100.0f);
			if(inputManager::isKeyDown(GLUT_KEY_RIGHT)) setLongitude(longitude + 10.0_to_rad * dTime / 100.0f);
			if(inputManager::isKeyDown(GLUT_KEY_PAGE_DOWN)) setAltitude(altitude + 0.5f * dTime / 100.0f);
			if(inputManager::isKeyDown(GLUT_KEY_PAGE_UP))   setAltitude(altitude - 0.5f * dTime / 100.0f);

			if(inputManager::isWindowsFocused()){
				setLatitude(latitude - 90.0_to_rad * (0.5f - (inputManager::getMousePos().y / glutGet(GLUT_WINDOW_HEIGHT))));
				setLongitude(longitude + 180.0_to_rad * (0.5f - (inputManager::getMousePos().x / glutGet(GLUT_WINDOW_WIDTH))));
			}
			if(inputManager::isButtonDown(3)) setAltitude(altitude + 0.5f * dTime / 100.0f);
			if(inputManager::isButtonDown(4)) setAltitude(altitude - 0.5f * dTime / 100.0f);

			change = true;
		}
		catch(exception &e){ (void)e; }
	}
	else if(mode == Emode::free_mode){
		static map<int, glm::vec3> move1 = {
			{GLUT_KEY_UP,		glm::vec3{ 0, 0,-1}},
			{GLUT_KEY_DOWN,		glm::vec3{ 0, 0, 1}},
			{GLUT_KEY_RIGHT,	glm::vec3{ 1, 0, 0}},
			{GLUT_KEY_LEFT,		glm::vec3{-1, 0, 0}},
			{GLUT_KEY_PAGE_UP,	glm::vec3{ 0, 1, 0}},
			{GLUT_KEY_PAGE_DOWN,glm::vec3{ 0,-1, 0}},
		};
		static map<char, glm::vec3> move2 = {
			{'w',				glm::vec3{ 0, 1, 0}},
			{'a',				glm::vec3{-1, 0, 0}},
			{'s',				glm::vec3{ 0,-1, 0}},
			{'d',				glm::vec3{ 1, 0, 0}},
			{'q',				glm::vec3{ 0, 0,-1}},
			{'e',				glm::vec3{ 0, 0, 1}},
		};
		for(auto &it : move1){
			const auto &click = inputManager::getKey(it.ff);
			if(click.isDown){
				change = true;
				the_translation += (it.ss * (click.shift ? 1.0f : 0.1f));
				the_translation.y = max(the_translation.y, 0.1f);
			}
		}
		for(auto &it : move2){
			const auto click = inputManager::getKey(it.ff);
			if(click.isDown){
				change = true;
				the_rotation += (it.ss * (click.shift ? 1.5f : 0.15f));
			}
		}
	}
	if(change){
		// recalculate view matrix with new values
		const glm::mat4 &view = GLmanager::calcViewMatrix();
		//set the shader programs to use the new matrix
		for(auto &[_, prog] : TshaderProgram::manager.resources){
			prog->setUniform("viewMatrix", view);
			prog->setUniform("cameraPosition", glm::vec4(translation, 1.0f));
		}
		glUseProgram(0);
	}
}
glm::mat4 Tcamera::getViewMatrix(){
	if(mode == Emode::follow_mode){
		try{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, longitude, glm::vec3(0, 1, 0));
			model = glm::rotate(model, latitude, glm::vec3(0, 0, 1));
			model = glm::translate(model, glm::vec3(altitude, 0, 0));

			model = getTarget().getAbsoluteModelMatrix() * model;

			the_translation = (model * glm::vec4(0, 0, 0, 1)).xyz();

			return glm::lookAt(translation, getTarget().getAbsolutePosition(), glm::vec3(0, 1, 0));
		}
		catch(exception &e){
			(void)e;
			return glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		}
	}
	else{
		return glm::lookAt(
			translation,
			translation + (glm::vec4(0, -1, -10, 1) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0))).xyz(),
			glm::vec3(0, 1, 0)
			);
	}
}

//follow mode
void Tcamera::follow(const string &t){ the_target = t; }
void Tcamera::matchRotation(bool b){ the_follow_rotation = b; }
void Tcamera::setLongitude(GLfloat l){ the_longitude = fmod(360.0_to_rad + l, 360.0_to_rad); }
void Tcamera::setLatitude(GLfloat l){ the_latitude = min( 60.0_to_rad, max(-60.0_to_rad, l)); }
void Tcamera::setAltitude(GLfloat a){ the_altitude = min( 100.0f, max(1.1f, a)); }

//free mode
void Tcamera::setTranslation(const glm::vec3 &t){ the_translation = t; }
void Tcamera::setRotation(const glm::vec3 &r){ the_rotation = r; }
