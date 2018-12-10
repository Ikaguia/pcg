#ifndef HEADER_DATAFILE
#define HEADER_DATAFILE

#include <common.hpp>

class TdataFile{
	FILE* read(FILE*);
public:
	map<string, vector<TdataFile>> sons;
	map<string, vector<string>> strings;
	map<string, vector<int>> ints;
	map<string, vector<float>> floats;

	TdataFile(){}
	~TdataFile(){}
	TdataFile(const string &fileName, const string &subfolder="", const string &extension="");
	void print(uint depth = 0) const;

	static string findFile(const string &file, const string &subfolder="", const string &ext="");
};


#endif//HEADER_DATAFILE
