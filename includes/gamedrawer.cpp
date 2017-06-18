#include <math.h>
#include <sstream>
#include <vector>
#include <string>

#include <GL/glut.h>

#include "../headers/gamedrawer.h"

using namespace std;
using namespace gamedraw;
using namespace gameinfo;

//Transforma a imagem em uma textura, retorna o ID referente
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

GameDrawer::GameDrawer() {
	game = NULL;
	playerCrabDir = 0;

	//Começa um novo jogo com score máximo de 0 (tela inicial)
	startNewGame(0, 0);

	waterTextureOffset = 0;

	//Carregando texturas de cada caranguejo, assim como seu modelo
	vector<const char*> textureFilenames;
	textureFilenames.push_back("textures/crab1.bmp");
	textureFilenames.push_back("textures/crab2.bmp");
	textureFilenames.push_back("textures/crab3.bmp");
	textureFilenames.push_back("textures/crab4.bmp");
	crabModel = MD2Model::load("textures/crab.md2", textureFilenames);

	//Barreiras e Barra
	setupBarriers();
	setupPole();

	//Carrega "arena"
	Image* image = loadBMP("textures/wood.bmp");
	woodTextureId = loadTexture(image);
	delete image;

	//Carrega a agua
	image = loadBMP("textures/water.bmp");
	waterTextureId = loadTexture(image);
	delete image;
}

GameDrawer::~GameDrawer() {
	delete game;
}

void GameDrawer::setGame(Game* game1) {
	if (game != NULL) {
		delete game;
	}

	game = game1;
	game->setPlayerCrabDir(playerCrabDir);
	_timeUntilNextStep = 0;

	isGameOver0 = (game->score(0) == 0);
	waitingForFirstGame = isGameOver0;

	for(int i = 0; i < 4; i++) {
		animTimes[i] = 0;

		if (!isGameOver0) {
			crabFadeAmounts[i] = 1;
		}
		else {
			crabFadeAmounts[i] = 0;
		}

		Crab* crab = game->crabs()[i];
		if (crab != NULL) {
			oldCrabPos[i] = game->crabs()[i]->getPosition();
		}
		else {
			oldCrabPos[i] = 0.5f;
		}
	}
}

void GameDrawer::setupBarriers() {
	Image* image = loadBMP("textures/vtr.bmp");
	GLuint textureId = loadTexture(image);
	delete image;

	GLuint barrierDisplayListId = glGenLists(1);
	glNewList(barrierDisplayListId, GL_COMPILE);

	//Desenha o topo da barreira
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLfloat materialColor[] = {1, 1, 1, 1};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);

	glNormal3f(0, 1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5f, 0.5f);
	glVertex3f(0, BARRIER_HEIGHT, 0);
	for(int i = NUM_BARRIER_POINTS; i >= 0; i--) {
		float angle = 2 * PI * (float)i / (float)NUM_BARRIER_POINTS;
		glTexCoord2f(-cos(angle) / 2 + 0.5f, sin(angle) / 2 + 0.5f);
		glVertex3f(BARRIER_SIZE * cos(angle),
				   BARRIER_HEIGHT,
				   BARRIER_SIZE * sin(angle));
	}
	glEnd();

	//Desenha a parte de baixo da barreira
	GLfloat materialColor2[] = {1.0f, 0.0f, 0.0f, 1};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor2);

	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 0.0f, 0.0f);
	glNormal3f(0, -1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for(int i = 0; i <= NUM_BARRIER_POINTS; i++) {
		float angle = 2 * PI * (float)i / (float)NUM_BARRIER_POINTS;
		glVertex3f(BARRIER_SIZE * cos(angle), 0, BARRIER_SIZE * sin(angle));
	}
	glEnd();

	//Desenha a area ao redor (parte cilindrica)
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i <= NUM_BARRIER_POINTS; i++) {
		float angle = 2 * PI * ((float)i - 0.5f) / (float)NUM_BARRIER_POINTS;
		glNormal3f(cos(angle), 0, sin(angle));
		float angle2 = 2 * PI * (float)i / (float)NUM_BARRIER_POINTS;
		glVertex3f(BARRIER_SIZE * cos(angle2), 0, BARRIER_SIZE * sin(angle2));
		glVertex3f(BARRIER_SIZE * cos(angle2),
				   BARRIER_HEIGHT,
				   BARRIER_SIZE * sin(angle2));
	}
	glEnd();
	glEndList();



	//Cria um displaylist com as copias das barreiras
	barriersDisplayListId = glGenLists(1);
	glNewList(barriersDisplayListId, GL_COMPILE);
	glDisable(GL_COLOR_MATERIAL);

	//Adiciona uma leve especularidade
	GLfloat materialSpecular[] = {1, 1, 1, 1};
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 15.0f);

	for(float z = 0; z < 2; z++) {
		for(float x = 0; x < 2; x++) {
			glPushMatrix();
			glTranslatef(x, 0, z);
			glCallList(barrierDisplayListId);
			glPopMatrix();
		}
	}
	glEnable(GL_COLOR_MATERIAL);



	glEndList();
}

void GameDrawer::setupPole() {
	poleDisplayListId = glGenLists(1);
	glNewList(poleDisplayListId, GL_COMPILE);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 0.8f, 0.0f);

	//Desenha o circulo à esquerda
	glNormal3f(-1, 0, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(BARRIER_SIZE, POLE_HEIGHT, -POLE_RADIUS);
	for(int i = NUM_POLE_POINTS; i >= 0; i--) {
		float angle = 2 * PI * (float)i / (float)NUM_POLE_POINTS;
		glVertex3f(BARRIER_SIZE,
				   POLE_HEIGHT + POLE_RADIUS * cos(angle),
				   POLE_RADIUS * (sin(angle) - 1));
	}
	glEnd();

	//Desenha o circulo à direita
	glNormal3f(1, 0, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(1 - BARRIER_SIZE, POLE_HEIGHT, -POLE_RADIUS);
	for(int i = 0; i <= NUM_POLE_POINTS; i++) {
		float angle = 2 * PI * (float)i / (float)NUM_POLE_POINTS;
		glVertex3f(1 - BARRIER_SIZE,
				   POLE_HEIGHT + POLE_RADIUS * cos(angle),
				   POLE_RADIUS * (sin(angle) - 1));
	}
	glEnd();

	//Desenha a parte cilindrica
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i <= NUM_POLE_POINTS; i++) {
		float angle = 2 * PI * ((float)i - 0.5f) / (float)NUM_POLE_POINTS;
		glNormal3f(0, cos(angle), sin(angle));
		float angle2 = 2 * PI * (float)i / (float)NUM_POLE_POINTS;
		glVertex3f(1 - BARRIER_SIZE,
				   POLE_HEIGHT + POLE_RADIUS * cos(angle2),
				   POLE_RADIUS * (sin(angle2) - 1));
		glVertex3f(BARRIER_SIZE,
				   POLE_HEIGHT + POLE_RADIUS * cos(angle2),
				   POLE_RADIUS * (sin(angle2) - 1));
	}
	glEnd();

	glEndList();
}

void GameDrawer::step() {
	//Avança o jogo
	game->advance(STEP_TIME);

	//Sensação da agua se movento
	waterTextureOffset += STEP_TIME / WATER_TEXTURE_TIME;
	while (waterTextureOffset > WATER_TEXTURE_SIZE) {
		waterTextureOffset -= WATER_TEXTURE_SIZE;
	}

	//Atualiza animações, se o caranguejo some ou se é gameover
	bool opponentAlive = false;
	for(int i = 0; i < 4; i++) {
		Crab* crab = game->crabs()[i];

		if (crab != NULL) {
			oldCrabPos[i] = crab->getPosition();
		}

		//Atualiza tempo de animação
		if (crab != NULL || crabFadeAmounts[i] > 0) {
			if (crab != NULL && crab->getDirection() != 0) {
				if (crab->getDirection() > 0) {
					animTimes[i] += STEP_TIME / WALK_ANIM_TIME;
				}
				else {
					animTimes[i] -= STEP_TIME / WALK_ANIM_TIME;
				}
			}
			else {
				animTimes[i] += STEP_TIME / STAND_ANIM_TIME;
			}

			while (animTimes[i] > 1) {
				animTimes[i] -= 1;
			}
			while (animTimes[i] < 0) {
				animTimes[i] += 1;
			}
		}

		//Atualiza caranguejo sumindo ao perder
		if (crab == NULL) {
			crabFadeAmounts[i] -= STEP_TIME / CRAB_FADE_OUT_TIME;
			if (crabFadeAmounts[i] < 0) {
				if (i == 0) {
					isGameOver0 = true;
				}
				crabFadeAmounts[i] = 0;
			}
			else if (i != 0) {
				opponentAlive = true;
			}
		}
		else if (i != 0) {
			opponentAlive = true;
		}
	}

	if (!opponentAlive) {
		isGameOver0 = true;
	}
}

void GameDrawer::setupLighting() {
	GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	//Insere uma luz em cada canto da arena
	int index = 0;
	for(float z = 0; z < 2; z += 1) {
		for(float x = 0; x < 2; x += 1) {
			glEnable(GL_LIGHT0 + index);
			GLfloat lightColor[] = {0.2f, 0.2f, 0.2f, 1};
			GLfloat lightPos[] = {x, 1.5f, z, 1};
			glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, lightColor);
			glLightfv(GL_LIGHT0 + index, GL_SPECULAR, lightColor);
			glLightfv(GL_LIGHT0 + index, GL_POSITION, lightPos);
			index++;
		}
	}
}

void GameDrawer::drawCrabsAndPoles() {
	if (crabModel != NULL) {
		glEnable(GL_NORMALIZE);
		for(int i = 0; i < 4; i++) {
			Crab* crab = game->crabs()[i];

			//Translada e rotaciona em cada lado da "arena"
			glPushMatrix();
			switch(i) {
				case 1:
					glTranslatef(0, 0, 1);
					glRotatef(90, 0, 1, 0);
					break;
				case 2:
					glTranslatef(1, 0, 1);
					glRotatef(180, 0, 1, 0);
					break;
				case 3:
					glTranslatef(1, 0, 0);
					glRotatef(270, 0, 1, 0);
					break;
			}

			if (crab != NULL || crabFadeAmounts[i] > 0) {
				//Desenha o caranguejo
				glPushMatrix();

				float crabPos;
				if (crab != NULL) {
					crabPos = crab->getPosition();
				}
				else {
					crabPos = oldCrabPos[i];
				}
				glTranslatef(crabPos, 0.055f, CRAB_OFFSET);
				if (crab == NULL) {
					//Usado no efeito de desaparecimento do caranguejo ao perder
					glTranslatef(0, -0.055f * (1 - crabFadeAmounts[i]), 0);
					glScalef(crabFadeAmounts[i],
							 crabFadeAmounts[i],
							 crabFadeAmounts[i]);
				}

				glRotatef(-90, 0, 1, 0);
				glRotatef(-90, 1, 0, 0);
				glScalef(0.05f, 0.05f, 0.05f);

				if (crab == NULL || crab->getDirection() == 0) {
					crabModel->setAnimation("stand");
				}
				else {
					crabModel->setAnimation("run");
				}

				glColor3f(1, 1, 1);
				crabModel->draw(i, animTimes[i]);
				glPopMatrix();
			}

			if (crab == NULL) {
				glCallList(poleDisplayListId);
			}

			glPopMatrix();
		}
	}
}

void GameDrawer::drawBarriers() {
	glCallList(barriersDisplayListId);
}

void GameDrawer::drawScores() {
	float d = 0.1f;
	for(int i = 0; i < 4; i++) {
		ostringstream oss;
		oss << game->score(i);
		string str = oss.str();

		glPushMatrix();
		int hAlign;
		int vAlign;
		switch(i) {
			case 0:
				glTranslatef(0.5f, 0, -d);
				hAlign = 0;
				vAlign = -1;
				break;
			case 1:
				glTranslatef(-d, 0, 0.5f);
				hAlign = -1;
				vAlign = 0;
				break;
			case 2:
				glTranslatef(0.5f, 0, 1 + d);
				hAlign = 0;
				vAlign = 1;
				break;
			case 3:
				glTranslatef(1 + d, 0, 0.5f);
				hAlign = 1;
				vAlign = 0;
				break;
		}

		glTranslatef(0, 0.04f, 0);
		glRotatef(90, 1, 0, 0);
		glRotatef(180, 0, 1, 0);
		glScalef(0.1f, 0.1f, 0.1f);
		t3dDraw3D(str, hAlign, vAlign, 0.3f);
		glPopMatrix();
	}
}

void GameDrawer::drawBalls() {
	glDisable(GL_NORMALIZE);


	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	vector<Ball*> balls = game->balls();
	for(unsigned int i = 0; i < balls.size(); i++) {
		Ball* ball = balls[i];

		if (ball->fadeAmount() < 1) {
			glEnable(GL_BLEND);
			glColor4f(0.75f, 0.75f, 0.75f, ball->fadeAmount());
		}
		else {
			glColor3f(0.75f, 0.75f, 0.75f);
		}

		glPushMatrix();
		glTranslatef(ball->getXAxis(), ball->getRadius() + 0.01f, ball->getZAxis());
		glutSolidSphere(ball->getRadius(), 10, 6);
		glPopMatrix();

		if (ball->fadeAmount() < 1) {
			glDisable(GL_BLEND);
		}
	}
}

void GameDrawer::drawWood() {
	//Altura da arena em relação à agua
	float height = 0.01f;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, woodTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glDisable(GL_NORMALIZE);
	glColor3f(1, 1, 1);
	glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(0, height, 0);
	glTexCoord2f(1, 1);
	glVertex3f(0, height, 1);
	glTexCoord2f(0, 1);
	glVertex3f(1, height, 1);
	glTexCoord2f(0, 0);
	glVertex3f(1, height, 0);
	glEnd();
}

void GameDrawer::drawWater() {
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, waterTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glDisable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glColor4f(1, 1, 1, WATER_ALPHA);
	glNormal3f(0, 1, 0);

	glBegin(GL_QUADS);
	glTexCoord2f(200 / WATER_TEXTURE_SIZE,
				 -waterTextureOffset / WATER_TEXTURE_SIZE);
	glVertex3f(-100, 0, -100);
	glTexCoord2f(200 / WATER_TEXTURE_SIZE,
				 (200 - waterTextureOffset) / WATER_TEXTURE_SIZE);
	glVertex3f(-100, 0, 100);
	glTexCoord2f(0, (200 - waterTextureOffset) / WATER_TEXTURE_SIZE);
	glVertex3f(100, 0, 100);
	glTexCoord2f(0, -waterTextureOffset / WATER_TEXTURE_SIZE);
	glVertex3f(100, 0, -100);
	glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
}

void GameDrawer::drawWinner() {
	if (!isGameOver0) {
		return;
	}

	glColor3f(0.15f, 1.0f, 0.2f);

	if (!waitingForFirstGame) {
		//Desenha se perdeu ou ganhou
		string str;
		if (game->score(0) > 0) {
			str = "Voce venceu :D";
		}
		else {
			str = "Voce perdeu :(";
		}

		glPushMatrix();
		glTranslatef(0.5f, 0.05f, 0.5f);
		glScalef(0.1f, 0.1f, 0.1f);
		glTranslatef(0, 0.15f, 0);
		glRotatef(180, 0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		t3dDraw3D(str, 0, 0, 0.3f);
		glPopMatrix();
	}

	//Instruções - Tela inicial
	glPushMatrix();
	glTranslatef(0.5f, 0.05f, 0.35f);
	glScalef(0.04f, 0.04f, 0.04f);
	glTranslatef(0, 0.15f, 0);
	glRotatef(180, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	t3dDraw3D("[ENTER] - Inicia o jogo\n[ESC] - Sair do jogo\n (Use os direcionais para mover-se)", 0, -1, 0.3f);
	glPopMatrix();
}

void GameDrawer::draw() {
	//Seta a cor do background pra um tipo de azul
	glClearColor(0.7f, 0.9f, 1.0f, 1);

	//Desenha a outra parte
	glCullFace(GL_BACK);
	setupLighting();
	drawWater();
	drawWood();
	drawWinner();
	drawCrabsAndPoles();
	drawBarriers();
	drawScores();
	drawBalls();
}

void GameDrawer::advance(float dt) {
	while (dt > 0) {
		if (_timeUntilNextStep < dt) {
			dt -= _timeUntilNextStep;
			step();
			_timeUntilNextStep = STEP_TIME;
		}
		else {
			_timeUntilNextStep -= dt;
			dt = 0;
		}
	}
}

void GameDrawer::setPlayerCrabDir(int dir) {
	playerCrabDir = dir;
	game->setPlayerCrabDir(dir);
}

bool GameDrawer::isGameOver() {
	return isGameOver0;
}

void GameDrawer::startNewGame(float maximumSpeedForOpponents,
							  int startingScore) {
	setGame(new Game(maximumSpeedForOpponents, startingScore));
}

void initGameDrawer() {
	t3dInit();
}

void cleanupGameDrawer() {
	t3dCleanup();
}