#include <fstream>
#include <vector>
#include <cstring>

#include "../headers/imageloader.h"
#include "../headers/md2model.h"

using namespace std;

namespace {
	//Normals used in the MD2 file format
	float NORMALS[486] =
		{-0.525731f,  0.000000f,  0.850651f,
		 -0.442863f,  0.238856f,  0.864188f,
		 -0.295242f,  0.000000f,  0.955423f,
		 -0.309017f,  0.500000f,  0.809017f,
		 -0.162460f,  0.262866f,  0.951056f,
		  0.000000f,  0.000000f,  1.000000f,
		  0.000000f,  0.850651f,  0.525731f,
		 -0.147621f,  0.716567f,  0.681718f,
		  0.147621f,  0.716567f,  0.681718f,
		  0.000000f,  0.525731f,  0.850651f,
		  0.309017f,  0.500000f,  0.809017f,
		  0.525731f,  0.000000f,  0.850651f,
		  0.295242f,  0.000000f,  0.955423f,
		  0.442863f,  0.238856f,  0.864188f,
		  0.162460f,  0.262866f,  0.951056f,
		 -0.681718f,  0.147621f,  0.716567f,
		 -0.809017f,  0.309017f,  0.500000f,
		 -0.587785f,  0.425325f,  0.688191f,
		 -0.850651f,  0.525731f,  0.000000f,
		 -0.864188f,  0.442863f,  0.238856f,
		 -0.716567f,  0.681718f,  0.147621f,
		 -0.688191f,  0.587785f,  0.425325f,
		 -0.500000f,  0.809017f,  0.309017f,
		 -0.238856f,  0.864188f,  0.442863f,
		 -0.425325f,  0.688191f,  0.587785f,
		 -0.716567f,  0.681718f, -0.147621f,
		 -0.500000f,  0.809017f, -0.309017f,
		 -0.525731f,  0.850651f,  0.000000f,
		  0.000000f,  0.850651f, -0.525731f,
		 -0.238856f,  0.864188f, -0.442863f,
		  0.000000f,  0.955423f, -0.295242f,
		 -0.262866f,  0.951056f, -0.162460f,
		  0.000000f,  1.000000f,  0.000000f,
		  0.000000f,  0.955423f,  0.295242f,
		 -0.262866f,  0.951056f,  0.162460f,
		  0.238856f,  0.864188f,  0.442863f,
		  0.262866f,  0.951056f,  0.162460f,
		  0.500000f,  0.809017f,  0.309017f,
		  0.238856f,  0.864188f, -0.442863f,
		  0.262866f,  0.951056f, -0.162460f,
		  0.500000f,  0.809017f, -0.309017f,
		  0.850651f,  0.525731f,  0.000000f,
		  0.716567f,  0.681718f,  0.147621f,
		  0.716567f,  0.681718f, -0.147621f,
		  0.525731f,  0.850651f,  0.000000f,
		  0.425325f,  0.688191f,  0.587785f,
		  0.864188f,  0.442863f,  0.238856f,
		  0.688191f,  0.587785f,  0.425325f,
		  0.809017f,  0.309017f,  0.500000f,
		  0.681718f,  0.147621f,  0.716567f,
		  0.587785f,  0.425325f,  0.688191f,
		  0.955423f,  0.295242f,  0.000000f,
		  1.000000f,  0.000000f,  0.000000f,
		  0.951056f,  0.162460f,  0.262866f,
		  0.850651f, -0.525731f,  0.000000f,
		  0.955423f, -0.295242f,  0.000000f,
		  0.864188f, -0.442863f,  0.238856f,
		  0.951056f, -0.162460f,  0.262866f,
		  0.809017f, -0.309017f,  0.500000f,
		  0.681718f, -0.147621f,  0.716567f,
		  0.850651f,  0.000000f,  0.525731f,
		  0.864188f,  0.442863f, -0.238856f,
		  0.809017f,  0.309017f, -0.500000f,
		  0.951056f,  0.162460f, -0.262866f,
		  0.525731f,  0.000000f, -0.850651f,
		  0.681718f,  0.147621f, -0.716567f,
		  0.681718f, -0.147621f, -0.716567f,
		  0.850651f,  0.000000f, -0.525731f,
		  0.809017f, -0.309017f, -0.500000f,
		  0.864188f, -0.442863f, -0.238856f,
		  0.951056f, -0.162460f, -0.262866f,
		  0.147621f,  0.716567f, -0.681718f,
		  0.309017f,  0.500000f, -0.809017f,
		  0.425325f,  0.688191f, -0.587785f,
		  0.442863f,  0.238856f, -0.864188f,
		  0.587785f,  0.425325f, -0.688191f,
		  0.688191f,  0.587785f, -0.425325f,
		 -0.147621f,  0.716567f, -0.681718f,
		 -0.309017f,  0.500000f, -0.809017f,
		  0.000000f,  0.525731f, -0.850651f,
		 -0.525731f,  0.000000f, -0.850651f,
		 -0.442863f,  0.238856f, -0.864188f,
		 -0.295242f,  0.000000f, -0.955423f,
		 -0.162460f,  0.262866f, -0.951056f,
		  0.000000f,  0.000000f, -1.000000f,
		  0.295242f,  0.000000f, -0.955423f,
		  0.162460f,  0.262866f, -0.951056f,
		 -0.442863f, -0.238856f, -0.864188f,
		 -0.309017f, -0.500000f, -0.809017f,
		 -0.162460f, -0.262866f, -0.951056f,
		  0.000000f, -0.850651f, -0.525731f,
		 -0.147621f, -0.716567f, -0.681718f,
		  0.147621f, -0.716567f, -0.681718f,
		  0.000000f, -0.525731f, -0.850651f,
		  0.309017f, -0.500000f, -0.809017f,
		  0.442863f, -0.238856f, -0.864188f,
		  0.162460f, -0.262866f, -0.951056f,
		  0.238856f, -0.864188f, -0.442863f,
		  0.500000f, -0.809017f, -0.309017f,
		  0.425325f, -0.688191f, -0.587785f,
		  0.716567f, -0.681718f, -0.147621f,
		  0.688191f, -0.587785f, -0.425325f,
		  0.587785f, -0.425325f, -0.688191f,
		  0.000000f, -0.955423f, -0.295242f,
		  0.000000f, -1.000000f,  0.000000f,
		  0.262866f, -0.951056f, -0.162460f,
		  0.000000f, -0.850651f,  0.525731f,
		  0.000000f, -0.955423f,  0.295242f,
		  0.238856f, -0.864188f,  0.442863f,
		  0.262866f, -0.951056f,  0.162460f,
		  0.500000f, -0.809017f,  0.309017f,
		  0.716567f, -0.681718f,  0.147621f,
		  0.525731f, -0.850651f,  0.000000f,
		 -0.238856f, -0.864188f, -0.442863f,
		 -0.500000f, -0.809017f, -0.309017f,
		 -0.262866f, -0.951056f, -0.162460f,
		 -0.850651f, -0.525731f,  0.000000f,
		 -0.716567f, -0.681718f, -0.147621f,
		 -0.716567f, -0.681718f,  0.147621f,
		 -0.525731f, -0.850651f,  0.000000f,
		 -0.500000f, -0.809017f,  0.309017f,
		 -0.238856f, -0.864188f,  0.442863f,
		 -0.262866f, -0.951056f,  0.162460f,
		 -0.864188f, -0.442863f,  0.238856f,
		 -0.809017f, -0.309017f,  0.500000f,
		 -0.688191f, -0.587785f,  0.425325f,
		 -0.681718f, -0.147621f,  0.716567f,
		 -0.442863f, -0.238856f,  0.864188f,
		 -0.587785f, -0.425325f,  0.688191f,
		 -0.309017f, -0.500000f,  0.809017f,
		 -0.147621f, -0.716567f,  0.681718f,
		 -0.425325f, -0.688191f,  0.587785f,
		 -0.162460f, -0.262866f,  0.951056f,
		  0.442863f, -0.238856f,  0.864188f,
		  0.162460f, -0.262866f,  0.951056f,
		  0.309017f, -0.500000f,  0.809017f,
		  0.147621f, -0.716567f,  0.681718f,
		  0.000000f, -0.525731f,  0.850651f,
		  0.425325f, -0.688191f,  0.587785f,
		  0.587785f, -0.425325f,  0.688191f,
		  0.688191f, -0.587785f,  0.425325f,
		 -0.955423f,  0.295242f,  0.000000f,
		 -0.951056f,  0.162460f,  0.262866f,
		 -1.000000f,  0.000000f,  0.000000f,
		 -0.850651f,  0.000000f,  0.525731f,
		 -0.955423f, -0.295242f,  0.000000f,
		 -0.951056f, -0.162460f,  0.262866f,
		 -0.864188f,  0.442863f, -0.238856f,
		 -0.951056f,  0.162460f, -0.262866f,
		 -0.809017f,  0.309017f, -0.500000f,
		 -0.864188f, -0.442863f, -0.238856f,
		 -0.951056f, -0.162460f, -0.262866f,
		 -0.809017f, -0.309017f, -0.500000f,
		 -0.681718f,  0.147621f, -0.716567f,
		 -0.681718f, -0.147621f, -0.716567f,
		 -0.850651f,  0.000000f, -0.525731f,
		 -0.688191f,  0.587785f, -0.425325f,
		 -0.587785f,  0.425325f, -0.688191f,
		 -0.425325f,  0.688191f, -0.587785f,
		 -0.425325f, -0.688191f, -0.587785f,
		 -0.587785f, -0.425325f, -0.688191f,
		 -0.688191f, -0.587785f, -0.425325f};

	//Retorna se o sistema é little-endian
	bool littleEndian() {
		//O valor short 1 tem bytes (1, 0) em little-endian e (0, 1) em big-endian
		short s = 1;
		return (((char*)&s)[0]) == 1;
	}

	//Converte um vetor de 4 caracteres para int, usando little-endian form
	int toInt(const char* bytes) {
		return (int)(((unsigned char)bytes[3] << 24) |
					 ((unsigned char)bytes[2] << 16) |
					 ((unsigned char)bytes[1] << 8) |
					 (unsigned char)bytes[0]);
	}

	//Converte um vetor de 2 caracteres para short, usando little-endian form
	short toShort(const char* bytes) {
		return (short)(((unsigned char)bytes[1] << 8) |
					   (unsigned char)bytes[0]);
	}

	//Converte um vetor de 2 caracteres para unsigned short, usando little-endian form
	unsigned short toUShort(const char* bytes) {
		return (unsigned short)(((unsigned char)bytes[1] << 8) |
								(unsigned char)bytes[0]);
	}

	//Converte um vetor de 4 caracteres para float, usando little-endian form
	float toFloat(const char* bytes) {
		float f;
		if (littleEndian()) {
			((char*)&f)[0] = bytes[0];
			((char*)&f)[1] = bytes[1];
			((char*)&f)[2] = bytes[2];
			((char*)&f)[3] = bytes[3];
		}
		else {
			((char*)&f)[0] = bytes[3];
			((char*)&f)[1] = bytes[2];
			((char*)&f)[2] = bytes[1];
			((char*)&f)[3] = bytes[0];
		}
		return f;
	}

	//Lê os próximos 4 bytes de um inteiro, utilizando little-endian form
	int readInt(ifstream &input) {
		char buffer[4];
		input.read(buffer, 4);
		return toInt(buffer);
	}

	//Lê os próximos 2 bytes como short, utilizando little-endian form
	short readShort(ifstream &input) {
		char buffer[2];
		input.read(buffer, 2);
		return toShort(buffer);
	}

	//Lê os próximos 2 bytes como unsigned short, utilizando little-endian form
	unsigned short readUShort(ifstream &input) {
		char buffer[2];
		input.read(buffer, 2);
		return toUShort(buffer);
	}

	//Lê os próximos 4 bytes como float, utilizando little-endian form
	float readFloat(ifstream &input) {
		char buffer[4];
		input.read(buffer, 4);
		return toFloat(buffer);
	}

	//Chama readFload() três vezes e retorna os resultados como um objeto Vec3f
	Vec3f readVec3f(ifstream &input) {
		float x = readFloat(input);
		float y = readFloat(input);
		float z = readFloat(input);
		return Vec3f(x, y, z);
	}

	//Torna a imagem em uma textura, e retorna o ID da textura
	GLuint loadTexture(Image *image) {
		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGB,
					 image->width, image->height,
					 0,
					 GL_RGB,
					 GL_UNSIGNED_BYTE,
					 image->pixels);
		return textureId;
	}
}

MD2Model::~MD2Model() {
	if (frames != NULL) {
		for(int i = 0; i < numFrames; i++) {
			delete[] frames[i].vertices;
		}
		delete[] frames;
	}

	if (texCoords != NULL) {
		delete[] texCoords;
	}
	if (triangles != NULL) {
		delete[] triangles;
	}
}

MD2Model::MD2Model() {
	frames = NULL;
	texCoords = NULL;
	triangles = NULL;
}

//Carrega o MD2 model
MD2Model* MD2Model::load(const char* filename,
						 vector<const char*> textureFilenames) {
	ifstream input;
	input.open(filename, istream::binary);

	char buffer[64];
	input.read(buffer, 4); //Deve ser "IPD2", se for um arquivo MD2
	if (buffer[0] != 'I' || buffer[1] != 'D' ||
		buffer[2] != 'P' || buffer[3] != '2') {
		return NULL;
	}
	if (readInt(input) != 8) { //O número da versão
		return NULL;
	}

	int textureWidth = readInt(input);   //A largura das texturas
	int textureHeight = readInt(input);  //A altura das texturas
	readInt(input);                      //Número de bytes por frame
	int numTextures = readInt(input);    //O número de texturas
	if (numTextures != 1) {
		return NULL;
	}
	int numVertices = readInt(input);    //Número de vértices
	int numTexCoords = readInt(input);   //O número de coordenadas de textura
	int numTriangles = readInt(input);   //Número de triângulos
	readInt(input);                      //Número de comandos OpenGL
	int numFrames = readInt(input);      //Número de frames

	//Offsets (número de bytes depois do começo do arquivo até o início de onde
	//certo dado apareceu)
	readInt(input);                      //Offset das texturas
	int texCoordOffset = readInt(input); //Offset das coordenadas de textura
	int triangleOffset = readInt(input); //Offset dos triângulos
	int frameOffset = readInt(input);    //Offset dos frames
	readInt(input);                      //Offset dos comandos OpenGL
	readInt(input);                      //Offset para o fim do arquivo

	//Carrega as texturas (ignora a textura sugerida pelo arquivo MD2)
	MD2Model* model = new MD2Model();
	for(unsigned int i = 0; i < textureFilenames.size(); i++) {
		const char* f = textureFilenames[i];
		if (strlen(f) < 5 ||
			strcmp(f + strlen(f) - 4, ".bmp") != 0) {
			delete model;
			return NULL;
		}
		Image* image = loadBMP(f);
		GLuint textureId = loadTexture(image);
		delete image;
		model->textureIds.push_back(textureId);
	}

	//Carrega as coordenadas de textura
	input.seekg(texCoordOffset, ios_base::beg);
	model->texCoords = new MD2TexCoord[numTexCoords];
	for(int i = 0; i < numTexCoords; i++) {
		MD2TexCoord* texCoord = model->texCoords + i;
		texCoord->texCoordX = (float)readShort(input) / textureWidth;
		texCoord->texCoordY = 1 - (float)readShort(input) / textureHeight;
	}

	//Carrega os triângulos
	input.seekg(triangleOffset, ios_base::beg);
	model->triangles = new MD2Triangle[numTriangles];
	model->numTriangles = numTriangles;
	for(int i = 0; i < numTriangles; i++) {
		MD2Triangle* triangle = model->triangles + i;
		for(int j = 0; j < 3; j++) {
			triangle->vertices[j] = readUShort(input);
		}
		for(int j = 0; j < 3; j++) {
			triangle->texCoords[j] = readUShort(input);
		}
	}

	//Carrega os frames
	input.seekg(frameOffset, ios_base::beg);
	model->frames = new MD2Frame[numFrames];
	model->numFrames = numFrames;
	for(int i = 0; i < numFrames; i++) {
		MD2Frame* frame = model->frames + i;
		frame->vertices = new MD2Vertex[numVertices];
		Vec3f scale = readVec3f(input);
		Vec3f translation = readVec3f(input);
		input.read(frame->name, 16);

		for(int j = 0; j < numVertices; j++) {
			MD2Vertex* vertex = frame->vertices + j;
			input.read(buffer, 3);
			Vec3f v((unsigned char)buffer[0],
					(unsigned char)buffer[1],
					(unsigned char)buffer[2]);
			vertex->pos = translation + Vec3f(scale[0] * v[0],
											  scale[1] * v[1],
											  scale[2] * v[2]);
			input.read(buffer, 1);
			int normalIndex = (int)((unsigned char)buffer[0]);
			vertex->normal = Vec3f(NORMALS[3 * normalIndex],
								   NORMALS[3 * normalIndex + 1],
								   NORMALS[3 * normalIndex + 2]);
		}
	}

	model->startFrame = 0;
	model->endFrame = numFrames - 1;
	return model;
}

void MD2Model::setAnimation(const char* name) {
	/* Os nomes dos frames começam normalmente com o nome da animação na qual estão,
	 * por exemplo, "Run", e são seguidos por um caractere não alfabético.
	 * Normalmente, indicam o seu número de frame na animação, p. "Run_1", "run_2", etc.
	 */
	bool found = false;
	for(int i = 0; i < numFrames; i++) {
		MD2Frame* frame = frames + i;
		if (strlen(frame->name) > strlen(name) &&
			strncmp(frame->name, name, strlen(name)) == 0 &&
			!isalpha(frame->name[strlen(name)])) {
			if (!found) {
				found = true;
				startFrame = i;
			}
			else {
				endFrame = i;
			}
		}
		else if (found) {
			break;
		}
	}
}

void MD2Model::draw(int textureNum, float time) {
	if (time > -100000000 && time < 1000000000) {
		time -= (int)time;
		if (time < 0) {
			time += 1;
		}
	}
	else {
		time = 0;
	}

	glEnable(GL_TEXTURE_2D);
	//Usa a textura apropriada
	glBindTexture(GL_TEXTURE_2D, textureIds[textureNum]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Descobrir os dois quadros entre os quais estamos interpolando
	int frameIndex1 = (int)(time * (endFrame - startFrame + 1)) + startFrame;
	if (frameIndex1 > endFrame) {
		frameIndex1 = startFrame;
	}

	int frameIndex2;
	if (frameIndex1 < endFrame) {
		frameIndex2 = frameIndex1 + 1;
	}
	else {
		frameIndex2 = startFrame;
	}

	MD2Frame* frame1 = frames + frameIndex1;
	MD2Frame* frame2 = frames + frameIndex2;

	//Descobrir a fração que estamos entre os dois frames
	float frac =
		(time - (float)(frameIndex1 - startFrame) /
		 (float)(endFrame - startFrame + 1)) * (endFrame - startFrame + 1);

	//Desenha o modelo como uma interpolação entre os frames
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < numTriangles; i++) {
		MD2Triangle* triangle = triangles + i;
		for(int j = 2; j >= 0; j--) {
			MD2Vertex* v1 = frame1->vertices + triangle->vertices[j];
			MD2Vertex* v2 = frame2->vertices + triangle->vertices[j];
			Vec3f pos = v1->pos * (1 - frac) + v2->pos * frac;
			Vec3f normal = v1->normal * (1 - frac) + v2->normal * frac;
			if (normal[0] == 0 && normal[1] == 0 && normal[2] == 0) {
				normal = Vec3f(0, 0, 1);
			}
			glNormal3f(normal[0], normal[1], normal[2]);

			MD2TexCoord* texCoord = texCoords + triangle->texCoords[j];
			glTexCoord2f(texCoord->texCoordX, texCoord->texCoordY);
			glVertex3f(pos[0], pos[1], pos[2]);
		}
	}
	glEnd();
}