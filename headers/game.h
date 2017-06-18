#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <cmath>
#include <cstdlib>
#include <math.h>
#include <vector>
#include "const.h"

//Representa um único caranguejo
class Crab {
	private:
		//Velocidade máxima
		float _maximumSpeed;
		//A distância do centro do caranguejo ao canto à sua direita
		float _position;
		//-1 se o caranguejo está acelerando em direção ao canto à sua direita, 
		//1 se acelerar em direção ao outro canto
		//0 se estiver desacelerando
		int _direction;
		//Velocidade do caranguejo
		float _speed;
		//A quantidade de tempo de jogo até a próxima chamada para step()
		float _timeUntilNextStep;
		//Avança o estado do caranguejo por um curto período de tempo
		void step();
	public:
		//Constrói um novo Caranguejo com a velocidade máxima especificada
		Crab(float maximumSpeed);
		//Retorna a distancia do centro do caranguejo até o canto à sua direita
		float getPosition();
		//-1 se ele está acelerando pra direita
		//1 se acelera pra esquerda
		//0 se desacelera
		int getDirection();
		//Retorna a velocidade do caranguejo
		float getSpeed();
		//Retorna a aceleração ou desaceleração
		float getAcceleration();
		//-1 se o caranguejo está acelerando em direção ao canto à sua direita, 
		//1 se acelerar em direção ao outro canto
		//0 se estiver desacelerando
		void setDirection(int direction);
		//Avança o estado do caranguejo em uma certa fração de tempo
		void advance(float dt);
};

//Representa uma única bola
class Ball {
	private:
		//Raio da bola
		float _radius;
		//Coordenada x
		float _xAxis;
		//Coordenada z
		float _zAxis;
		//Angula da trajetoria da bola
		//0 - Direção positiva em x
		// PI / 2 - Direção positiva em z
		float _angle;
		//Fração em que a bola está aparecendo, de 0 a 1
		//Diferente de 1 quando a bola está aparecendo ou sumindo
		float _fadeAmount;
		//Verifica se a bola está desaparecendo
		bool _isFadingOut;
	public:
		//Construtor de uma bola
		Ball(float radius, float xAxis, float zAxis, float angle);
		
		//Retorna o raio da bola
		float getRadius();
		//Retorna a coordenada x da bola
		float getXAxis();
		//Retorna a coordenada z da bola
		float getZAxis();
		//Retorna o angulo da bola
		float getAngle();
		//Seta o angulo da bola
		void setAngle(float angle);
		//Retorna a fração em que a bola está aparecendo, de 0 a 1
		//Diferente de 1 quando a bola está aparecendo ou sumindo
		float fadeAmount();
		void fadeOut();
		bool isFadingOut();
		//Avança o estado da bola em uma fração do tempo
		void advance(float dt);
};

//Gerencia o estado do jogo
class Game {
	private:
		//Os quatro caranguejos(paddles)
		Crab* _crabs[4];
		//As bolas que ainda estão em jogo
		std::vector<Ball*> _balls;
		//Os scores de cada player/caranguejo
		int scores[4];
		//Quanto de tempo do jogo até a proxima chamada do step()
		float _timeUntilNextStep;
		
		//Gerencia todas as colisões
		void handleCollisions();
		//Ajusta as direções que a IA deve-se mexer
		//Sistema simples, o caranguejo vai pra bola mais próxima dele
		void IA();
		//Avança o estado do jogo em um curto periodo de tempo
		void step();
	public:
		//Construtor do jogo, gerencia velocidade da IA e score inicial
		Game(float maximumSpeedForOpponents, int startingScore);
		~Game();
		
		//Gerencia a direção do caranguejo controlado pelo jogador
		//Negativo direita, positivo esquerda, 0 desacelerado
		void setPlayerCrabDir(int dir);
		//Retorna o score do caranguejo especificado
		int score(int crabNum);
		//Avança o estado do jogo
		void advance(float dt);
		//Retorna todos os caranguejos
		//Se nulo, foi eliminado (score 0)
		Crab** crabs();
		//Retorna as bolas em jogo
		std::vector<Ball*> balls();
};
#endif