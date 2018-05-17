#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
#define MAX_PLATFORMS 10

class platform
{
public:
	platform();
	~platform();

	RectangleShape getRectangle();

	void update(float dt);
	void draw(RenderWindow &window);

private:

	RectangleShape platRect;
	RectangleShape platRectStatic;
	RectangleShape platRectStatic2;


	//Vector2f platPosition [MAX_PLATFORMS];
	Vector2f platSize;
	float platRotation;
};

