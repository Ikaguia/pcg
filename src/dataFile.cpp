#include <dataFile.hpp>


FILE* TdataFile::read(FILE* in){
	char *name = nullptr, *str = nullptr, *str2 = nullptr, type;
	int i1;
	float f1;

	while(fscanf(in, " %ms", &name) == 1){
		if(string(name) == "]"){
			free(name);
			name = nullptr;
			break;
		}
		if((name[0] == name[1] and name[0] == '/') or string(name) == "}"){
			free(name);
			name = nullptr;
			continue;
		}
		fscanf(in, " = %c", &type);
		switch(type){
			case '['://datafield
				try{
					TdataFile a;
					in = a.read(in);
					sons[name].push_back(move(a));
				}
				catch(exception &e){
					cerr << e.what() << endl;
					throw e;
				}
				break;
			case '\"'://string
				if(fscanf(in, "%m[^\"]\"", &str) != 1)
					throw invalid_argument("DataFile string missmatch");
				strings[name].pb(str);
				free(str);
				str = nullptr;
				break;
			case 'i'://vec int
				if(fscanf(in, " { %m[^}]s }", &str) != 1)
					throw invalid_argument("DataFile int vector missmatch");
				str2 = str;
				while(sscanf(str2, " %d", &i1) == 1){
					ints[name].push_back(i1);
					while(*str2 and *str2 != ',') str2++;
					if(*str2) str2++;
					else break;
				}
				free(str);
				str = nullptr;
				break;
			case 'f'://vec float
				if(fscanf(in, " { %m[^}]s }", &str) != 1)
					throw invalid_argument("DataFile float vector missmatch");
				str2 = str;
				while(sscanf(str2, " %f", &f1) == 1){
					floats[name].push_back(f1);
					while(*str2 and *str2 != ',') str2++;
					if(*str2) str2++;
					else break;
				}
				free(str);
				str = nullptr;
				break;
		}
		free(name);
		name = nullptr;
	}
	return in;
}


TdataFile::TdataFile(const string &fileName, const string &subfolder, const string &extension){
	FILE* file = fopen(findFile(fileName, subfolder, extension).c_str(), "r");

	read(file);

	fclose(file);
}
void TdataFile::print(uint depth)const{
	string tab;
	FOR(i, 0, depth) tab += "\t";

	// cerr << "printing ints" << endl;
	for(const auto &in : ints){
		cout << tab << in.ff << " = " << "i{ ";
		for(const auto &i : in.ss) cout << i << " ";
		cout << "}" << endl;
	}

	// cerr << "printing floats" << endl;
	for(const auto &fl : floats){
		cout << tab << fl.ff << " = " << "f{ ";
		for(const auto &f : fl.ss) cout << f << " ";
		cout << "}" << endl;
	}

	// cerr << "printing strings" << endl;
	for(const auto &st : strings){
		if(st.ss.size() == 0) continue;
		if(st.ss.size() == 1) cout << tab << st.ff << " = " << "\"" << st.ss[0] << "\"" << endl;
		else{
			cout << tab << st.ff << " = " << "s{ ";
			for(const auto s : st.ss) cout << s << " ";
			cout << "}" << endl;
		}
	}

	// cerr << "printing sons" << endl;
	for(const auto &son : sons){
		for(const auto &so : son.ss){
			cout << tab << son.ff << " = [" << endl;
			so.print(depth + 1);
			cout << tab << "]" << endl;
		}
	}
}


string TdataFile::findFile(const string &fileName, const string &subfolder, const string &extension){
	FILE *file;
	string result;

	result = (data_path + "/" + subfolder + "/" + fileName + extension);
	if((file = fopen(result.c_str(), "r"))) return fclose(file), result;

	result = (data_path + "/" + subfolder + "/" + fileName);
	if((file = fopen(result.c_str(), "r"))) return fclose(file), result;

	result = (data_path + "/" + fileName + extension);
	if((file = fopen(result.c_str(), "r"))) return fclose(file), result;

	result = (data_path + "/" + fileName);
	if((file = fopen(result.c_str(), "r"))) return fclose(file), result;

	result = (subfolder + "/" + fileName + extension);
	if((file = fopen(result.c_str(), "r"))) return fclose(file), result;

	result = (subfolder + "/" + fileName);
	if((file = fopen(result.c_str(), "r"))) return fclose(file), result;

	result = (fileName + extension);
	if((file = fopen(result.c_str(), "r"))) return fclose(file), result;

	result = (fileName);
	if((file = fopen(result.c_str(), "r"))) return fclose(file), result;


	string err;
	err += "Error opening file:\n";
	err += "\t fileName = '" + fileName + "'\n";
	err += "\t subfolder = '" + subfolder + "'\n";
	err += "\t extension = '" + extension + "'\n";
	throw invalid_argument(err);
}


// example
// int main(){
// 	auto cube = DataFile::read("cube", "mesh", ".mesh");

// 	cube.print();
// }
