#include <GLmanager.hpp>
#include <object.hpp>
#include <scene.hpp>
#include <inputManager.hpp>
#include <mesh.hpp>
#include <shaderProgram.hpp>
#include <camera.hpp>

// #define STAR "kepler-90-binary"
// #define STAR "kepler-90"
// #define STAR "sol"

namespace GLmanager{
	bool isGlInitialized = false;
	bool isGlewInitialized = false;
	bool isGlutInitialized = false;
	Tscene *scene = new Tscene("scene");
	namespace {
		Tcamera camera;

		GLuint width = 500, height = 500;
		GLuint displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE;

		GLfloat cameraFOV = 45.0f;
		GLfloat zNear = 0.001f, zFar = 1000000.0f;

		void initCamera(){
			camera.setMode(Tcamera::Emode::free_mode);
			// camera.setMode(Tcamera::Emode::follow_mode);
			// camera.follow("center");
			// camera.follow(STAR);
			camera.update(0);

			inputManager::setMouseHidden(true);
			inputManager::setMouseLocked(true);
		}
		void initScene(){
			scene->load("trab2-cidade1");
			// scene->load("trab1-" STAR);
		}
		void init(){
			initScene();
			initCamera();

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LEQUAL);

			glEnable(GL_MULTISAMPLE);
		}
		void display(){
			//reset buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			scene->draw();

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
				auto res = getViewportResolution();
				//set the shader programs to use the new matrix
				for(auto &[_, prog] : TshaderProgram::manager.resources){
					prog->setUniform("clipMatrix", clip);
					prog->setUniform("iResolution", res);
				}
				glUseProgram(0);

				glViewport(0, 0, (GLsizei) w, (GLsizei) h);
			}
		}
		void cleanup(){
			delete scene;
			Tmesh::manager.clear();
			Tshader::manager.clear();
			TshaderProgram::manager.clear();
		}
		void update(){
			static GLfloat lastTime = 0.0f;
			GLfloat curTime = glutGet(GLUT_ELAPSED_TIME);
			GLfloat dTime = curTime - lastTime;


			if(inputManager::isKeyDown(key_escape)) glutLeaveMainLoop();

			scene->update(dTime);
			camera.update(dTime);
			inputManager::update(dTime);

			lastTime = curTime;
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

		glutInitDisplayMode(displayMode);
		glutInitContextVersion(4, 5);
		glutInitContextProfile(GLUT_CORE_PROFILE);
		glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
		glutInitWindowSize(width, height);
		glutInitWindowPosition(300, 200);
		glutCreateWindow(argv[0]);
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
		glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
		#ifndef DEBUG
		glutFullScreen();
		#endif
		isGlutInitialized = true;

		glewExperimental=GL_TRUE;
		GLenum err = glewInit();
		if(err != GLEW_OK){
			string s = (char*)glewGetErrorString(err);
			throw runtime_error("Error initializing glew: '" + s + "'\n");
		}
		isGlewInitialized = true;

		#ifdef DEBUG
		W(glGetString(GL_VERSION));
		if(glDebugMessageCallback){
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(debugCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
			glutInitContextFlags(GLUT_DEBUG);
		}
		#endif//DEBUG
		glClearDepth(zFar);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		isGlInitialized = true;

		init();

		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutIdleFunc(update);
		glutCloseFunc(cleanup);
		inputManager::init();//sets up keyboard and mouse callbacks
		glutMainLoop();
		isGlutInitialized = isGlewInitialized = isGlInitialized = false;
	}

	glm::mat4 calcClipMatrix(){
		return glm::perspective(glm::radians(cameraFOV), GLfloat(width)/GLfloat(height), zNear, zFar);
	}
	glm::mat4 calcViewMatrix(glm::mat4 view){
		view = camera.getViewMatrix();

		if(scene) view = view * scene->scene_scale;

		return view;
	}
	glm::vec3 getViewportResolution(){
		return glm::vec3(width, height, 1);
	}
}


