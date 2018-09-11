#include <dataFile.hpp>


FILE* TdataFile::read(FILE* in){
	char *name, *str, *str2, type;
	int i1;
	float f1;

	while(fscanf(in, " %ms", &name) == 1){
		if(string(name) == "]"){
			free(name);
			break;
		}
		if((name[0] == name[1] and name[0] == '/') or string(name) == "}"){
			free(name);
			continue;
		}
		fscanf(in, " = %c", &type);
		switch(type){
			case '['://datafield
				{
					TdataFile son;
					in = son.read(in);
					sons[name].pb(son);
				}
				break;
			case '\"'://string
				fscanf(in, "%m[^\"]\"", &str);
				strings[name].pb(str);
				free(str);
				break;
			case 'i'://vec int
				fscanf(in, " {%m[^}]s}", &str);
				// W(str);
				str2 = str;
				while(sscanf(str2, " %d", &i1) == 1){
					// W(i1);
					ints[name].push_back(i1);
					while(*str2 and *str2 != ',') str2++;
					if(*str2) str2++;
				}
				free(str);
				break;
			case 'f'://vec float
				fscanf(in, " {%m[^}]s }", &str);
				str2 = str;
				while(sscanf(str2, " %f", &f1) == 1){
					floats[name].push_back(f1);
					while(*str2 and *str2 != ',') str2++;
					if(*str2) str2++;
				}
				free(str);
				break;
		}
		free(name);
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

	cerr << "Error opening file:" << endl;
	cerr << "\t fileName = '" << fileName << "'" << endl;
	cerr << "\t subfolder = '" << subfolder << "'" << endl;
	cerr << "\t extension = '" << extension << "'" << endl;
	cerr << endl;
	exit(1);
}


// example
// int main(){
// 	auto cube = DataFile::read("cube", "mesh", ".mesh");

// 	cube.print();
// }
