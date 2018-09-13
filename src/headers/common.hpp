#ifndef HEADER_COMMON
#define HEADER_COMMON

#include <bits/stdc++.h>
using namespace std;

#define FOR(cont,start,end)	for(int (cont) = (start), cont##_END = (end);   (cont) <  cont##_END; (cont)++)
#define ROF(cont,start,end)	for(int (cont) = (end)-1, cont##_END = (start); (cont) >= cont##_END; (cont)--)
#define LOG(x)				(31 - __builtin_clz(x))

#define pb					emplace_back
#define ff					first
#define ss					second
// #define sz(v)				ll(v.size())
#define allIT(x)				x.begin(), x.end()

const string colorReset = "\x1b[0m";
const string colorRed = "\x1b[31m";

using ll = long long;
using llu = unsigned ll;
using uint = unsigned int;
using ii = pair<int,int>;

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

const ll oo = 1e9;
const ll ep = 1e-9;
const string data_path = DATA_PATH;

#ifdef DEBUG
#define WARN(s)	cerr << colorRed << "'" << __func__ << "': " << s << colorReset << endl
#define W_IN()	WARN("entering")
#define W_OUT()	WARN("leaving")
#define W(x)	cerr << colorRed << #x << " = " << x << colorReset << endl
#else
#define WARN()
#define W_IN()
#define W_OUT()
#define W()
#endif

#endif//HEADER_COMMON
