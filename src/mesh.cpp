#include <mesh.hpp>
#include <GLmanager.hpp>

//static variables
TresourceManager<Tmesh> Tmesh::manager;

//constructors
Tmesh::Tmesh(const string &fileName):
	vao{(glGenVertexArrays(1, &g_tmp), g_tmp)},
	vbo{(glGenBuffers(1, &g_tmp), g_tmp)},
	ebo{(glGenBuffers(1, &g_tmp), g_tmp)},
	name{fileName}{
		vector<vertex> vertices;
		vector<uint> indices;
		{//read file
			ifstream meshFile(TdataFile::findFile(fileName, "mesh", ".obj"));

			string the_line, in;
			vector<glm::vec4> vertCoords;
			vector<glm::vec2> vertTextures;
			vector<glm::vec3> vertNormals;
			int line_count = 0;

			map<vertex, int> vToInd;
			while(getline(meshFile, the_line)){
				line_count++;
				stringstream line(the_line);
				line >> in;
				if(in[0] == '#') continue;
				if(in == "v"){//read vertex coordinate
					glm::vec4 vert{0.0f, 0.0f, 0.0f, 1.0f};
					line >> vert[0] >> vert[1] >> vert[2] >> vert[3];
					vertCoords.pb(vert);
				}
				else if(in == "vt"){//read vertex texture coordinate
					glm::vec2 vert{0.0f, 0.0f};
					// line >> vert.u >> vert.v;
					line >> vert[0] >> vert[1];
					vertTextures.pb(vert);
				}
				else if(in == "vn"){//read vertex normal coordinates
					glm::vec3 vert{0.0f, 0.0f, 0.0f};
					line >> vert[0] >> vert[1] >> vert[2];
					vert = glm::normalize(vert);
					vertNormals.pb(vert);
				}
				else if(in == "f"){//read face
					string vertexStr;
					vector<uint> face;
					getline(line, vertexStr, ' ');
					while(getline(line, vertexStr, ' ')){//read all vertices of the face
						int v = -1, vt = -1, vn = -1;

						int cnt = 0, last = 0;
						FOR(i, 0, vertexStr.size()+1){//read all parameters of a single vertexStr
							if(uint(i) == vertexStr.size() or vertexStr[i] == '/'){
								if(cnt == 0){
									try{ v = stoi(vertexStr.substr(last, i)); }
									catch(invalid_argument &e){
										(void)e;
										W(last);
										W(i);
										W(vertexStr);
										W(vertexStr.substr(last, i));
										WARN(the_line);
										string error = "Invalid face in mesh file '" + name + ".obj' line " + to_string(line_count);
										throw invalid_argument(error);
									}
								}
								else if(cnt == 1){
									try{ vt = stoi(vertexStr.substr(last+1, i - last - 1)); }
									catch(invalid_argument &e){ (void)e; }
								}
								else if(cnt == 2){
									try{ vn = stoi(vertexStr.substr(last+1, i - last - 1)); }
									catch(invalid_argument &e){ (void)e; }
								}
								last = i;
								cnt++;
							}
						}
						if(cnt >= 4){
							string error = "Invalid face in mesh file '" + name + ".obj' line " + to_string(line_count);
							throw invalid_argument(error);
						}

						vertex vert;
						vert.xyzw = vertCoords[v-1];;
						vert.tex  = vertTextures[vt-1];
						vert.norm = vertNormals[vn-1];
						if(not vToInd.count(vert)){
							vToInd[vert] = vertices.size();
							vertices.pb(vert);
						}
						face.pb(vToInd[vert]);
					}
					auto triangFace = triangulate(face, vertices);
					for(auto &triang : triangFace){
						indices.pb(triang[0]);
						indices.pb(triang[1]);
						indices.pb(triang[2]);
					}
				}
				else if(in == "mtllib" or in == "o" or in == "usemtl" or in == "s") continue;
				#ifdef DEBUG
				else{
					cerr << ".obj interpreter does not recognize '" << in;
					cerr << "' at '" << name << ".obj' line " << line_count << endl;
				}
				#endif
			}
		}
		construct(vertices, indices);
}
Tmesh::~Tmesh(){ destruct(); }

bool Tmesh::vertex::operator<(const vertex &other) const{
	FOR(i, 0, 4) if(xyzw[i] != other.xyzw[i]) return xyzw[i] < other.xyzw[i];
	FOR(i, 0, 2) if(tex[i]  != other.tex[i])  return tex[i]  < other.tex[i];
	FOR(i, 0, 3) if(norm[i] != other.norm[i]) return norm[i] < other.norm[i];
	return false;
}

void Tmesh::construct(vector<vertex> vertices, vector<uint> inds){
	vertexCount = inds.size();

	//bind vertex array object
	glBindVertexArray(vao);

	//copy data to vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

	{//set vertex data on vao
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		size_t vertsz = sizeof(vertex);
		size_t offset0 = offsetof(vertex, xyzw);
		size_t offset1 = offsetof(vertex, tex);
		size_t offset2 = offsetof(vertex, norm);

		//array pos
		//n of vals per unit
		//vals type
		//(dont)normalize
		//offset between units
		//offset before first member
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertsz, (void*)offset0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertsz, (void*)offset1);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, vertsz, (void*)offset2);
	}

	//copy data to index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(GLuint), inds.data(), GL_STATIC_DRAW);

	//unbind vao
	glBindVertexArray(0);
}

void Tmesh::destruct(){
	if(not GLmanager::isGlInitialized){
		throw logic_error("Tmesh '" + name + "' being destroyed after gl is deinitialized\n");
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

//public functions
void Tmesh::draw(){
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
