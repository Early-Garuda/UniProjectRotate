#include "platform.h"

platform::platform()
{
	/*platRect.setFillColor(Color::Blue);
	for (int i = 0; i < MAX_PLATFORMS; i++)
	{
	platPosition[i] = Vector2f(1 + i * 100, 200);
	platRect.setPosition(platPosition[i]);
	}*/

	platSize = Vector2f(50, 300);
	platRect.setOrigin(25, 150);
	platRect.setSize(platSize);
	platRect.setFillColor(Color::Blue);
	platRect.setPosition(Vector2f(950, 300 + 150));

	platSize = Vector2f(50, 300);
	platRectStatic.setOrigin(25, 150);
	platRectStatic.setSize(platSize);
	platRectStatic.setFillColor(Color::Red);
	platRectStatic.setPosition(Vector2f(950, 0 + 150));

	platSize = Vector2f(50, 300);
	platRectStatic2.setOrigin(25, 150);
	platRectStatic2.setSize(platSize);
	platRectStatic2.setFillColor(Color::Red);
	platRectStatic2.setPosition(Vector2f(950, 600 + 150));

}


platform::~platform()
{
}

RectangleShape platform::getRectangle()
{
	return platRect;
}

void platform::update(float dt)
{

	platRect.rotate(1);

}

void platform::draw(RenderWindow & window)
{
	/*for (int i = 0; i < MAX_PLATFORMS; i++)
	{
	platPosition[i] = Vector2f(1 + i * 50, 200);
	platRect.setPosition(platPosition[i]);
	window.draw(platRect);
	}*/
	window.draw(platRectStatic);
	window.draw(platRect);
	window.draw(platRectStatic2);
}
