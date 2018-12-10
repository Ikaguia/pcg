#ifndef HEADER_COMMON
#define HEADER_COMMON

#include <bits/stdc++.h>
using namespace std;

#define FOR(cont,start,end)	for(int (cont) = (start), cont##_END = (end);   (cont) <  cont##_END; (cont)++)
#define ROF(cont,start,end)	for(int (cont) = (end)-1, cont##_END = (start); (cont) >= cont##_END; (cont)--)
#define FORIT(iter,obj)		for(auto (it) = (obj).begin(); (it) != (obj).end(); (it)++)
#define LOG(x)				(31 - __builtin_clz(x))

#define elif else if

#define pb					emplace_back
#define ff					first
#define ss					second
// #define sz(v)				ll(v.size())
#define allIT(x)				x.begin(), x.end()
#define ___ << " " <<

using ll = long long;
using llu = unsigned ll;
using uint = unsigned int;
using uchar = unsigned char;
using ii = pair<int,int>;

#include <GL/glew.h>
#include <GL/freeglut.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp>


#ifdef DEBUG
#define WARN(s)	cerr << colorRed << "'" << __PRETTY_FUNCTION__ << "': " << s << colorReset << endl
#define W_IN()	WARN("entering")
#define W_OUT()	WARN("leaving")
#define W(x)	cerr << colorRed << #x << " = '" << x << "'" << colorReset << endl
#else
#define WARN(s)
#define W_IN()
#define W_OUT()
#define W(x)
#endif






constexpr float PI = atan(1.0f) * 4.0f;
constexpr float degToRadProp = (PI * 2.0f) / 360.0f;
constexpr ll oo = 1e9;
constexpr ll ep = 1e-9;
constexpr char key_enter = '\x0d';
constexpr char key_escape = '\x1b';
const string colorReset = "\x1b[0m";
const string colorRed = "\x1b[31m";
#ifdef DATA_PATH
const string data_path = DATA_PATH;
#else
const string data_path = "";
#endif


extern GLuint g_tmp;





ostream& operator<<(ostream& is, const glm::mat4 m);
ostream& operator<<(ostream& is, const glm::mat3 m);
ostream& operator<<(ostream& is, const glm::mat2 m);
ostream& operator<<(ostream& is, const glm::vec4 v);
ostream& operator<<(ostream& is, const glm::vec3 v);
ostream& operator<<(ostream& is, const glm::vec2 v);
template<typename T> ostream& operator<<(ostream& is, const vector<T> v){
	for(const auto &it : v) is << it << " ";
	return is;
}
template<typename T1, typename T2> ostream& operator<<(ostream& is, const map<T1,T2> m){
	for(const auto &it : m) is << it << " ";
	return is;
}
template<typename T1, typename T2> ostream& operator<<(ostream& is, const pair<T1,T2> p){
	is << p.ff << "," << p.ss;
	return is;
}
//erases all elements of obj, for which foo return false
template<typename T> void filter_container(T &obj, function<bool(typename T::iterator iter)> foo){
	for(typename T::iterator iter = obj.begin(); iter != obj.end();){
		if(not foo(iter)) iter = obj.erase(iter);
		else iter++;
	}
}
glm::vec1 makeVec1(const vector<float> &v);
glm::vec2 makeVec2(const vector<float> &v);
glm::vec3 makeVec3(const vector<float> &v);
glm::vec4 makeVec4(const vector<float> &v);
glm::mat2 makeMat2(const vector<vector<float>> &m);
glm::mat3 makeMat3(const vector<vector<float>> &m);
glm::mat4 makeMat4(const vector<vector<float>> &m);
GLfloat degToRad(const GLfloat &ang);
GLfloat radToDeg(const GLfloat &ang);

GLfloat operator "" _to_rad(long double ang);


#endif//HEADER_COMMON
