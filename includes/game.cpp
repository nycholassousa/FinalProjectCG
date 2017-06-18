#include "../headers/game.h"

using namespace std;
using namespace gameinfo;

/** Início das funções em relação ao caranguejo **/
Crab::Crab(float maximumSpeed) {
	_maximumSpeed = maximumSpeed;
	_position = 0.5f;
	_direction = 0;
	_speed = 0;
	_timeUntilNextStep = 0;
}

void Crab::step() {
	//Aceleração e desaceleração do crab
	float ds = CRAB_STEP_TIME * getAcceleration();
	if (_direction != 0) {
		_speed += _direction * ds;
		if (_speed > _maximumSpeed) {
			_speed = _maximumSpeed;
		}
		else if (_speed < -_maximumSpeed) {
			_speed = -_maximumSpeed;
		}
	}
	else {
		float s = abs(_speed);
		s -= ds;
		if (s < 0) {
			s = 0;
		}
		if (_speed > 0) {
			_speed = s;
		}
		else {
			_speed = -s;
		}
	}
	
	//Mover o crab
	_position += CRAB_STEP_TIME * _speed;
	if (_position < BARRIER_SIZE + CRAB_LENGTH / 2) {
		_position = BARRIER_SIZE + CRAB_LENGTH / 2;
		_speed = 0;
	}
	else if (_position > 1 - BARRIER_SIZE - CRAB_LENGTH / 2) {
		_position = 1 - BARRIER_SIZE - CRAB_LENGTH / 2;
		_speed = 0;
	}
}

float Crab::getPosition() {
	return _position;
}

int Crab::getDirection() {
	return _direction;
}

float Crab::getSpeed() {
	return _speed;
}

float Crab::getAcceleration() {
	return _maximumSpeed / TIME_TO_MAXIMUM_SPEED;
}

void Crab::setDirection(int dir1) {
	if (dir1 < 0) {
		_direction = -1;
	}
	else if (dir1 > 0) {
		_direction = 1;
	}
	else {
		_direction = 0;
	}
}

void Crab::advance(float dt) {
	while (dt > 0) {
		if (_timeUntilNextStep < dt) {
			dt -= _timeUntilNextStep;
			step();
			_timeUntilNextStep = CRAB_STEP_TIME;
		}
		else {
			_timeUntilNextStep -= dt;
			dt = 0;
		}
	}
}

/** Fim das funções em relação ao caranguejo **/

/** Inicio das funções em relação à bola **/
Ball::Ball(float radius1, float x1, float z1, float angle1) {
	_radius = radius1;
	_xAxis = x1;
	_zAxis = z1;
	_angle = angle1;
	_fadeAmount = 0;
	_isFadingOut = false;
}

float Ball::getRadius() {
	return _radius;
}

float Ball::getXAxis() {
	return _xAxis;
}

float Ball::getZAxis() {
	return _zAxis;
}

float Ball::getAngle() {
	return _angle;
}

void Ball::setAngle(float angle1) {
	_angle = angle1;
}

float Ball::fadeAmount() {
	return _fadeAmount;
}

void Ball::fadeOut() {
	_isFadingOut = true;
}

bool Ball::isFadingOut() {
	return _isFadingOut;
}

void Ball::advance(float dt) {
	if (_isFadingOut) {
		//Fade out
		_fadeAmount -= dt / BALL_FADE_OUT_TIME;
		if (_fadeAmount < 0) {
			_fadeAmount = 0;
		}
	}
	else if (_fadeAmount < 1) {
		//Fade in
		_fadeAmount += dt / BALL_FADE_IN_TIME;
		if (_fadeAmount > 1) {
			dt = (_fadeAmount - 1) * BALL_FADE_IN_TIME;
			_fadeAmount = 1;
		}
		else {
			dt = 0;
		}
	}
	
	if (dt <= 0) {
		return;
	}
	
	//Move a bola
	_xAxis += dt * cos(_angle) * BALL_SPEED;
	_zAxis += dt * sin(_angle) * BALL_SPEED;
}

/** Fim das funções em relação à bola **/

/** Inicio das funções em relação ao jogo **/
Game::Game(float maximumSpeedForOpponents, int startingScore) {
	if (startingScore > 0) {
		_crabs[0] = new Crab(PLAYER_MAXIMUM_SPEED);
		for(int i = 1; i < 4; i++) {
			_crabs[i] = new Crab(maximumSpeedForOpponents);
		}
	}
	else {
		for(int i = 0; i < 4; i++) {
			_crabs[i] = NULL;
		}
	}
	
	for(int i = 0; i < 4; i++) {
		scores[i] = startingScore;
	}
	_timeUntilNextStep = 0;
}

Game::~Game() {
	for(int i = 0; i < 4; i++) {
		if (_crabs[i] != NULL) {
			delete _crabs[i];
		}
	}
	for(unsigned int i = 0; i < _balls.size(); i++) {
		delete _balls[i];
	}
}
/** Fim das funções em relação ao jogo **/

//Retorna um float entre 0 e 1
//(direção inicial da bola)
float randomFloat() {
	return (float)rand() / ((float)RAND_MAX + 1);
}

//Retorna se (dx, dz) está entre as unidades (0,0) de _radius
bool intersectsCircle(float dx, float dz, float _radius) {
	return dx * dx + dz * dz < _radius * _radius;
}

//Retorna se a bola está colidindo com um circulo
bool collisionWithCircle(float dx, float dz, float _radius, float vx, float vz) {
	return intersectsCircle(dx, dz, _radius) && vx * dx + vz * dz > 0;
}


float reflect(float angle, float normal) {
	angle = 2 * normal - PI - angle;
	while (angle < 0) {
		angle += 2 * PI;
	}
	while (angle > 2 * PI) {
		angle -= 2 * PI;
	}
	return angle;
}

//Ajusta o ângulo da bola em resposta a uma colisão com um círculo na posição
void collideWithCircle(Ball* ball, float x, float z) {
	if (ball->fadeAmount() < 1) {
		return;
	}
	
	float dx = x - ball->getXAxis();
	float dz = z - ball->getZAxis();
	float normal = atan2(-dz, -dx);
	float newBallAngle = reflect(ball->getAngle(), normal);
	if (newBallAngle < 0) {
		newBallAngle += 2 * PI;
	}
	else if (newBallAngle > 2 * PI) {
		newBallAngle -= 2 * PI;
	}
	ball->setAngle(newBallAngle);
}

//Retorna se um caranguejo na posição indicada interceptou uma bola na posição indicada, 
//onde as posições são medidas paralelamente à em que o caranguejo se move
bool collisionWithCrab(float crabPos, float ballPos) {
	return abs(crabPos - ballPos) < CRAB_LENGTH / 2;
}

//Ajusta o ângulo da bola em resposta a uma colisão com um caranguejo.
void collideWithCrab(Ball* ball,
					 int crabIndex,
					 float crabPos,
					 float ballPos) {
	float angle = (1 - crabIndex) * PI / 2 +
		MAX_CRAB_BOUNCE_ANGLE_OFFSET *
			(crabPos - ballPos) / (CRAB_LENGTH / 2);
	while (angle < 0) {
		angle += 2 * PI;
	}
	while (angle >= 2 * PI) {
		angle -= 2 * PI;
	}
	ball->setAngle(angle);
}

//Função onde o jogo gerencia as colisões
void Game::handleCollisions() {
	for(unsigned int i = 0; i < _balls.size(); i++) {
		Ball* ball = _balls[i];
		
		if (ball->fadeAmount() < 1 || ball->isFadingOut()) {
			continue;
		}
		
		//Colisão bola-barreira
		for(float z = 0; z < 2; z += 1) {
			for(float x = 0; x < 2; x += 1) {
				if (collisionWithCircle(x - ball->getXAxis(), z - ball->getZAxis(),
										ball->getRadius() + BARRIER_SIZE,
										BALL_SPEED * cos(ball->getAngle()),
										BALL_SPEED * sin(ball->getAngle()))) {
					collideWithCircle(ball, x, z);
				}
			}
		}
		
		//Colisão entre bolas
		for(unsigned int j = i + 1; j < _balls.size(); j++) {
			Ball* ball2 = _balls[j];
			if (collisionWithCircle(ball2->getXAxis() - ball->getXAxis(),
									ball2->getZAxis() - ball->getZAxis(),
									ball->getRadius() + ball2->getRadius(),
									BALL_SPEED * (cos(ball->getAngle()) -
												  cos(ball2->getAngle())),
									BALL_SPEED * (sin(ball->getAngle()) -
												  sin(ball2->getAngle())))) {
				collideWithCircle(ball, ball2->getXAxis(), ball2->getZAxis());
				collideWithCircle(ball2, ball->getXAxis(), ball->getZAxis());
			}
		}
		
		//Colisão bola-caranguejo (e bola-barreira)
		int crabIndex;
		float ballPos;
		if (ball->getZAxis() < ball->getRadius()) {
			crabIndex = 0;
			ballPos = ball->getXAxis();
		}
		else if (ball->getXAxis() < ball->getRadius()) {
			crabIndex = 1;
			ballPos = 1 - ball->getZAxis();
		}
		else if (ball->getZAxis() > 1 - ball->getRadius()) {
			crabIndex = 2;
			ballPos = 1 - ball->getXAxis();
		}
		else if (ball->getXAxis() > 1 - ball->getRadius()) {
			crabIndex = 3;
			ballPos = ball->getZAxis();
		}
		else {
			crabIndex = -1;
			ballPos = 0;
		}
		
		if (crabIndex >= 0) {
			if (_crabs[crabIndex] != NULL) {
				float crabPos = _crabs[crabIndex]->getPosition();
				if (collisionWithCrab(crabPos, ballPos)) {
					collideWithCrab(ball, crabIndex, crabPos, ballPos);
				}
			}
			else {
				float normal = (1 - crabIndex) * PI / 2;
				float newAngle = reflect(ball->getAngle(), normal);
				ball->setAngle(newAngle);
			}
		}
	}
}

//Retorna a posição na qual o caranguejo especificado pára se ele começa a desacelerar
float stopPos(Crab* crab) {
	float d = crab->getSpeed() * crab->getSpeed() / crab->getAcceleration();
	if (crab->getSpeed() > 0) {
		return crab->getPosition() + d;
	}
	else {
		return crab->getPosition() - d;
	}
}

//Inteligência artificial
void Game::IA() {
	for(int i = 1; i < 4; i++) {
		Crab* crab = _crabs[i];
		if (crab == NULL) {
			continue;
		}
		
		//Encontra a posição da bola que está mais próxima do lado do caranguejo, e o resultado em targetPos
		float closestBallDist = 100;
		float targetPos = 0.5f;
		for(unsigned int j = 0; j < _balls.size(); j++) {
			Ball* ball = _balls[j];
			
			float ballDist;
			float ballPos;
			switch(i) {
				case 1:
					ballDist = ball->getXAxis() - ball->getRadius();
					ballPos = 1 - ball->getZAxis();
					break;
				case 2:
					ballDist = 1 - ball->getZAxis() - ball->getRadius();
					ballPos = 1 - ball->getXAxis();
					break;
				case 3:
					ballDist = 1 - ball->getXAxis() - ball->getRadius();
					ballPos = ball->getZAxis();
					break;
			}
			
			if (ballDist < closestBallDist) {
				targetPos = ballPos;
				closestBallDist = ballDist;
			}
		}
		
		//Move-se para targetPos
		//Para quando a bola vai bater em ao menos CRAB_IA_DIFICULT do hitbox
		if (abs(stopPos(crab) - targetPos) < CRAB_IA_DIFICULT * (CRAB_LENGTH / 2)) {
			crab->setDirection(0);
		}
		else if (targetPos < crab->getPosition()) {
			crab->setDirection(-1);
		}
		else {
			crab->setDirection(1);
		}
	}
}

void Game::step() {
	//Avança os caranguejos
	for(int i = 0; i < 4; i++) {
		Crab* crab = _crabs[i];
		if (crab != NULL) {
			crab->advance(GAME_STEP_TIME);
		}
	}
	
	//Avança as bolas
	for(unsigned int i = 0; i < _balls.size(); i++) {
		_balls[i]->advance(GAME_STEP_TIME);
	}
	
	//Lidando com colisões - chamada de função 
	handleCollisions();
	
	//Checa as bolas em relação ao score
	vector<Ball*> newBalls;
	for(unsigned int i = 0; i < _balls.size(); i++) {
		Ball* ball = _balls[i];
		if (ball->fadeAmount() == 1 && !ball->isFadingOut()) {
			newBalls.push_back(ball);
			
			int scoredOn;
			if (ball->getZAxis() < ball->getRadius() && (ball->getAngle() > PI)) {
				scoredOn = 0;
			}
			else if (ball->getXAxis() < ball->getRadius() &&
					 (ball->getAngle() > PI / 2 && ball->getAngle() < 3 * PI / 2)) {
				scoredOn = 1;
			}
			else if (ball->getZAxis() > 1 - ball->getRadius() && (ball->getAngle() < PI)) {
				scoredOn = 2;
			}
			else if (ball->getXAxis() > 1 - ball->getRadius() &&
					 (ball->getAngle() < PI / 2 || ball->getAngle() > 3 * PI / 2)) {
				scoredOn = 3;
			}
			else {
				scoredOn = -1;
			}
			
			if (scoredOn >= 0 && _crabs[scoredOn] != NULL) {
				scores[scoredOn]--;
				if (scores[scoredOn] == 0) {
					delete _crabs[scoredOn];
					_crabs[scoredOn] = NULL;
				}
				
				ball->fadeOut();
			}
		}
		else if (ball->fadeAmount() > 0 || !ball->isFadingOut()) {
			newBalls.push_back(ball);
		}
		else {
			delete ball;
		}
	}
	_balls = newBalls;
	
	//Checa se o jogo acabou
	bool isGameOver;
	if (_crabs[0] != NULL) {
		for(int i = 0; i < 4; i++) {
			if (_crabs[i] != NULL) {
				isGameOver = false;
			}
		}
	}
	else {
		isGameOver = true;
	}
	
	if (!isGameOver) {
		while (_balls.size() < (unsigned int)NUM_BALLS) {
			//Coloca a bola no centro da arena
			bool ballFits = true;
			for(unsigned int i = 0; i < _balls.size(); i++) {
				Ball* ball = _balls[i];
				if (intersectsCircle(ball->getXAxis() - 0.5f, ball->getZAxis() - 0.5f,
									 2 * BALL_RADIUS)) {
					ballFits = false;
					break;
				}
			}
			
			if (ballFits) {
				Ball* ball =
					new Ball(BALL_RADIUS, 0.5f, 0.5f, 2 * PI * randomFloat());
				_balls.push_back(ball);
			}
			else {
				break;
			}
		}
	}
	else {
		for(unsigned int i = 0; i < _balls.size(); i++) {
			_balls[i]->fadeOut();
		}
	}
	
	//Roda a IA
	IA();
}

void Game::setPlayerCrabDir(int dir) {
	if (_crabs[0] != NULL) {
		_crabs[0]->setDirection(dir);
	}
}

int Game::score(int crabNum) {
	return scores[crabNum];
}

void Game::advance(float dt) {
	while (dt > 0) {
		if (_timeUntilNextStep < dt) {
			dt -= _timeUntilNextStep;
			step();
			_timeUntilNextStep = CRAB_STEP_TIME;
		}
		else {
			_timeUntilNextStep -= dt;
			dt = 0;
		}
	}
}

Crab** Game::crabs() {
	return _crabs;
}

vector<Ball*> Game::balls() {
	return _balls;
}