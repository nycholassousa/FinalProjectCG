#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

const float PI = 3.1415926535f;
const int TIMER_MS = 25;

namespace gameinfo{	
	//Score inicial do jogo
	const int INIT_SCORE = 10;
	//Dificuldade do IA, considere como porcentagem, quanto maior, mais dificil o jogo fica
	const float CRAB_IA_DIFICULT = 0.05f;
	//Velocidade máxima da movimentação do IA
	const float MAX_SPEED_IA = 2.2f;
	//Quantas vezes o caranguejo e a bola irão avançar com o método step()
	const float CRAB_STEP_TIME = 0.01f;
	//Tempo necessário para sair da velocidade 0 até velocidade máxima
	const float TIME_TO_MAXIMUM_SPEED = 0.15f;
	//Ângulo máximo da nova trajetória da bola ao bater na extremo lateral do caranguejo
	const float MAX_CRAB_BOUNCE_ANGLE_OFFSET = 0.85f * PI / 2;
	//Velocidade máxima do crab do jogador
	const float PLAYER_MAXIMUM_SPEED = 2.0f;
	//Tempo para uma nova bola aparecer na arena
	const float BALL_FADE_IN_TIME = 0.5f;
	//Tempo para uma bola fora da arena desaparecer ao passar
	const float BALL_FADE_OUT_TIME = 0.7f;
	//Raio da bola
	const float BALL_RADIUS = 0.04f;
	//Velocidade das bolas
	const float BALL_SPEED = 0.9f;
	//Número máximo de bolas simultâneas na arena
	const int NUM_BALLS = 3;
	//Número de vezes que o jogo avança cada vez que step() é chamado
	const float GAME_STEP_TIME = 0.01f;
	//Hitbox do caranguejo
	const float CRAB_LENGTH = 0.35f;
	//O raio das barreiras posiciondas nos cantos
	const float BARRIER_SIZE = 0.10f;
}

namespace gamedraw{	
	//Quanto de tempo cada estado do jogo avança em relação a cada chamada do método step()
	const float STEP_TIME = 0.01f;
	//A duração do loop da animação de andar
	const float WALK_ANIM_TIME = 0.4f;
	//Duração do ciclo de animação em pé
	const float STAND_ANIM_TIME = 2.0f;
	//Fade out do caranguejo após "morrer"
	const float CRAB_FADE_OUT_TIME = 1.5f;
	//Número de pontos usados para aproximar de um circulo na barreira lateral
	const int NUM_BARRIER_POINTS = 30;
	//Numero de pontos usados para aproximar de um circulo na barra
	const int NUM_POLE_POINTS = 6;
	//Altura da barreira
	const float BARRIER_HEIGHT = gameinfo::BARRIER_SIZE;
	//Radio da barra
	const float POLE_RADIUS = 0.02f;
	//Altura da barra em relação à agua
	const float POLE_HEIGHT = 0.07f;
	//Numero de unidades que o caranguejo vai ser transladado na direção z
	const float CRAB_OFFSET = -0.032f;
	//Quantidade em tempo até que a agua percorra WATER_TEXTURE_SIZE unidades
	const float WATER_TEXTURE_TIME = 8.0f;
	//Comprimento de uma simples repetição da textura da agua
	const float WATER_TEXTURE_SIZE = 0.7f;
	//Opacidade da agua
	const float WATER_ALPHA = 0.8f;
}

#endif