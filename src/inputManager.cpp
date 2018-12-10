#include <inputManager.hpp>
#include <shaderProgram.hpp>

namespace inputManager{
	namespace {
		map<uchar, click> keys;
		map<GLenum, click> special_keys;
		map<GLenum, click> buttons;
		glm::vec2 mousePos;
		GLfloat lastUpd = -1;
		bool mouseHidden = false, mouseLocked = false, windowsFocused = false;

		void keyboard(uchar key, int x, int y){
			key = tolower(key);
			if(keys[key].isDown) return;
			keys[key].isDown = true;
			keys[key].clickTime = glutGet(GLUT_ELAPSED_TIME);
			keys[key].clickPos = glm::vec2(x,y);
			keys[key].shift = glutGetModifiers() & GLUT_ACTIVE_SHIFT;
			keys[key].ctrl = glutGetModifiers() & GLUT_ACTIVE_CTRL;
			keys[key].alt = glutGetModifiers() & GLUT_ACTIVE_ALT;
		}
		void keyboardUp(uchar key, int x, int y){
			key = tolower(key);
			if(not keys[key].isDown) return;
			keys[key].isDown = false;
			keys[key].releaseTime = glutGet(GLUT_ELAPSED_TIME);
			keys[key].releasePos = glm::vec2(x,y);
		}
		void keyboard_special(int key, int x, int y){
			if(special_keys[key].isDown) return;
			special_keys[key].isDown = true;
			special_keys[key].clickTime = glutGet(GLUT_ELAPSED_TIME);
			special_keys[key].clickPos = glm::vec2(x,y);
			special_keys[key].shift = glutGetModifiers() & GLUT_ACTIVE_SHIFT;
			special_keys[key].ctrl = glutGetModifiers() & GLUT_ACTIVE_CTRL;
			special_keys[key].alt = glutGetModifiers() & GLUT_ACTIVE_ALT;
		}
		void keyboardUp_special(int key, int x, int y){
			if(not special_keys[key].isDown) return;
			special_keys[key].isDown = false;
			special_keys[key].releaseTime = glutGet(GLUT_ELAPSED_TIME);
			special_keys[key].releasePos = glm::vec2(x,y);
		}
		void mouse(int b, int state, int x, int y){
			if(state == GLUT_DOWN){
				if(buttons[b].isDown) return;
				buttons[b].isDown = true;
				buttons[b].clickTime = glutGet(GLUT_ELAPSED_TIME);
				buttons[b].clickPos = glm::vec2(x,y);
				buttons[b].shift = glutGetModifiers() & GLUT_ACTIVE_SHIFT;
				buttons[b].ctrl = glutGetModifiers() & GLUT_ACTIVE_CTRL;
				buttons[b].alt = glutGetModifiers() & GLUT_ACTIVE_ALT;
			}
			else{
				if(not buttons[b].isDown) return;
				buttons[b].isDown = false;
				buttons[b].releaseTime = glutGet(GLUT_ELAPSED_TIME);
				buttons[b].releasePos = glm::vec2(x,y);
			}
		}
		void mouseMotion(int x, int y){
			mousePos.x = x;
			mousePos.y = y;
		}
		void entryFunc(int state){
			if(state == GLUT_LEFT) windowsFocused = false;
			else if(state == GLUT_ENTERED){
				windowsFocused = true;
				if(mouseLocked){
					mousePos = glm::vec2(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
					glutWarpPointer(mousePos.x, mousePos.y);
				}
			}
			else throw exception();
		}
	}
	void init(){
		glutKeyboardFunc(keyboard);
		glutKeyboardUpFunc(keyboardUp);
		glutSpecialFunc(keyboard_special);
		glutSpecialUpFunc(keyboardUp_special);
		glutMouseFunc(mouse);
		glutMotionFunc(mouseMotion);
		glutPassiveMotionFunc(mouseMotion);
		glutEntryFunc(entryFunc);
	}
	void update(GLfloat dTime){
		if(mouseLocked){
			mousePos = glm::vec2(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
			if(windowsFocused) glutWarpPointer(mousePos.x, mousePos.y);
		}
		lastUpd = glutGet(GLUT_ELAPSED_TIME);
		for(auto &[_, sh] : TshaderProgram::manager.resources)
			sh->setUniform("iMouse", glm::vec4(mousePos.x, mousePos.y, 0.0f, 0.0f));
	}

	bool isKeyDown(char key){ return keys[tolower(key)].isDown; }
	bool isKeyDown(int key){ return special_keys[key].isDown; }
	bool isButtonDown(int b){ return buttons[b].isDown; }

	bool onKeyClick(char key){
		click c = getKey(key);
		return (c.isDown and (c.clickTime == lastUpd));
	}
	bool onKeyClick(int key){
		click c = getKey(key);
		return (c.isDown and (c.clickTime == lastUpd));
	}
	bool onButtonClick(int b){
		click c = getButton(b);
		return (c.isDown and (c.clickTime == lastUpd));
	}

	bool onKeyRelease(char key){
		click c = getKey(key);
		return (not c.isDown and (c.releaseTime == lastUpd));
	}
	bool onKeyRelease(int key){
		click c = getKey(key);
		return (not c.isDown and (c.releaseTime == lastUpd));
	}
	bool onButtonRelease(int b){
		click c = getButton(b);
		return (not c.isDown and (c.releaseTime == lastUpd));
	}

	glm::vec2 getMousePos(){ return mousePos; }

	click getKey(char key){ return keys[tolower(key)]; }
	click getKey(int key){ return special_keys[key]; }
	click getButton(int b){ return buttons[b]; }


	void setMouseHidden(bool b){ mouseHidden = b; glutSetCursor(mouseHidden ? GLUT_CURSOR_NONE : GLUT_CURSOR_INHERIT); }
	void setMouseLocked(bool b){ mouseLocked = b; }
	bool getMouseHidden(){ return mouseHidden; }
	bool getMouseLocked(){ return mouseLocked; }

	bool isWindowsFocused(){ return windowsFocused; }
}
