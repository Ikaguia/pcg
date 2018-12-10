#ifndef HEADER_CAMERA
#define HEADER_CAMERA

#include <common.hpp>

class Tobject;

class Tcamera{
public:
	enum Emode{
		follow_mode,
		free_mode,
	};
private:
	//private members
	Emode the_mode = Emode::follow_mode;

	//follow mode
	string the_target = "";
	bool the_follow_rotation = false;
	GLfloat the_longitude = 0.0f, the_latitude = 0.0f, the_altitude = 5.0f;

	//free mode
	glm::vec3 the_translation;
	glm::vec3 the_rotation;


	//private functions
	Tobject& getTarget();
	const Tobject& getTarget() const;
public:
	//public members
	const Emode &mode = the_mode;

	//follow mode
	const string &target = the_target;
	const bool &follow_rotation = the_follow_rotation;
	const GLfloat &longitude = the_longitude;
	const GLfloat &latitude = the_latitude;
	const GLfloat &altitude = the_altitude;

	//free mode
	const glm::vec3 &translation = the_translation;
	const glm::vec3 &rotation = the_rotation;

	//public functions
	void setMode(Emode m);
	void update(GLfloat dTime);
	glm::mat4 getViewMatrix();

	//follow mode
	void follow(const string &t);
	void matchRotation(bool b);
	void setLongitude(GLfloat l);
	void setLatitude(GLfloat l);
	void setAltitude(GLfloat a);

	//free mode
	void setTranslation(const glm::vec3 &t);
	void setRotation(const glm::vec3 &r);
};

#endif//HEADER_CAMERA
