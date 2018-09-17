#include <GLmanager.hpp>
#include <shader.hpp>
#include <dataFile.hpp>
#include <mesh.hpp>

namespace GLmanager{
	struct object{
		// GLuint vao, vbo, ebo;
		Tmesh *mesh;
		glm::vec3 pos;
		glm::vec3 sca;
		glm::vec3 rot;
	};
	vector<object> objects;
	namespace {
		GLuint width = 500, height = 500;
		GLuint displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;

		GLuint shProgram;

		GLuint modelMatrixUnif, viewMatrixUnif, clipMatrixUnif;

		GLfloat cameraFOV = 45.0f;
		GLfloat zNear = 0.1f, zFar = 100.0f;

		glm::mat4 calcClipMatrix(){
			return glm::perspective(glm::radians(cameraFOV), GLfloat(width)/GLfloat(height), zNear, zFar);
		}

		glm::mat4 calcViewMatrix(glm::mat4 view = glm::mat4(1.0f)){
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
			view = glm::rotate(view, glm::radians(45.0f), glm::vec3(1, 0, 0));

			// //cameraPosition
			// view = glm::translate(view, -cameraPosition);
			// //cameraRotation

			return view;
		}

		glm::mat4 calcModelMatrix(const object &obj, glm::mat4 model = glm::mat4(1.0f)){
			//translation
			model = glm::translate(model, obj.pos);
			//rotation
			model = glm::rotate(model, obj.rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, obj.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, obj.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
			//scale
			model = glm::scale(model, obj.sca);
			return model;
		}

		void defaults(){}
		void initShaders(){
			shProgram = glShaderProgram::create({
				glShader::createF("vertexColor.fshader", GL_FRAGMENT_SHADER),
				// glShader::createF("model-world-camera-clip.vshader", GL_VERTEX_SHADER),
				glShader::createF("test.vshader", GL_VERTEX_SHADER),
			});
			modelMatrixUnif = glGetUniformLocation(shProgram, "modelMatrix");
			viewMatrixUnif = glGetUniformLocation(shProgram, "viewMatrix");
			clipMatrixUnif = glGetUniformLocation(shProgram, "clipMatrix");

			glUseProgram(shProgram);
			glUniformMatrix4fv(modelMatrixUnif, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
			glUniformMatrix4fv(viewMatrixUnif, 1, GL_FALSE, glm::value_ptr(calcViewMatrix()));
			glUniformMatrix4fv(clipMatrixUnif, 1, GL_FALSE, glm::value_ptr(calcClipMatrix()));
			glUseProgram(0);
		}
		void initObjects(){
			Tmesh *cube = new Tmesh("cube", "cube");
			objects.resize(2);

			objects[0].mesh = cube;
			objects[0].pos = glm::vec3{ 0.0f,  0.0f,  0.0f};
			objects[0].sca = glm::vec3{ 0.5f,  0.5f,  0.5f};
			objects[0].rot = glm::vec3{ 0.0f,  0.0f,  0.0f};

			objects[1].mesh = cube;
			objects[1].pos = glm::vec3{ 0.0f, -0.8f,  0.0f};
			objects[1].sca = glm::vec3{ 1.0f,  0.1f,  0.7f};
			objects[1].rot = glm::vec3{ 0.0f,  0.0f,  0.0f};
		}
		void init(){
			initShaders();
			initObjects();

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LEQUAL);
			// glDepthRange(0, 1);
		}
		void display(){
			//reset buffers
			glClearDepth(zFar);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//setup shader program
			glUseProgram(shProgram);


			// glUniformMatrix4fv(viewMatrixUnif, 1, GL_FALSE, glm::value_ptr(calcViewMatrix()));
			// glUniformMatrix4fv(modelMatrixUnif, 1, GL_FALSE, glm::value_ptr(calcModelMatrix(objects[0])));
			// objects[0].mesh->draw();
			for(auto &obj : objects){
				glUniformMatrix4fv(modelMatrixUnif, 1, GL_FALSE, glm::value_ptr(calcModelMatrix(obj)));

				obj.mesh->draw();
				// glBindVertexArray(obj.vao);
				// glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
				// glBindVertexArray(0);
			}

			// //render each object
			// for(auto &obj : objs){
			// 	//TODO: check if object should be drawn
			// 	calcModelMatrix(obj);
			// 	glUniformMatrix4fv(modelMatrixUnif, 1, GL_FALSE,
			// 		glm::value_ptr(model));
			// 	obj.mesh->draw();
			// }

			// //reset shader uniforms
			// glUniformMatrix4fv(modelMatrixUnif, 1, GL_FALSE,
			// 	glm::value_ptr((model = glm::mat4(1.0f))));

			glUseProgram(0);

			//flush changes
			glutSwapBuffers();
			glutPostRedisplay();
		}
		void reshape(int w, int h){
			if((GLuint)w != width or (GLuint)h != height){
				//save width and height values
				width = w;
				height = h;
				// recalculate perspective matrix with new values
				auto clip = calcClipMatrix();
				//set the shader program to use the new matrix
				glUseProgram(shProgram);
				glUniformMatrix4fv(
					clipMatrixUnif, 1, GL_FALSE, glm::value_ptr(clip));
				glUseProgram(0);

				glViewport(0, 0, (GLsizei) w, (GLsizei) h);
			}
		}
		void keyboard(unsigned char key, int x, int y){
			switch (key){
				case '\x1b'://escape
					glutLeaveMainLoop();
				return;
			}
		}
		void cleanup(){
			glShaderProgram::destroy(shProgram);
			delete objects[0].mesh;
			objects.clear();
		}
		void update(){
			GLfloat time = glutGet(GLUT_ELAPSED_TIME);


			objects[0].pos.x = sin(time / 500.0f);
			objects[0].rot.z = sin(glm::radians(360.0f) - (time / 500.0f));


			glm::vec4 p1( objects[0].sca.x, -objects[0].sca.y, 0.0f, 1.0f);
			glm::vec4 p2(-objects[0].sca.x, -objects[0].sca.y, 0.0f, 1.0f);
			glm::mat4 rot(1.0f);
			glm::rotate(rot, objects[0].rot.x, glm::vec3(1, 0, 0));
			glm::rotate(rot, objects[0].rot.y, glm::vec3(0, 1, 0));
			rot = glm::rotate(rot, objects[0].rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
			p1 = rot * p1;
			p2 = rot * p2;

			objects[0].pos.y = objects[1].pos.y + objects[1].sca.y - min(p1.y, p2.y);
		}
		#ifdef DEBUG
		void GLAPIENTRY debugCallback(GLenum source,
											GLenum type,
											GLuint id,
											GLenum severity,
											GLsizei length,
											const GLchar* message,
											const void* userParam){
			cerr << "---------------------opengl-callback-start------------" << endl;
			cerr << "message: "<< message << endl;
			cerr << "type: ";
			switch (type) {
				case GL_DEBUG_TYPE_ERROR:
					cerr << "ERROR";
					break;
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
					cerr << "DEPRECATED_BEHAVIOR";
					break;
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
					cerr << "UNDEFINED_BEHAVIOR";
					break;
				case GL_DEBUG_TYPE_PORTABILITY:
					cerr << "PORTABILITY";
					break;
				case GL_DEBUG_TYPE_PERFORMANCE:
					cerr << "PERFORMANCE";
					break;
				case GL_DEBUG_TYPE_OTHER:
					cerr << "OTHER";
					break;
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					cerr << "NOTIFICATION";
					break;
				default:
					cerr << "0x" << hex << type;
			}
			cerr << endl;
			cerr << "source: ";
			switch (source){
				case GL_DEBUG_SOURCE_API:
					cerr << "API";
					break;
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
					cerr << "WINDOW-SYSTEM";
					break;
				case GL_DEBUG_SOURCE_SHADER_COMPILER:
					cerr << "SHADER-COMPILER";
					break;
				case GL_DEBUG_SOURCE_THIRD_PARTY:
					cerr << "THIRD-PARTY";
					break;
				case GL_DEBUG_SOURCE_APPLICATION:
					cerr << "USER";
					break;
				case GL_DEBUG_SOURCE_OTHER:
					cerr << "OTHER";
					break;
				default:
					cerr << "0x" << hex << source;
			}
			cerr << endl;

			cerr << "id: " << id << endl;
			cerr << "severity: ";
			switch (severity){
				case GL_DEBUG_SEVERITY_LOW:
					cerr << "LOW";
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					cerr << "MEDIUM";
					break;
				case GL_DEBUG_SEVERITY_HIGH:
					cerr << "HIGH";
					break;
				default:
					cerr << "0x" << hex << severity;
			}
			cerr << endl;
			cerr << "---------------------opengl-callback-end--------------" << endl;
		}
		#endif//DEBUG
	}
	void run(int argc, char **argv){
		glutInit(&argc, argv);

		defaults();

		glutInitDisplayMode(displayMode);
		glutInitContextVersion(4, 5);
		glutInitContextProfile(GLUT_CORE_PROFILE);
		glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
		glutInitWindowSize(width, height);
		glutInitWindowPosition(300, 200);
		glutCreateWindow(argv[0]);
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

		glewExperimental=GL_TRUE;
		GLenum err = glewInit();
		if(err != GLEW_OK){
			cerr << "Error initializing glew: " << glewGetErrorString(err) << endl;
			exit(1);
		}


		#ifdef DEBUG
		W(glGetString(GL_VERSION));
		if(glDebugMessageCallback){
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(debugCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
			glutInitContextFlags(GLUT_DEBUG);
		}
		else WARN("glDebugMessageCallback not available");
		#endif//DEBUG

		init();

		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keyboard);
		glutIdleFunc(update);
		glutMainLoop();

		cleanup();
	}
	glm::vec1 makeVec1(const vector<GLfloat> &v){ return glm::vec1{ v[0] }; }
	glm::vec2 makeVec2(const vector<GLfloat> &v){ return glm::vec2{ v[0], v[1] }; }
	glm::vec3 makeVec3(const vector<GLfloat> &v){ return glm::vec3{ v[0], v[1], v[2] }; }
	glm::vec4 makeVec4(const vector<GLfloat> &v){ return glm::vec4{ v[0], v[1], v[2], v[3] }; }
	glm::mat2 makeMat2(const vector<vector<GLfloat>> &m){
		return glm::mat2{ makeVec2(m[0]), makeVec2(m[1]) };
	}
	glm::mat3 makeMat3(const vector<vector<GLfloat>> &m){
		return glm::mat3{ makeVec3(m[0]), makeVec3(m[1]), makeVec3(m[2]) };
	}
	glm::mat4 makeMat4(const vector<vector<GLfloat>> &m){
		return glm::mat4{ makeVec4(m[0]), makeVec4(m[1]), makeVec4(m[2]), makeVec4(m[3]) };
	}

	template<typename T> GLuint createBuffer(const vector<T> &vals, GLenum type, GLenum usage){
		GLuint buffer;
		glGenBuffers(1, &buffer);

		glBindBuffer(type, buffer);
		glBufferData(type, vals.size() * sizeof(T), vals.data(), usage);
		glBindBuffer(type, 0);

		return buffer;
	}

	GLfloat degToRad(const GLfloat &ang){ return ang * degToRadProp; }
	GLfloat radToDeg(const GLfloat &ang){ return ang / degToRadProp; }

}


