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

	instructions.setFont(mFont);
	instructions.setPosition(50.0f, 250.0f);
	instructions.setCharacterSize(50);
	instructions.setFillColor(Color::White);
	instructions.setString("W,A,S,D to move.\n\nLeft Click to shoot.");
	//Text vars for DEAD screen
	endText.setFont(mFont);
	endText.setPosition(50.0f, 50.0f);
	endText.setCharacterSize(50);
	endText.setFillColor(Color::White);
	endText.setString("GAME OVER");

	youDead.setFont(mFont);
	youDead.setPosition(50.0f, 250.0f);
	youDead.setCharacterSize(50);
	youDead.setFillColor(Color::White);
	youDead.setString("You have died press Enter\n to return to main menu.");
	//In game text
	ssTime << "Time: " << gameSeconds;

	gameTimeText.setFont(mFont);
	gameTimeText.setPosition(25.0f, 25.0f);
	gameTimeText.setCharacterSize(25);
	gameTimeText.setFillColor(Color::Black);
	gameTimeText.setString(ssTime.str());


	//KillCount
	ssKCount << "Kills: " << killCount;

	killCounter.setFont(mFont);
	killCounter.setPosition(250.0f, 25.0f);
	killCounter.setCharacterSize(25);
	killCounter.setFillColor(Color::Black);
	killCounter.setString(ssKCount.str());

	//box
	box.setFillColor(Color::Black);
	box.setPosition(Vector2f(200, 200));
	//ball
	ball.setFillColor(Color::White);
	ball.setPosition(Vector2f(300, 200));
	//
	
	//setting frame limit
	window.setFramerateLimit(60);

	//Player
	player.setRadius(25.f);
	player.setFillColor(Color::White);
	killCount = 0;

	//Bullets
	bullets.push_back(Bullet(b1));

	//Enemy
	enemy.setFillColor(Color::Magenta);
	enemy.setSize(Vector2f(80.f, 80.f));

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


void Game::updateTime(float gameSeconds)
{
	gameTimeText.setString(std::to_string(gameSeconds));
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
		//time
		elapsedGameTime = gameClock.restart();
		gameSeconds = elapsedGameTime.asSeconds();
		std::cout << "Game Seconds: " << gameSeconds << std::endl;
		//Update Game Time
		ssTime.str(" ");
		ssTime << "Time: " << gameSeconds;
		gameTimeText.setString(ssTime.str());
		//get player center
		playerCenter = Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius());
		//get aim direction
		aimDir = mousePosWindow - playerCenter;
		//get aim normals
		aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));

		//Player
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			player.move(0.f, -6.5f);
		}
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			player.move(-6.5f, 0.f);
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			player.move(0.f, 6.5f);
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			player.move(6.5f, 0.f);
		}
		//Collision with window
		if (player.getPosition().x <= 0)//Left
		{
			player.setPosition(0.f, player.getPosition().y);
		}
		if (player.getPosition().x >= WIDTH - player.getGlobalBounds().width)//Right
		{
			player.setPosition(WIDTH - player.getGlobalBounds().width, player.getPosition().y);
		}
		if (player.getPosition().y <= 0)//Top
		{
			player.setPosition(player.getPosition().x, 0.f);
		}
		if (player.getPosition().y >= HEIGHT - player.getGlobalBounds().height)//Bottom
		{
			player.setPosition(player.getPosition().x, HEIGHT - player.getGlobalBounds().height);
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

			enemy.setPosition(WIDTH - enemy.getGlobalBounds().width, rand()%HEIGHT - enemy.getGlobalBounds().height);

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
						killCount++;
						//Updateing Score on screen
						ssKCount.str(" ");
						ssKCount << "Kills: " << killCount;
						killCounter.setString(ssKCount.str());
						break;
					}
				}
			}

		}

		//Enemy
		for (size_t i = 0; i < enemies.size(); i++)
		{
			//enemy movement
			enemies[i].move(-10.f, 0.f);

			if (enemies[i].getPosition().x <= 0 - enemies[i].getGlobalBounds().width)
			{
				enemies.erase(enemies.begin() + i);
			}
			//If enemy touches player game over
			if (enemies[i].getGlobalBounds().intersects(player.getGlobalBounds()))
			{
				elapsedGameTime = endTime;
				gameState = 3;
			}
		}
		
		break;

		case 3:
			
			if (Keyboard::isKeyPressed(Keyboard::Return))
			{
				gameState = 1;
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
		window.clear(Color::Green);
		window.draw(mText);
		window.draw(instructions);
		break;
	case 2://Level 1
		window.clear(Color::Green);

		
		//plat.draw(window);
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
		//Time score
		window.draw(gameTimeText);

		//Kill Count
		window.draw(killCounter);
		break;
	case 3://Died
		enemies.clear();
		window.clear(Color::Red);
		window.draw(endText);
		window.draw(youDead);
		break;

	}

	window.display();

}

Game::~Game()
{
}

