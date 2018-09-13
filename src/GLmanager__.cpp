#include <GLmanager.hpp>
#include <shader.hpp>
#include <dataFile.hpp>
#include <mesh.hpp>

namespace GLmanager{
	struct object{
		unique_ptr<Tmesh> mesh;
		glm::vec3 pos, scale, rot;
	};
	namespace _{
		GLuint width = 500;
		GLuint height = 500;
		GLuint displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;
		vector<object> objs;

		GLuint shProgram;

		GLuint modelToWorldUnif, worldToCameraUnif, cameraToClipUnif;
		glm::mat4 modelToWorld, worldToCamera, cameraToClip;

		glm::vec3 cameraPosition, cameraRotation;

		GLfloat cameraFOV = 45.0f;
		GLfloat zNear = 0.1f, zFar = 10.0f;
	}
	namespace {
		// void calcCameraToClipMatrix(){
		// 	GLfloat frustumScale = 1.0f / tan(degToRad(_::cameraFOV) / 2.0f);

		// 	glm::mat4 matrix(1.0f);
		// 	matrix[0].x = matrix[1].y = frustumScale;
		// 	matrix[2].z = (_::zFar  + _::zNear)
		// 				/ (_::zNear - _::zFar);
		// 	matrix[2].w = -1.0f;
		// 	matrix[3].z = (_::zFar  * _::zNear * 2.0f)
		// 				/ (_::zNear - _::zFar);

		// 	_::cameraToClip = matrix;
		// }
		void calcCameraToClipMatrix(){
			_::cameraToClip = glm::perspective(glm::radians(_::cameraFOV),
				GLfloat(_::width)/GLfloat(_::height), _::zNear, _::zFar);
		}

		void calcWorldToCameraMatrix(){
			glm::mat4 matrix(1.0f);

			//cameraPosition
			matrix = glm::translate(matrix, -_::cameraPosition);
			//cameraRotation

			_::worldToCamera = matrix;
		}

		void calcModelToWorldMatrix(const object &obj, glm::mat4 matrix = glm::mat4(1.0f)){
			//scale
			matrix = glm::scale(matrix, obj.scale);
			//rotation
			matrix = glm::rotate(matrix, obj.rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
			matrix = glm::rotate(matrix, obj.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
			matrix = glm::rotate(matrix, obj.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
			//translation
			matrix = glm::translate(matrix, obj.pos);
			_::modelToWorld = matrix;
		}

		void defaults(){}
		void initShaders(){
			_::shProgram = glShaderProgram::create({
				glShader::createF("vertexColor.fshader", GL_FRAGMENT_SHADER),
				// glShader::createF("model-world-camera-clip.vshader", GL_VERTEX_SHADER),
				glShader::createF("test.vshader", GL_VERTEX_SHADER),
			});
			_::modelToWorldUnif = glGetUniformLocation(_::shProgram, "modelToWorld");
			_::worldToCameraUnif = glGetUniformLocation(_::shProgram, "worldToCamera");
			_::cameraToClipUnif = glGetUniformLocation(_::shProgram, "cameraToClip");

			calcCameraToClipMatrix();
			calcWorldToCameraMatrix();

			glUseProgram(_::shProgram);
			glUniformMatrix4fv(_::modelToWorldUnif, 1, GL_FALSE, glm::value_ptr(_::modelToWorld));
			glUniformMatrix4fv(_::worldToCameraUnif, 1, GL_FALSE, glm::value_ptr(_::worldToCamera));
			glUniformMatrix4fv(_::cameraToClipUnif, 1, GL_FALSE, glm::value_ptr(_::cameraToClip));
			glUseProgram(0);
		}
		void initObjects(){
			_::objs.resize(1);
			_::objs[0].mesh = unique_ptr<Tmesh>{new Tmesh{"cube1", "cube"}};
			_::objs[0].pos = glm::vec3(0.0f, 0.0f, -1.0f);

			// _::objs.resize(2);

			// _::objs[0].mesh = unique_ptr<Tmesh>{new Tmesh{"cube1", "cube"}};
			// _::objs[0].pos = glm::vec3(0.0f, 0.0f, -1.0f);

			// _::objs[1].mesh = unique_ptr<Tmesh>{new Tmesh{"cube2", "cube"}};
			// _::objs[0].pos = glm::vec3(1.0f, 0.0f, -2.0f);
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
			glDepthRange(_::zNear, _::zFar);
		}
		void display(){
			//reset buffers
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClearDepth(_::zFar);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//setup shader program
			glUseProgram(_::shProgram);

			//TODO: check if camera pos/rot changed
			calcWorldToCameraMatrix();
			glUniformMatrix4fv(_::worldToCameraUnif, 1, GL_FALSE, glm::value_ptr(_::worldToCamera));

			//render each object
			for(auto &obj : _::objs){
				//TODO: check if object should be drawn
				calcModelToWorldMatrix(obj);
				glUniformMatrix4fv(_::modelToWorldUnif, 1, GL_FALSE,
					glm::value_ptr(_::modelToWorld));
				obj.mesh->draw();
			}

			//reset shader uniforms
			glUniformMatrix4fv(_::modelToWorldUnif, 1, GL_FALSE,
				glm::value_ptr((_::modelToWorld = glm::mat4(1.0f))));

			glUseProgram(0);

			//flush changes
			glutSwapBuffers();
			glutPostRedisplay();
		}
		void reshape(int w, int h){
			if((GLuint)w != _::width or (GLuint)h != _::height){
				//save width and height values
				_::width = w;
				_::height = h;
				//recalculate perspective matrix with new values
				calcCameraToClipMatrix();
				//set the shader program to use the new matrix
				glUseProgram(_::shProgram);
				glUniformMatrix4fv(
					_::cameraToClipUnif, 1, GL_FALSE, glm::value_ptr(_::cameraToClip));
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
			glShaderProgram::destroy(_::shProgram);
			_::objs.clear();
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

		glutInitDisplayMode(_::displayMode);
		glutInitContextVersion(4, 5);
		glutInitContextProfile(GLUT_CORE_PROFILE);
		glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
		glutInitWindowSize(_::width, _::height);
		glutInitWindowPosition(300, 200);
		glutCreateWindow(argv[0]);
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

		glewExperimental=GL_TRUE;
		GLenum err = glewInit();
		if(err != GLEW_OK){
			cerr << "Error initializing glew: " << glewGetErrorString(err) << endl;
			exit(1);
		}

		W(glGetString(GL_VERSION));

		#ifdef DEBUG
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


