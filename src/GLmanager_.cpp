#include <GLmanager.hpp>
#include <shader.hpp>

namespace GLmanager{
	uint width;
	uint height;
	uint displayMode;
	namespace _{
		struct vert{
			float x,y,z,w = 1.0f;
			float r,g,b,a = 1.0f;
		};
		const vector<vert> vertexData = {
			{
				0.25f,  0.25f, -1.25f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
				}, {
				0.25f, -0.25f, -1.25f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
				}, {
				-0.25f,  0.25f, -1.25f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
			},
			{
				0.25f, -0.25f, -1.25f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
				}, {
				-0.25f, -0.25f, -1.25f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
				}, {
				-0.25f,  0.25f, -1.25f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
			},
			{
				0.25f,  0.25f, -2.75f, 1.0f,
				0.8f, 0.8f, 0.8f, 1.0f,
				}, {
				-0.25f,  0.25f, -2.75f, 1.0f,
				0.8f, 0.8f, 0.8f, 1.0f,
				}, {
				0.25f, -0.25f, -2.75f, 1.0f,
				0.8f, 0.8f, 0.8f, 1.0f,
			},
			{
				0.25f, -0.25f, -2.75f, 1.0f,
				0.8f, 0.8f, 0.8f, 1.0f,
				}, {
				-0.25f,  0.25f, -2.75f, 1.0f,
				0.8f, 0.8f, 0.8f, 1.0f,
				}, {
				-0.25f, -0.25f, -2.75f, 1.0f,
				0.8f, 0.8f, 0.8f, 1.0f,
			},
			{
				-0.25f,  0.25f, -1.25f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				}, {
				-0.25f, -0.25f, -1.25f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				}, {
				-0.25f, -0.25f, -2.75f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
			},
			{
				-0.25f,  0.25f, -1.25f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				}, {
				-0.25f, -0.25f, -2.75f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				}, {
				-0.25f,  0.25f, -2.75f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
			},
			{
				0.25f,  0.25f, -1.25f, 1.0f,
				0.5f, 0.5f, 0.0f, 1.0f,
				}, {
				0.25f, -0.25f, -2.75f, 1.0f,
				0.5f, 0.5f, 0.0f, 1.0f,
				}, {
				0.25f, -0.25f, -1.25f, 1.0f,
				0.5f, 0.5f, 0.0f, 1.0f,
			},
			{
				0.25f,  0.25f, -1.25f, 1.0f,
				0.5f, 0.5f, 0.0f, 1.0f,
				}, {
				0.25f,  0.25f, -2.75f, 1.0f,
				0.5f, 0.5f, 0.0f, 1.0f,
				}, {
				0.25f, -0.25f, -2.75f, 1.0f,
				0.5f, 0.5f, 0.0f, 1.0f,
			},
			{
				0.25f,  0.25f, -2.75f, 1.0f,
				1.0f, 0.0f, 0.0f, 1.0f,
				}, {
				0.25f,  0.25f, -1.25f, 1.0f,
				1.0f, 0.0f, 0.0f, 1.0f,
				}, {
				-0.25f,  0.25f, -1.25f, 1.0f,
				1.0f, 0.0f, 0.0f, 1.0f,
			},
			{
				0.25f,  0.25f, -2.75f, 1.0f,
				1.0f, 0.0f, 0.0f, 1.0f,
				}, {
				-0.25f,  0.25f, -1.25f, 1.0f,
				1.0f, 0.0f, 0.0f, 1.0f,
				}, {
				-0.25f,  0.25f, -2.75f, 1.0f,
				1.0f, 0.0f, 0.0f, 1.0f,
			},
			{
				0.25f, -0.25f, -2.75f, 1.0f,
				0.0f, 1.0f, 1.0f, 1.0f,
				}, {
				-0.25f, -0.25f, -1.25f, 1.0f,
				0.0f, 1.0f, 1.0f, 1.0f,
				}, {
				0.25f, -0.25f, -1.25f, 1.0f,
				0.0f, 1.0f, 1.0f, 1.0f,
			},
			{
				0.25f, -0.25f, -2.75f, 1.0f,
				0.0f, 1.0f, 1.0f, 1.0f,
				}, {
				-0.25f, -0.25f, -2.75f, 1.0f,
				0.0f, 1.0f, 1.0f, 1.0f,
				}, {
				-0.25f, -0.25f, -1.25f, 1.0f,
				0.0f, 1.0f, 1.0f, 1.0f,
			},
		};
		float shPerspectiveMatrix[] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};
		GLuint shProgram, vertexBufferObj, vertexArrayObj;
		GLuint shOffsetLocation, shPerspectiveMatrixLocation;
		float xOffset, yOffset, shFrustumScale=1.0f, shZNear=0.5f, shZFar=3.0f;
	}
	namespace {
		void calcOffset(){
			const float fLoopDuration = 5.0f;
			const float fScale = 3.14159f * 2.0f / fLoopDuration;

			float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

			float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

			_::xOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.5f;
			_::yOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.5f;
		}
		void defaults(){
			width = 500;
			height = 500;
			displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;
		}
		void initShaders(){
			_::shProgram = glShaderProgram::create({
				glShader::createF("vertexColor.fshader", GL_FRAGMENT_SHADER),
				glShader::createF("matrixPersp.vshader", GL_VERTEX_SHADER),
			});
			_::shOffsetLocation = glGetUniformLocation(_::shProgram, "offset");
			_::shPerspectiveMatrixLocation = glGetUniformLocation(_::shProgram, "perspectiveMatrix");

			_::shPerspectiveMatrix[0]  = _::shPerspectiveMatrix[5] = _::shFrustumScale;
			_::shPerspectiveMatrix[10] = (_::shZFar + _::shZNear) / (_::shZNear - _::shZFar);
			_::shPerspectiveMatrix[14] = (2 * _::shZFar * _::shZNear) / (_::shZNear - _::shZFar);
			_::shPerspectiveMatrix[11] = -1.0f;

			glUseProgram(_::shProgram);
			glUniformMatrix4fv(_::shPerspectiveMatrixLocation, 1, GL_FALSE, _::shPerspectiveMatrix);
			glUseProgram(0);
		}
		void initObjects(){
			_::vertexBufferObj = GLmanager::createBuffer(_::vertexData, GL_ARRAY_BUFFER, GL_STATIC_DRAW);

			glGenVertexArrays(1, &_::vertexArrayObj);
			glBindVertexArray(_::vertexArrayObj);
		}
		void init(){
			initShaders();
			initObjects();

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CW);
		}
		void display(){
			calcOffset();

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(_::shProgram);
		    glUniform2f(_::shOffsetLocation, _::xOffset, _::yOffset);

			glBindBuffer(GL_ARRAY_BUFFER, _::vertexBufferObj);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			size_t vertsz = sizeof(_::vert);
			size_t offset0 = offsetof(_::vert, x);
			size_t offset1 = offsetof(_::vert, r);

			//array pos
			//n of vals per unit
			//vals type
			//(dont)normalize
			//offset between units
			//offset before first member
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertsz, (void*)offset0);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertsz, (void*)offset1);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glUseProgram(0);

			glutSwapBuffers();
			glutPostRedisplay();
		}
		void reshape(int w, int h){
			// if(w < h) glViewport(0, (h-w) / 2, GLsizei(w), GLsizei(w));
			// else      glViewport((w-h) / 2, 0, GLsizei(h), GLsizei(h));
			_::shPerspectiveMatrix[0] = _::shFrustumScale / (w / (float)h);
			_::shPerspectiveMatrix[5] = _::shFrustumScale;

			glUseProgram(_::shProgram);
			glUniformMatrix4fv(_::shPerspectiveMatrixLocation, 1, GL_FALSE, _::shPerspectiveMatrix);
			glUseProgram(0);

			glViewport(0, 0, (GLsizei) w, (GLsizei) h);
		}
		void keyboard(unsigned char key, int x, int y){
			switch (key){
				case 27:
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
			(void)length;
			(void)userParam;
		}
		#endif//DEBUG
	}
	void run(int argc, char **argv){
		glutInit(&argc, argv);

		defaults();

		glutInitDisplayMode(displayMode);
		glutInitContextVersion(3, 3);
		glutInitContextProfile(GLUT_CORE_PROFILE);
		glutInitWindowSize(width, height); 
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
	template<typename T> GLuint createBuffer(const vector<T> &vals, GLenum type, GLenum usage){
		GLuint buffer;
		glGenBuffers(1, &buffer);

		glBindBuffer(type, buffer);
		glBufferData(type, vals.size() * sizeof(T), vals.data(), usage);
		glBindBuffer(type, 0);

		return buffer;
	}
	glm::vec1 makeVec(const array<float, 1> &v){ return glm::vec1{ v[0] }; }
	glm::vec2 makeVec(const array<float, 2> &v){ return glm::vec2{ v[0], v[1] }; }
	glm::vec3 makeVec(const array<float, 3> &v){ return glm::vec3{ v[0], v[1], v[2] }; }
	glm::vec4 makeVec(const array<float, 4> &v){ return glm::vec4{ v[0], v[1], v[2], v[3] }; }
	glm::mat2 makeMat(const array<array<float, 2>, 2> &m){
		return glm::mat2{ makeVec(m[0]), makeVec(m[1]) };
	}
	glm::mat3 makeMat(const array<array<float, 3>, 3> &m){
		return glm::mat3{ makeVec(m[0]), makeVec(m[1]), makeVec(m[2]) };
	}
	glm::mat4 makeMat(const array<array<float, 4>, 4> &m){
		return glm::mat4{ makeVec(m[0]), makeVec(m[1]), makeVec(m[2]), makeVec(m[3]) };
	}
}


