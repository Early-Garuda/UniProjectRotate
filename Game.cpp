#include "Game.h"

using namespace sf;

const Time Game::TimePerFrame = seconds(1.0f / 60.0f);
RectangleShape box(Vector2f(100.0f, 100.0f));
CircleShape ball(50);
//Constructor
Game::Game(RenderWindow & window, int width, int height)
{
	//Setting game state
	gameState = 1;
	//setting font for project
	if (!mFont.loadFromFile("CENTURY.TTF"))
	{
		return;
	}
	//Text vars for main menu
	mText.setFont(mFont);
	mText.setPosition(50.0f, 50.0f);
	mText.setCharacterSize(50);
	mText.setFillColor(Color::White);
	mText.setString("Space to play");
	//box
	box.setFillColor(Color::Black);
	box.setPosition(Vector2f(200, 200));
	//ball
	ball.setFillColor(Color::White);
	ball.setPosition(Vector2f(300, 200));
	//
	speed = 5;
	//setting frame limit
	window.setFramerateLimit(60);

	//Player
	player.setRadius(25.f);
	player.setFillColor(Color::White);

	//Bullets
	bullets.push_back(Bullet(b1));

	//Enemy
	enemy.setFillColor(Color::Magenta);
	enemy.setSize(Vector2f(50.f, 50.f));

	srand(time(NULL));

}

void Game::run(RenderWindow &window)
{
	Clock clock;
	Time timeSinceLastUpdate = Time::Zero;//Reset Time

	while (window.isOpen())
	{
		Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents(window);
			update(TimePerFrame);
		}
		render(window);
	}
}


//Listen for window events
void Game::processEvents(RenderWindow &window)
{
	Event event;//Enable Closing Window
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case Event::Closed:
			window.close();
			break;
		default:
			break;
		}
	}

	mousePosWindow = Vector2f(Mouse::getPosition(window));
}
//Listen for input and move objects
void Game::update(Time elapsedTime)
{

	switch (gameState)
	{
	case 1://Menu checking for input
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			gameState = 2;
		}
		break;
	case 2://Level 1 checking for input
		//get player center
		playerCenter = Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius());
		//get aim direction
		aimDir = mousePosWindow - playerCenter;
		//get aim normals
		aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));

		//playermovement
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			player.move(-speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			player.move(speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			player.move(0.f, -speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			player.move(0.f, speed);
		}


		//if (ball.getGlobalBounds().intersects(box.getGlobalBounds()))
		//{
		//	ball.setPosition(600, 200);
		//}

		//player collision with platforms
		if (player.getGlobalBounds().intersects(plat.getRectangle().getGlobalBounds()))
		{
			isColliding = true;
		}
		else
		{
			isColliding = false;
		}

		//Colliding

		if (Keyboard::isKeyPressed(Keyboard::A) && !isColliding)
		{
			box.move(-speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::D) && !isColliding)
		{
			box.move(speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::D) && isColliding)
		{
			box.move(0, 0);
		}

		if (Keyboard::isKeyPressed(Keyboard::W) && !isColliding)
		{
			box.move(0, -speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::S) && !isColliding)
		{
			box.move(0, speed);
		}


		//platform rotation
		if (Keyboard::isKeyPressed(Keyboard::U))
		{
			plat.update(elapsedTime.asSeconds());
		}

		//Enemy spawn counter
		if (spawnCounter < 20)
		{
			spawnCounter++;
		}
		//Randomly spawns enemies
		if (spawnCounter >= 20 && enemies.size() < 50)
		{
			enemy.setPosition(Vector2f(rand() % WIDTH, rand() % HEIGHT));
			enemies.push_back(RectangleShape(enemy));

			spawnCounter = 0;
		}
		//Shooting
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			b1.shape.setPosition(playerCenter);
			b1.currentVelocity = aimDirNorm * b1.maxSpeed;

			bullets.push_back(Bullet(b1));
		}

		for (size_t i = 0; i < bullets.size(); i++)
		{
			bullets[i].shape.move(bullets[i].currentVelocity);
			//Checking if bullets are in the screen
			if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > WIDTH || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > HEIGHT)
			{
				bullets.erase(bullets.begin() + i);
			}
			else
			{
				//enemy collision for bullet
				for (size_t k = 0; k < enemies.size(); k++)
				{
					if (bullets[i].shape.getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
					{
						bullets.erase(bullets.begin() + i);
						enemies.erase(enemies.begin() + k);
						break;
					}
				}
			}

		}

		break;

		//p1.update(0);

	default:
		/*
		{Speed.x = 0 Speed.y = 0}

		if D pressed {speed.x = 5}
		if A pressed {speed.x = -5}
		if W pressed {speed.y = 5}
		if S pressed {speed.y = -5}

		if speed.x < 0 -> going left
		if speed.x > 0 -> going right
		if speed.y < 0 -> going up
		if speed.y > 0 -> going down



		box.move(Speed.x, Speed.y);

		*/
		break;
	}
}
//Draw to screen
void Game::render(RenderWindow &window)
{
	switch (gameState)
	{
	case 1://Menu 
		window.clear(sf::Color::Green);
		window.draw(mText);
		break;
	case 2://Level 1
		window.clear(sf::Color::Green);
		
		plat.draw(window);
		//Draw enemies
		for (size_t i = 0; i < enemies.size(); i++)
		{
			window.draw(enemies[i]);
		}
		//draw player
		window.draw(player);

		//bullets
		for (size_t i = 0; i < bullets.size(); i++)
		{
			window.draw(bullets[i].shape);
		}
		break;
	//case 3://level 2

	//	break;

	}
	window.display();
}

Game::~Game()
{
}

