#include <common.hpp>

GLuint g_tmp;

ostream& operator<<(ostream& is, const glm::mat4 m){
	is << m[0] << endl;
	is << m[1] << endl;
	is << m[2] << endl;
	is << m[3];
	return is;
}
ostream& operator<<(ostream& is, const glm::mat3 m){
	is << m[0] << endl;
	is << m[1] << endl;
	is << m[2] << endl;
	return is;
}
ostream& operator<<(ostream& is, const glm::mat2 m){
	is << m[0] << endl;
	is << m[1] << endl;
	return is;
}
ostream& operator<<(ostream& is, const glm::vec4 v){
	is << "[" ___ v.x ___ v.y ___ v.z ___ v.w ___ "]";
	return is;
}
ostream& operator<<(ostream& is, const glm::vec3 v){
	is << "[" ___ v.x ___ v.y ___ v.z ___ "]";
	return is;
}
ostream& operator<<(ostream& is, const glm::vec2 v){
	is << "[" ___ v.x ___ v.y ___ "]";
	return is;
}

glm::vec1 makeVec1(const vector<GLfloat> &v){ return glm::vec1{ v[0] }; }
glm::vec2 makeVec2(const vector<GLfloat> &v){ return glm::vec2{ v[0], v[1] }; }
glm::vec3 makeVec3(const vector<GLfloat> &v){ return glm::vec3{ v[0], v[1], v[2] }; }
glm::vec4 makeVec4(const vector<GLfloat> &v){ return glm::vec4{ v[0], v[1], v[2], v[3] }; }
glm::mat2 makeMat2(const vector<vector<GLfloat>> &m){
	return glm::mat2{ makeVec2(m[0]), makeVec2(m[1]) };
}
glm::mat3 makeMat3(const vector<vector<GLfloat>> &m){
	return glm::mat3{ makeVec3(m[0]), makeVec3(m[1]), makeVec3(m[2]) };
}
glm::mat4 makeMat4(const vector<vector<GLfloat>> &m){
	return glm::mat4{ makeVec4(m[0]), makeVec4(m[1]), makeVec4(m[2]), makeVec4(m[3]) };
}
GLfloat degToRad(const GLfloat &ang){ return glm::radians(ang); }
GLfloat radToDeg(const GLfloat &ang){ return ang / degToRadProp; }

GLfloat operator "" _to_rad(long double ang){ return degToRad(ang); }
