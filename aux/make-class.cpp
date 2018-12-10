#include <bits/stdc++.h>
using namespace std;

#define FOR(cont,start,end)	for(int (cont) = (start), cont##_END = (end);   (cont) <  cont##_END; (cont)++)
#define ROF(cont,start,end)	for(int (cont) = (end)-1, cont##_END = (start); (cont) >= cont##_END; (cont)--)
#define LOG(x)				(31 - __builtin_clz(x))
#define W(x)				cerr << "\033[31m" << #x << " = " << x << "\033[0m" << endl;

#define oo 					int(1e9)
#define ep					int(1e-9)
#define pb					emplace_back
#define ff					first
#define ss					second
#define sz(v)				ll(v.size())

using ll = long long;
using llu = unsigned ll;
using uint = unsigned;
using ii = pair<int,int>;

string to_upper(string s){
	for(char &c : s) c = toupper(c);
	return s;
}

void make_hpp(string classname, string path){
	{
		ifstream file(path + "/" + classname + ".hpp");
		if(file.is_open()) throw exception();
	}

	ofstream file(path + "/" + classname + ".hpp");

	string header_class = "HEADER_" + to_upper(classname);

	classname = "T" + classname;

	file << "#ifndef " << header_class << endl;		//#ifndef HEADER_CLASS
	file << "#define " << header_class << endl;		//#define HEADER_CLASS
	file << endl;
	file << "#include <common.hpp>" << endl;		//#include <common.hpp>
	file << endl;
	file << "class " << classname << "{" << endl;	//class TCLASS{
	file << "\t//private members" << endl;			//	//private members
	file << "\tuint x;" << endl;					//	uint x;
	file << "\t//private functions" << endl;		//	//private functions
	file << "\tvoid foo();" << endl;				//	void foo();
	file << "public:" << endl;						//public:
	file << "\t//public members" << endl;			//	//public members
	file << "\tuint id;" << endl;					//	uint id;
	file << "\t//constructors" << endl;				//	//constructors
	file << "\t" << classname << "();" << endl;		//	TCLASS();
	file << "\t//public functions" << endl;			//	//public functions
	file << "\tvoid foo();" << endl;				//	void foo();
	file << "};" << endl;							//};
	file << endl;
	file << "#endif//" << header_class << endl;
}

void make_cpp(string classname, string path){
	{
		ifstream file(path + "/" + classname + ".cpp");
		if(file.is_open()) throw exception();
	}
	ofstream file(path + "/" + classname + ".cpp");

	file << "#include <" << classname << ".hpp>" << endl;	//#include <<CLASS.hpp>
	file << endl;

	classname = "T" + classname;

	file << "//static members" << endl;						////private members
	file << classname << "::uint id = 1;" << endl;			//TCLASS::uint id = 0;
	file << endl;
	file << "//constructors" << endl;						////constructors
	file << classname << "::" << classname << "(){" << endl;//TCLASS::TCLASS(){
	file << "\tid = 0;" << endl;							//	id = 0;
	file << "}" << endl;									//}
	file << endl;
	file << "//private functions" << endl;					////private functions
	file << "void " << classname << "::foo();" << endl;		//void TCLASS::foo();
	file << endl;
	file << "//public functions" << endl;					////public functions
	file << "void " << classname << "::foo();" << endl;		//void TCLASS::foo();
}


int main(int argcnt, char **args) {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	if(argcnt < 2){
		cerr << "Missing args (./--- classname [path] [hpppath]" << endl;
		exit(1);
	}

	if(argcnt == 2){
		make_cpp(args[1], ".");
		make_hpp(args[1], ".");
	}
	else if(argcnt == 3){
		make_cpp(args[1], args[2]);
		make_hpp(args[1], args[2]);
	}
	else if(argcnt == 4){
		make_cpp(args[1], args[2]);
		make_hpp(args[1], args[3]);
	}
}
