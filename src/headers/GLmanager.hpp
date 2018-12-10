#ifndef HEADER_GLMANAGER
#define HEADER_GLMANAGER

#include <common.hpp>

class Tscene;

namespace GLmanager{
	extern bool isGlInitialized;
	extern bool isGlewInitialized;
	extern bool isGlutInitialized;
	extern Tscene* scene;

	void run(int argc, char **argv);

	glm::mat4 calcClipMatrix();
	glm::mat4 calcViewMatrix(glm::mat4 view = glm::mat4(1.0f));

	glm::vec3 getViewportResolution();

	template<typename T> GLuint createBuffer(const vector<T> &vals, GLenum type, GLenum usage){
		GLuint buffer;
		glGenBuffers(1, &buffer);

		glBindBuffer(type, buffer);
		glBufferData(type, vals.size() * sizeof(T), vals.data(), usage);
		glBindBuffer(type, 0);

		return buffer;
	}
};

#endif//HEADER_GLMANAGER
