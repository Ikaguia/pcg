#include <GLmanager.hpp>
#include <shader.hpp>
#include <dataFile.hpp>
#include <mesh.hpp>

namespace GLmanager{
	GLuint vao, vbo, ebo;
	namespace {
		GLuint width = 500, height = 500;
		GLuint displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;

		GLuint shProgram;

		GLuint modelMatrixUnif, viewMatrixUnif, clipMatrixUnif;
		glm::mat4 model, view, clip;

		GLfloat cameraFOV = 45.0f;
		GLfloat zNear = 0.1f, zFar = 100.0f;

		glm::mat4 calcClipMatrix(){
			return glm::perspective(glm::radians(cameraFOV), GLfloat(width)/GLfloat(height), zNear, zFar);
		}

		glm::mat4 calcviewMatrix(){
			return glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -3.0f));
			//////////////////////////////////////////////////////////////
			// glm::mat4 matrix(1.0f);

			// //cameraPosition
			// matrix = glm::translate(matrix, -cameraPosition);
			// //cameraRotation

			// return matrix;
		}

		// glm::mat4 calcmodelMatrix(const object &obj, glm::mat4 matrix = glm::mat4(1.0f)){
		// 	//translation
		// 	matrix = glm::translate(matrix, obj.pos);
		// 	//scale
		// 	matrix = glm::scale(matrix, obj.scale);
		// 	//rotation
		// 	matrix = glm::rotate(matrix, obj.rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
		// 	matrix = glm::rotate(matrix, obj.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		// 	matrix = glm::rotate(matrix, obj.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
		// 	return matrix;
		// }

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

			// glUseProgram(shProgram);
			// glUniformMatrix4fv(modelMatrixUnif, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
			// glUniformMatrix4fv(viewMatrixUnif, 1, GL_FALSE, glm::value_ptr(calcviewMatrix()));
			// glUniformMatrix4fv(clipMatrixUnif, 1, GL_FALSE, glm::value_ptr(calcClipMatrix()));
			// glUseProgram(0);
		}
		void initObjects(){
			const GLfloat scale = 0.5f;
			vector<GLfloat> vertices = {
				-scale, -scale,  scale,  1,		 1,  0,  0,  1,
				 scale, -scale,  scale,  1,		 1,  0,  0,  1,
				 scale,  scale,  scale,  1,		 1,  0,  0,  1,
				-scale,  scale,  scale,  1,		 1,  0,  0,  1,
				-scale,  scale, -scale,  1,		 0,  1,  0,  1,
				 scale,  scale, -scale,  1,		 0,  1,  0,  1,
				 scale, -scale, -scale,  1,		 0,  1,  0,  1,
				-scale, -scale, -scale,  1,		 0,  1,  0,  1,
				-scale, -scale,  scale,  1,		 0,  0,  1,  1,
				-scale, -scale, -scale,  1,		 0,  0,  1,  1,
				 scale, -scale, -scale,  1,		 0,  0,  1,  1,
				 scale, -scale,  scale,  1,		 0,  0,  1,  1,
				-scale,  scale,  scale,  1,		 1,  1,  0,  1,
				 scale,  scale,  scale,  1,		 1,  1,  0,  1,
				 scale,  scale, -scale,  1,		 1,  1,  0,  1,
				-scale,  scale, -scale,  1,		 1,  1,  0,  1,
				 scale,  scale,  scale,  1,		 0,  1,  1,  1,
				 scale, -scale,  scale,  1,		 0,  1,  1,  1,
				 scale, -scale, -scale,  1,		 0,  1,  1,  1,
				 scale,  scale, -scale,  1,		 0,  1,  1,  1,
				-scale,  scale,  scale,  1,		 1,  0,  1,  1,
				-scale,  scale, -scale,  1,		 1,  0,  1,  1,
				-scale, -scale, -scale,  1,		 1,  0,  1,  1,
				-scale, -scale,  scale,  1,		 1,  0,  1,  1,
			};
			vector<GLuint> indices = {
				0, 1, 2,
				0, 2, 3,

				4, 5, 6,
				4, 6, 7,

				8, 9, 10,
				8, 10, 11,

				12, 13, 14,
				12, 14, 15,

				16, 17, 18,
				16, 18, 19,

				20, 21, 22,
				20, 22, 23,
			};
			// vector<GLfloat> vertices = {
			// 	-scale, -scale,  0,  1,		 1,  0,  0,  1,
			// 	-scale,  scale,  0,  1,		 0,  0,  1,  1,
			// 	 scale,  scale,  0,  1,		 1,  1,  1,  1,
			// 	 scale, -scale,  0,  1,		 0,  1,  0,  1,
			// };
			// vector<GLuint> indices = {
			// 	0, 1, 2,
			// 	0, 2, 3,
			// };

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(0*sizeof(GLfloat)));
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(4*sizeof(GLfloat)));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

			// glBindVertexArray(0);
			// glBindBuffer(GL_ARRAY_BUFFER, 0);
			// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


			// objs.resize(1);
			// objs[0].mesh = unique_ptr<Tmesh>{new Tmesh{"cube1", "cube"}};
			// objs[0].pos = glm::vec3(0.0f, 0.0f, -1.0f);

			// objs.resize(2);

			// objs[0].mesh = unique_ptr<Tmesh>{new Tmesh{"cube1", "cube"}};
			// objs[0].pos = glm::vec3(0.0f, 0.0f, -1.0f);

			// objs[1].mesh = unique_ptr<Tmesh>{new Tmesh{"cube2", "cube"}};
			// objs[0].pos = glm::vec3(1.0f, 0.0f, -2.0f);
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
			// glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			// glClear(GL_COLOR_BUFFER_BIT);
			glClearDepth(zFar);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//setup shader program
			glUseProgram(shProgram);

			glm::mat4 model, view, perspective;

			glm::vec3 rot;
			rot.x = glutGet(GLUT_ELAPSED_TIME) / 500.0f;
			rot.x = fmod(rot.x, 360.0f);
			rot.y = glutGet(GLUT_ELAPSED_TIME) / 5000.0f;
			rot.y = fmod(rot.y, 360.0f);
			rot.z = glutGet(GLUT_ELAPSED_TIME) / 2500.0f;
			rot.z = fmod(rot.z, 360.0f);

			model = glm::rotate(model, rot.x, glm::vec3(1, 0, 0));
			model = glm::rotate(model, rot.y, glm::vec3(0, 1, 0));
			model = glm::rotate(model, rot.z, glm::vec3(0, 0, 1));
			glUniformMatrix4fv(modelMatrixUnif, 1, GL_FALSE, glm::value_ptr(model));

			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
			glUniformMatrix4fv(viewMatrixUnif, 1, GL_FALSE, glm::value_ptr(view));

			perspective = glm::perspective(glm::radians(cameraFOV), GLfloat(width)/GLfloat(height), zNear, zFar);
			glUniformMatrix4fv(clipMatrixUnif, 1, GL_FALSE, glm::value_ptr(perspective));

			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);


			//TODO: check if camera pos/rot changed
			// glUniformMatrix4fv(viewMatrixUnif, 1, GL_FALSE, glm::value_ptr(calcviewMatrix()));

			// //render each object
			// for(auto &obj : objs){
			// 	//TODO: check if object should be drawn
			// 	calcmodelMatrix(obj);
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
			// objs.clear();
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


