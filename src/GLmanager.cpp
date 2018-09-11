#include <GLmanager.hpp>
#include <shader.hpp>
#include <dataFile.hpp>
#include <mesh.hpp>

namespace GLmanager{
	struct object{
		Tmesh mesh;
		vec3 pos, scale, rot;
	};
	namespace _{
		uint width = 500;
		uint height = 500;
		uint displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;
		vector<object> objs;

		GLuint shProgram;

		GLuint modelToWorldUnif, worldToCameraUnif, cameraToClipUnif;
		glm::mat4 modelToWorld, worldToCamera, cameraToClip;

		glm::vec4 cameraPosition, cameraRotation;

		GLfloat cameraFOV = 45.0f, cameraFrustumScale, cameraZNear = 0.5f, cameraZFar = 3.0f;
		GLfloat rangeZNear = 0.0f, rangeZFar = 1.0f;
	}
	namespace {
		void calcFrustumScale(){
			_::cameraFrustumScale = 1.0f / tan(degToRad(_::cameraFOV) / 2.0f);
		}
		void calcCameraToClipMatrix(){
			calcFrustumScale();	

			mat4 matrix(1.0f);
			matrix[0].x = matrix[1].y = _::cameraFrustumScale;
			matrix[2].z = (_::cameraZFar  + _::cameraZNear)
						/ (_::cameraZNear - _::cameraZFar);
			matrix[2].w = -1.0f;
			matrix[3].z = (_::cameraZFar  * _::cameraZNear * 2.0f)
						/ (_::cameraZNear - _::cameraZFar);

			_::cameraToClip = matrix;
		}

		void calcWorldToCameraMatrix(){
			mat4 matrix(1.0f);

			//cameraPosition
			//cameraRotation

			_::worldToCamera = matrix;
		}

		void calcModelToWorldMatrix(const object &obj, const glm::vec4 &matrix = glm::mat4(1.0f)){
			//scale
			matrix = glm::scale(matrix, obj.scale);
			//rotation
			matrix = glm::rotate(matrix, obj.rot.x, glm::vec3(1, 0, 0));
			matrix = glm::rotate(matrix, obj.rot.y, glm::vec3(0, 1, 0));
			matrix = glm::rotate(matrix, obj.rot.z, glm::vec3(0, 0, 1));
			//translation
			matrix = glm::translate(matrix, obj.pos);
			_::modelToWorld = matrix;
		}

		void defaults(){
			calcFrustumScale();
		}
		void initShaders(){
			_::shProgram = glShaderProgram::create({
				glShader::createF("base2.fshader", GL_FRAGMENT_SHADER),
				glShader::createF("matrixPersp.vshader", GL_VERTEX_SHADER),
			});
			_::modelToCameraUnif = glGetUniformLocation(_::shProgram, "offset");
			_::cameraToClipUnif = glGetUniformLocation(_::shProgram, "perspectiveMatrix");


			glUseProgram(_::shProgram);
			glUniformMatrix4fv(_::cameraToClipUnif, 1, GL_FALSE,
				glm::value_ptr(_::cameraPerspMatrix));
			glUseProgram(0);
		}
		void initObjects(){
			_::objs.pb(Tmesh{"cube1", "cube"});
			_::objs.pb(Tmesh{"cube2", "cube"});
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
			glDepthRange(_::rangeZNear, _::rangeZFar);
		}
		void display(){
			calcOffset();

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClearDepth(_::rangeZFar);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(_::shProgram);

			for(auto &obj : _::objs){
				calcModelToWorldMatrix(obj);
				glUniformMatrix4fv(_::modelToCameraUnif, 1, GL_FALSE,
					glm::value_ptr(_::modelToWorld));
				obj.mesh.draw();
			}

			glUniformMatrix4fv(_::modelToCameraUnif, 1, GL_FALSE,
				glm::value_ptr((_::modelToWorld = glm::mat4(1.0f))));

			glUseProgram(0);

			glutSwapBuffers();
			glutPostRedisplay();
		}
		void reshape(int w, int h){
			// if(w < h) glViewport(0, (h-w) / 2, GLsizei(w), GLsizei(w));
			// else      glViewport((w-h) / 2, 0, GLsizei(h), GLsizei(h));
			_::cameraPerspMatrix[0] = _::cameraFrustumScale / (w / (GLfloat)h);
			_::cameraPerspMatrix[5] = _::cameraFrustumScale;

			glUseProgram(_::shProgram);
			glUniformMatrix4fv(_::cameraToClipUnif, 1, GL_FALSE, _::cameraPerspMatrix);
			glUseProgram(0);

			glViewport(0, 0, (GLsizei) w, (GLsizei) h);
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
		glutInitContextVersion(3, 3);
		glutInitContextProfile(GLUT_CORE_PROFILE);
		glutInitWindowSize(_::width, _::height); 
		glutInitWindowPosition(300, 200);
		glutCreateWindow(argv[0]);
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
		glewInit();

		#ifdef DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(debugCallback, 0);
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


