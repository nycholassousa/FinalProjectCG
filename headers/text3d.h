#ifndef TEXT_3D_H_INCLUDED
#define TEXT_3D_H_INCLUDED

#include <string>

//Inicia texto 3D. Deve ser chamado antes de outras funções.
void t3dInit();
//Libera memória alocada para texto 3D. Nenhuma outra função deste header
//deve ser chamada após esta.
void t3dCleanup();
/* Desenha a string especificada, usando OpenGL, como um conjunto de polígonos
 * no plano x-y, onde o topo das letras tem a maior coordenada y. As normais
 * apontam para a direção positiva de z. (Se precisar que as normais apontem
 * na direção positiva de alguns caracteres e negativa de outros,
 * chame t3dDraw3D com uma profundidade muito baixa.)
 *
 * A seqüência é desenhada alinhada à esquerda se hAlign for negativa,
 * alinhada à direita se for positiva e centralizada horizontalmente se for 0.
 * A seqüência é desenhada alinhada ao topo se vAlign for negativa,
 * alinhada ao fundo se for positiva e centrada Verticalmente se for 0.
 *
 * A string pode ter caracteres de nova linha,
 * caso em que a seqüência de caracteres será desenhada em várias linhas
 * como seria de esperar. As linhas são traçadas lineHeight vezes a altura da fonte separada.
 * A altura da fonte é a altura "normal" das letras maiúsculas,
 * em vez da distância do topo das letras maiúsculas "normais" para
 * a parte inferior das letras minúsculas como "p".
 *
 * Todos os caracteres ASCII que não possam ser printados serão desenhados como espaços.
 */
void t3dDraw2D(std::string str,
			   int hAlign, int vAlign,
			   float lineHeight = 1.5f);
/* Desenha a string especificada, usando OpenGL, usando polígonos como um prisma direito,
 * onde as faces paralelas são letras paralelas ao plano x-y,
 * com o topo das letras com a maior coordenada y.
 *
 * A string é desenhada alinhada à esquerda se hAlign for negativa,
 * alinhada à direita se for positiva e centralizada horizontalmente se for 0.
 * A seqüência é desenhada alinhada por cima se vAlign for negativa,
 * alinhada por baixo se for positiva e centrada Verticalmente se for 0.
 *
 * A string pode ter caracteres de nova linha,
 * caso em que a seqüência de caracteres será desenhada em várias linhas
 * como seria de esperar. As linhas são traçadas lineHeight vezes a altura da fonte separada.
 * A altura da fonte é a altura "normal" das letras maiúsculas,
 * em vez da distância do topo das letras maiúsculas "normais" para
 * a parte inferior das letras minúsculas como "p".
 *
 * A profundidade dos caracteres é a profundidade vezes a altura da fonte.
 * Os caracteres são centralizados em z = 0.
 *
 * Todos os caracteres ASCII que não possam ser printados serão desenhados como espaços.
 */
void t3dDraw3D(std::string str,
			   int hAlign, int vAlign,
			   float depth,
			   float lineHeight = 1.5f);
/* Retorna a largura de desenho da string especificada, como um múltiplo da altura da fonte.
 * A altura da fonte é a altura "normal" das letras maiúsculas,
 * em vez da distância do topo das letras maiúsculas "normais"
 * para a parte inferior das letras minúsculas como "p".
 * A largura é a mesma que a largura da linha mais longa.
 */
float t3dDrawWidth(std::string str);
/* Retorna a altura do desenho da string especificada, como um múltiplo da altura da fonte.
 * A altura da fonte é a altura "normal" das letras maiúsculas,
 * em vez da distância do topo das letras maiúsculas "normais" para a parte inferior
 * das letras minúsculas como "p". O draw é lineHeight vezes um menos do que o número
 * de linhas na seqüência de caracteres, mais 1.
 */
float t3dDrawHeight(std::string str, float lineHeight = 1.5f);

//Indica que ocorreu uma exceção ao configurar o texto 3D
class T3DLoadException {
	private:
		std::string message0;
	public:
		T3DLoadException(std::string message1);
		std::string message() const;
};

#endif