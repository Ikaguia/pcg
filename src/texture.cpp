#include <texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.hpp>


namespace {
	tuple<vector<unsigned char>, int, int> readData(const string &fileName){
		string texFile = TdataFile::findFile(fileName, "texture", "");

		// load and generate the texture
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);

		vector<unsigned char> data;
		unsigned char *pt = stbi_load(texFile.data(), &width, &height, &nrChannels, 0);
		FOR(i, 0, width * height * 3) data.pb(pt[i]);
		free(pt);

		return {data, width, height};
	}
	tuple<vector<unsigned char>, int, int> readData(const vector<float> &v){
		vector<unsigned char> data;
		for(float f : v) data.pb((unsigned char)(f * 256));
		return {data, 1, 1};
	}
}

//static variables
TresourceManager<Ttexture> Ttexture::manager;

//constructors
Ttexture::Ttexture(const string &file):id{construct(readData(file))}{}
Ttexture::Ttexture(const vector<GLfloat> &v):id{construct(readData(v))}{}
Ttexture::~Ttexture(){ destruct(); }

//private functions
GLuint Ttexture::construct(tuple<vector<unsigned char>, int, int> args){
	auto [data, width, height] = args;
	GLuint texture;

	if(data.size() == (width * height * 3)){
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else throw invalid_argument("Failed to load texture."s);

	return texture;
}
void Ttexture::destruct(){ glDeleteTextures(1, &id); }
