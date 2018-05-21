#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "platform.h"
#include "Bullet.h"
#include <math.h>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>

#define WIDTH 1000
#define HEIGHT 900

using namespace sf;

class Game
{
public:

	Game(RenderWindow &window, int width, int height);
	~Game();
	//platform
	platform plat;
	void run(RenderWindow &window);
	//Font
	Font mFont;
	//Text for Main menu.
	Text mText;
	Text instructions;
	//Text for end screen
	Text endText;
	Text youDead;


	//clock
	Clock gameClock;
	Time elapsedGameTime;
	float deltaTime = elapsedGameTime.asSeconds();
	Time endTime;
	std::ostringstream ssTime;

	Text gameTimeText;

	

	void updateTime( float deltaTime );

	float time_since_play;

	int gameState;

	//Player
	int killCount = 0;
	Text killCounter;
	std::ostringstream ssKCount;
	//PlayerVectors
	Vector2f playerCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;
	//Bullet 1
	Bullet b1;
	//Enemy
	float enemySpeed = 10;

private:
	void processEvents(RenderWindow &window);
	void update(Time elapsedTime);
	void render(RenderWindow &window);

	static const Time TimePerFrame;

	//Player
	CircleShape player;
	//Enemy
	RectangleShape enemy;
	int spawnCounter = 20;
	
	std::vector<RectangleShape> enemies;
	//bullets
	std::vector<Bullet> bullets;
};