#ifndef HEADER_INPUTMANAGER
#define HEADER_INPUTMANAGER

#include <common.hpp>

namespace inputManager{
	struct click{
		bool isDown;
		bool shift, ctrl, alt;
		GLuint clickTime, releaseTime;
		glm::vec2 clickPos, releasePos;
	};

	void init();

	void update(GLfloat dTime);

	bool isKeyDown(char key);
	bool isKeyDown(int key);
	bool isButtonDown(int b);

	bool onKeyClick(char key);
	bool onKeyClick(int key);
	bool onButtonClick(int b);

	bool onKeyRelease(char key);
	bool onKeyRelease(int key);
	bool onButtonRelease(int b);

	glm::vec2 getMousePos();

	click getKey(char key);
	click getKey(int key);
	click getButton(int b);

	void setMouseHidden(bool b);
	void setMouseLocked(bool b);
	bool getMouseHidden();
	bool getMouseLocked();

	bool isWindowsFocused();
};

#endif//HEADER_INPUTMANAGER
