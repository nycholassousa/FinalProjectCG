#ifndef MD2_MODEL_H_INCLUDED
#define MD2_MODEL_H_INCLUDED

#include <vector>
#include <GL/glut.h>

#include "vec3f.h"

struct MD2Vertex {
	Vec3f pos;
	Vec3f normal;
};

struct MD2Frame {
	char name[16];
	MD2Vertex* vertices;
};

struct MD2TexCoord {
	float texCoordX;
	float texCoordY;
};

struct MD2Triangle {
	int vertices[3];  //Os índices dos vértices neste triângulo
	int texCoords[3]; //Os índices das coordenadas de textura do triângulo
};

class MD2Model {
	private:
		MD2Frame* frames;
		int numFrames;
		MD2TexCoord* texCoords;
		MD2Triangle* triangles;
		int numTriangles;
		std::vector<GLuint> textureIds;

		int startFrame; //O primeiro frame da animação atual
		int endFrame;   //O último frame da animação atual

		MD2Model();
	public:
		~MD2Model();

		//Muda para a animação dada
		void setAnimation(const char* name);
		/* Desenha o estado do modelo animado na hora especificada na animação.
		 * Um tempo de i, inteiro i, indica o início da animação,
		 * e um tempo de i + 0,5 indica a metade da animação.
		 * TextureNum é o índice da textura que é usada ao desenhar o modelo.
		 */
		void draw(int textureNum, float time);

		//Carrega um MD3Model do arquivo especificado, carregando texturas de imagens
		//dos arquivos indicados. Retorna NULL se ocorrer algum erro enquanto carrega
		static MD2Model* load(const char* filename,
							  std::vector<const char*> textureFilenames);
};

#endif