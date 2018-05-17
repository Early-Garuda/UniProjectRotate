#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <math.h>


using namespace sf;

class Bullet
{
public:
	CircleShape shape;
	Vector2f currentVelocity;
	float maxSpeed;

	Bullet(float radius = 5.f);
	~Bullet();


};
