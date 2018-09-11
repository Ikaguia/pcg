#ifndef HEADER_GLMANAGER
#define HEADER_GLMANAGER

#include <common.hpp>

namespace GLmanager{
	constexpr float PI = atan(1.0f) * 4.0f;
	constexpr float degToRadProp = (PI * 2.0f) / 360.0f;

	void run(int argc, char **argv);
	glm::vec1 makeVec1(const vector<float> &v);
	glm::vec2 makeVec2(const vector<float> &v);
	glm::vec3 makeVec3(const vector<float> &v);
	glm::vec4 makeVec4(const vector<float> &v);
	glm::mat2 makeMat2(const vector<vector<float>> &m);
	glm::mat3 makeMat3(const vector<vector<float>> &m);
	glm::mat4 makeMat4(const vector<vector<float>> &m);
	template<typename T> GLuint createBuffer(const vector<T> &vals, GLenum type, GLenum usage);
	GLfloat degToRad(const GLfloat &ang);
	GLfloat radToDeg(const GLfloat &ang);
};

#endif//HEADER_GLMANAGER
