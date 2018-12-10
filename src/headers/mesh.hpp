#ifndef HEADER_MESH
#define HEADER_MESH

#include <common.hpp>
#include <dataFile.hpp>
#include <resourceManager.hpp>

class Tmesh{
	struct vertex{
		glm::vec4 xyzw = glm::vec4{0,0,0,1};
		glm::vec2 tex  = glm::vec2{0,0};
		glm::vec3 norm = glm::vec3{0,0,0};
		bool operator<(const vertex &other) const;
	};
	//private members
	const GLuint vao, vbo, ebo;//vertex array, vertex buffer, element buffer objects
	GLuint vertexCount;
	//private functions
	void construct(vector<vertex> vToInd, vector<uint> inds);
	void destruct();
public:
	//public members
	const string name;
	//constructors
	Tmesh(const string &fileName);
	~Tmesh();
	//public functions
	void draw();

	static vector<array<uint, 3>> triangulate(const vector<uint>&, const vector<vertex>&);
	static TresourceManager<Tmesh> manager;
};


#endif//HEADER_MESH
