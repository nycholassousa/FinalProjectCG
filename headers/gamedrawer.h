#ifndef GAME_DRAWER_H_INCLUDED
#define GAME_DRAWER_H_INCLUDED

#include <GL/glut.h>

#include "imageloader.h"
#include "md2model.h"
#include "text3d.h"
#include "game.h"

class Game;
class MD2Model;

//Mantém o estado do jogo utilizando objeto Game
class GameDrawer {
	private:
		Game* game;
		//Modelo do crab
		MD2Model* crabModel;
		//O ID para a lista de exibição das 4 barreiras no canto
		GLuint barriersDisplayListId;
		//O ID da lista de exibição para a "barra" que surge quando um crab é eliminado.
		GLuint poleDisplayListId;
		//O ID da textura para a madeira
		GLuint woodTextureId;
		//O ID da textura para a água
		GLuint waterTextureId;
		//A fração que cada Crab está "faded in", de 0 a 1.
		//Quando não é 1, o crab está desaparecendo ou já desapareceu completamente
		//após ser eliminado.
		float crabFadeAmounts[4];
		//Os tempos de animação para o modelo de Crab para os quatro crabs
		float animTimes[4];
		//A última posição conhecida de cada crab. Mantida quando um crab se torna NULL
		//depois de ser eliminado
		float oldCrabPos[4];
		//A distância que a água percorreu
		float waterTextureOffset;
		//Informa se está em Game Over ou não
		bool isGameOver0;
		//Informa de algum jogo já foi iniciado (ou finalizado)
		bool waitingForFirstGame;
		//Guarda um número negativo se o jogador está acelerando o crab para a direita
		//Guarda um número positivo se o jogador está acelerando para o outro lado
		//Guarda 0 se o crab do jogador está desacelerando
		int playerCrabDir;
		//O tempo de jogo até a próxima chamada de step()
		float _timeUntilNextStep;

		//Muda para usar o objeto Game especificado
		void setGame(Game* game);
		//Seta a lista de exibição para as 4 barreiras no canto
		void setupBarriers();
		//Seta a lista de exibição para a barra
		void setupPole();
		//Avança o estado do jogo por um curto tempo
		void step();

		//Seta a iluminação do openGL
		void setupLighting();
		//Faz os desenhos dos objetos descritos
		void drawCrabsAndPoles();
		void drawBarriers();
		void drawScores();
		void drawBalls();
		void drawWood();
		void drawWater();
		//Desenha o texto indicando o vencedor (e o texto adicional)
		void drawWinner();
	public:
		GameDrawer();
		~GameDrawer();

		//Desenha o jogo posicionado de acordo com as coordenadas da fronteira, para que
		//os cantos da fronteira estejam em (0, 0, 0), (0, 0, 1), (1, 0, 0), e (1, 0, 1).
		void draw();
		//Avança o estado do jogo por um tempo especificado
		void advance(float dt);
		//Seta o PlayerCrabDir
		void setPlayerCrabDir(int dir);
		//Retorna False se o jogo não estiver com uma partida em andamento
		bool isGameOver();
		//Inicia um novo jogo com a pontuação inicial especificada (startingScore)
		//maximumSpeedForOpponents é a velocidade máxima dos adversários
		//Pode ser setada para aumentar/diminuir a dificuldade
		void startNewGame(float maximumSpeedForOpponents, int startingScore);
};

//Executa a inicialização necessária para que o GameDrawer funcione corretamente
void initGameDrawer();
//Libera alguns recursos usados ​​pela classe GameDrawer após uma
//chamada para initGameDrawer ().
void cleanupGameDrawer();

#endif