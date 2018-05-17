#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <math.h>
#include <vector>
#include <cstdlib>
#include "platform.h"
#include "Bullet.h"

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
	int gameState;
	int speed;

	bool isColliding;

	bool isCollidingUp;
	//PlayerVectors
	Vector2f playerCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;
	//Bullet 1
	Bullet b1;

	

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