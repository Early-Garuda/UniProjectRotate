#include "Bullet.h"



Bullet::Bullet(float radius)
	: currentVelocity(0.f, 0.f), maxSpeed(15.f)
{
	this->shape.setRadius(radius);
	this->shape.setFillColor(Color::Red);
}


Bullet::~Bullet()
{
}

