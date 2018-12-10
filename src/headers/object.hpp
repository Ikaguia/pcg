#ifndef HEADER_OBJECT
#define HEADER_OBJECT

#include <common.hpp>
#include <dataFile.hpp>
#include <mesh.hpp>
#include <material.hpp>
#include <shaderProgram.hpp>

class Ttexture;

class Tobject{
	//private members
	string father;
	set<string> sons;

	glm::vec3 translation = glm::vec3{ 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation = glm::vec3{ 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
	glm::vec3 objectColor = glm::vec3{ 1.0f, 1.0f, 1.0f };

	glm::mat4 absoluteModelMatrix = glm::mat4(1.0f);
	glm::mat4 relativeModelMatrix = glm::mat4(1.0f);
	glm::mat4 fatherModelMatrix = glm::mat4(1.0f);

	shared_ptr<Tmesh> mesh;
	shared_ptr<TshaderProgram> shaderProgram;
	unique_ptr<Tmaterial> material;

	vector<function<void(Tobject&, GLfloat)>> update_functions;
	//private functions
	void construct(const TdataFile &objectData);
	void destruct();
public:
	//public members
	const string name;
	bool needs_recalc = true;

	//constructors
	Tobject(const string &thename, const TdataFile &objectData);
	Tobject(const TdataFile &objectData);
	Tobject(const string &thename = "no_name");
	~Tobject();

	//public functions
	void setFather(const string &f);
	void setSons(const set<string> &s);
	void insertSon(const string &s);

	void setTranslation(const glm::vec3 &t);
	void setRotation(const glm::vec3 &r);
	void setScale(const glm::vec3 &s);
	void setColor(const glm::vec3 &c);

	void draw() const;
	void update(const GLfloat &dTime);
	void addUpdateFunction(const function<void(Tobject&, GLfloat)> &foo);

	void calcAbsoluteModelMatrix();
	void calcRelativeModelMatrix();

	const string& getFather() const;
	const set<string>& getSons() const;

	const glm::vec3& getTranslation() const;
	const glm::vec3& getRotation() const;
	const glm::vec3& getScale() const;
	const glm::vec3& getColor() const;

	const glm::mat4& getAbsoluteModelMatrix() const;
	const glm::mat4& getRelativeModelMatrix() const;
	glm::vec3 getAbsolutePosition() const;
};

#endif//HEADER_OBJECT
