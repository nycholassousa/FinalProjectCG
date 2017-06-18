#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED

//Representa uma imagem
class Image {
	public:
		Image(char* ps, int w, int h);
		~Image();

		/* Um vetor de formato (R1, G1, B1, R2, G2, B2, ...) incicando a cor
		 * de cada pixel na imagem. Cada componente de cor vai de 0 a 255.
		 * O vetor inicia no pixel de baixo à esquerda, então move para a direita
		 * até o fim da linha, e então move para a próxima coluna, e continua
		 * o processo. Esse é o formato do OpenGL com imagens.
		 */
		char* pixels;
		int width;
		int height;
};

//Lê um arquivo imagem bitmap
Image* loadBMP(const char* filename);

#endif