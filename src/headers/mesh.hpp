#ifndef HEADER_MESH
#define HEADER_MESH

#include <common.hpp>

#include <dataFile.hpp>

class Tmesh{
	struct vertex{
		array<GLfloat,4> xyzw;
		array<GLfloat,4> rgba;
		bool operator<(const vertex &other)const;
	};
	//private members
	GLuint vbo, ibo, vao;//vertex buffer, index buffer, vertex array objects
	GLuint vertexCount;
	//private functions
	void destruct();
public:
	//public members
	const string name;
	//constructors
	Tmesh(const string &thename, const TdataFile &meshData);
	Tmesh(const string &thename, const string &fileName);
	~Tmesh();
	//public functions
	void construct(const TdataFile &meshData);
	void draw(glm::vec4 translation=glm::vec4(), glm::vec4 rotation=glm::vec4(), glm::vec4 scale=glm::vec4());

	static vector<array<uint, 3>> triangulate(const vector<uint>&, const vector<vertex>&);
};

#endif//HEADER_MESH
