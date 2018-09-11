#include <mesh.hpp>
#include <GLmanager.hpp>

//constructors
Tmesh::Tmesh(const string &thename, const TdataFile &meshData):name{thename}{ construct(meshData); }
Tmesh::Tmesh(const string &thename, const string &fileName):name{thename}{
		const auto& data = TdataFile(fileName, "mesh", ".mesh");
		construct(data);
}
Tmesh::~Tmesh(){ destruct(); }

bool Tmesh::vertex::operator<(const vertex &other)const{
	if(xyzw[0] != other.xyzw[0]) return xyzw[0] < other.xyzw[0];
	if(xyzw[1] != other.xyzw[1]) return xyzw[1] < other.xyzw[1];
	if(xyzw[2] != other.xyzw[2]) return xyzw[2] < other.xyzw[2];
	if(xyzw[3] != other.xyzw[3]) return xyzw[3] < other.xyzw[3];
	if(rgba[0] != other.rgba[0]) return rgba[0] < other.rgba[0];
	if(rgba[1] != other.rgba[1]) return rgba[1] < other.rgba[1];
	if(rgba[2] != other.rgba[2]) return rgba[2] < other.rgba[2];
	if(rgba[3] != other.rgba[3]) return rgba[3] < other.rgba[3];
	return false;
}

//private functions
void Tmesh::construct(const TdataFile &meshData){

	vector<array<GLfloat, 4>> points;
	if(meshData.sons.count("vertices")){
		for(const auto &vertData : meshData.sons.at("vertices")){
			array<GLfloat, 4> pt;
			if(vertData.floats.count("xyz")){
				pt[0] = vertData.floats.at("xyz")[0];
				pt[1] = vertData.floats.at("xyz")[1];
				pt[2] = vertData.floats.at("xyz")[2];
				pt[3] = 1.0f;
			}
			else if(vertData.floats.count("xyzw")){
				pt[0] = vertData.floats.at("xyzw")[0];
				pt[1] = vertData.floats.at("xyzw")[1];
				pt[2] = vertData.floats.at("xyzw")[2];
				pt[3] = vertData.floats.at("xyzw")[3];
			}
			else continue;
			points.pb(pt);
		}
	}
	else cerr << "no vertices" << endl;

	vector<array<GLfloat, 4>> colors;
	if(meshData.sons.count("colors")){
		for(const auto &colorData : meshData.sons.at("colors")){
			if(not colorData.floats.count("rgba"))continue;
			array<GLfloat, 4> color;
			FOR(i, 0, 4) color[i] = colorData.floats.at("rgba")[i];
			colors.pb(color);
		}
	}
	else cerr << "no colors" << endl;

	map<vertex, uint> verticesInd;//map vertex to their new indices
	vector<vertex> vertices;//the vertex data that will be stored in the buffer
	vector<uint> vertexIndices;//the vertex indices that will be stored in the buffer
	if(meshData.sons.count("faces")){
		for(const auto &faceData : meshData.sons.at("faces")){
			if(not faceData.ints.count("vertices"))continue;

			const auto &facePoints = faceData.ints.at("vertices");
			vector<uint> face;

			if(faceData.ints.count("colors")){
				const auto &faceColors = faceData.ints.at("colors");
				if(facePoints.size() != faceColors.size()){
					cerr << "Error: mesh '" << name << "' ";
					cerr << "has a face with different number of vertices and colors." << endl;
					continue;
				}
				FOR(i, 0, facePoints.size()){
					vertex vert{points[facePoints[i]], colors[faceColors[i]]};
					if(not verticesInd.count(vert)){
						verticesInd[vert] = vertices.size();
						vertices.pb(vert);
					}
					face.pb(verticesInd[vert]);
				}
			}
			else if(faceData.ints.count("color")){
				const auto &faceColor = colors[faceData.ints.at("color")[0]];
				FOR(i, 0, facePoints.size()){
					vertex vert{points[facePoints[i]], faceColor};
					if(not verticesInd.count(vert)){
						verticesInd[vert] = vertices.size();
						vertices.pb(vert);
					}
					face.pb(verticesInd[vert]);
				}
			}
			else continue;


			vector<array<uint, 3>> triangles = triangulate(face, vertices);
			for(const auto &triangle : triangles){
				for(const auto &ptInd : triangle){
					vertexIndices.pb(ptInd);
				}
			}
		}
	}
	else cerr << "no faces" << endl;

	vertexCount = vertexIndices.size();

	//create and bind vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//create and bind vertex buffer object
	// vbo = GLmanager::createBuffer(vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	{
		GLuint buffer;
		glGenBuffers(1, &buffer);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		vbo = buffer;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	{//set vertex data on vao
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		size_t vertsz = sizeof(vertex);
		size_t offset0 = offsetof(vertex, xyzw);
		size_t offset1 = offsetof(vertex, rgba);

		//array pos
		//n of vals per unit
		//vals type
		//(dont)normalize
		//offset between units
		//offset before first member
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertsz, (void*)offset0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertsz, (void*)offset1);
	}

	//create and bind index buffer object
	// ibo = GLmanager::createBuffer(vertexIndices, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	{
		GLuint buffer;
		glGenBuffers(1, &buffer);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(GLuint), vertexIndices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		ibo = buffer;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	//unbind vao
	glBindVertexArray(0);
}
void Tmesh::destruct(){
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

//public functions
void Tmesh::draw(glm::vec4 translation, glm::vec4 rotation, glm::vec4 scale){
	cerr << "Drawing mesh '" << name << "'" << endl;

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

vector<array<uint, 3>> Tmesh::triangulate(const vector<uint> &face, const vector<vertex> &vertices){
	//TODO: make this work for non convex polygons
	vector<array<uint, 3>> triangles;
	FOR(i, 1, face.size()-1) triangles.pb(array<uint, 3>{face[0], face[i], face[i+1]});
	return triangles;
}
