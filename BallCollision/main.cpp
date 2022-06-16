#include "SFML/Graphics.hpp"
#include "MiddleAverageFilter.h"
#include "Tree.h"
#include "Ball.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

constexpr int WINDOW_X = 1024;
constexpr int WINDOW_Y = 768;
constexpr int MAX_BALLS = 300;
constexpr int MIN_BALLS = 100;

Math::MiddleAverageFilter<float,100> fpscounter;

void draw_ball(sf::RenderWindow& window, const Ball& ball)
{
    window.draw(ball);
}

void draw_fps(sf::RenderWindow& window, float fps)
{
    char c[32];
    snprintf(c, 32, "FPS: %f", fps);
    std::string string(c);
    sf::String str(c);
    window.setTitle(str);
}

void verticalBorderCollision(Ball& ball, const int rightBorder)
{
	ball.setDirection({ -ball.getDirection().x, ball.getDirection().y });
	
	if (ball.getPosition().x < 0)
	{
		ball.setPosition( 0, ball.getPosition().y );
	}
	else
	{
		auto entire = ball.getPosition().x + ball.getRadius() * 2 - rightBorder;
		ball.setPosition(ball.getPosition().x - entire, ball.getPosition().y);
	}
}

void horizontalBorderCollision(Ball& ball, const int downBorder)
{
	ball.setDirection({ ball.getDirection().x, -ball.getDirection().y });

	if (ball.getPosition().y < 0)
	{
		ball.setPosition(ball.getPosition().x, 0);
	}
	else
	{
		auto entire = ball.getPosition().y + ball.getRadius() * 2 - downBorder;
		ball.setPosition(ball.getPosition().x, ball.getPosition().y - entire);
	}
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "ball collision demo");
    srand(time(NULL));

    std::vector<Ball> balls;

    // randomly initialize balls
    for (int i = 0; i < (rand() % (MAX_BALLS - MIN_BALLS) + MIN_BALLS); i++)
    {
		Ball newBall;
		newBall.setRadius(5 + rand() % 5);
		newBall.setSpeed(30 + rand() % 30);
		// position is upper left corner of the circumscribing square
		newBall.setPosition(rand() % WINDOW_X, rand() % WINDOW_Y);
		newBall.setDirection({ (-5 + (rand() % 10)) / 3.0f,
								(-5 + (rand() % 10)) / 3.0f });
        balls.push_back(newBall);
    }

   // window.setFramerateLimit(60);

    sf::Clock clock;
    float lastime = clock.restart().asSeconds();

	Tree tree({ 0, 0, static_cast<float>(WINDOW_X), static_cast<float>(WINDOW_Y) });

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        float current_time = clock.getElapsedTime().asSeconds();
        float deltaTime = current_time - lastime;
        fpscounter.push(1.0f / (current_time - lastime));
        lastime = current_time;

        /// <summary>
        /// TODO: PLACE COLLISION CODE HERE 
        /// объекты создаются в случайном месте на плоскости со случайным вектором скорости, имеют радиус R
        /// Объекты движутся кинетически. Пространство ограниченно границами окна
        /// Напишите обработчик столкновений шаров между собой и краями окна. Как это сделать эффективно?
        /// Массы пропорцианальны площадям кругов, описывающих объекты 
        /// Как можно было-бы улучшить текущую архитектуру кода?

		tree.clear();
		for (auto &it : balls)
			tree.insert(it);

		for (auto &ball : balls)
		{
			auto &nearbyObjects = tree.getNearbyBalls(ball.getGlobalBounds());

			// Check ball with ball collision
			auto collide = std::find_if(nearbyObjects.begin(), nearbyObjects.end(),
				[&ball](const auto &it)
			{
				auto &rBall = it.get();
				if (&rBall <= &ball) // look for balls only among the right ones in list, since the left ones have already collide
					return false;

				return ball.hasCollide(rBall);
			});

			if (collide != nearbyObjects.end())
				ball.onCollide(collide->get());

			// Check ball with wall collision
			if (ball.getPosition().x < 0 || ball.getPosition().x + ball.getRadius() * 2 > WINDOW_X)
				verticalBorderCollision(ball, WINDOW_X);

			if (ball.getPosition().y < 0 || ball.getPosition().y + ball.getRadius() * 2 > WINDOW_Y)
				horizontalBorderCollision(ball, WINDOW_Y);
		}

		for (auto &ball : balls)
			ball.move(deltaTime);

        window.clear();
        for (const auto ball : balls)
        {
            draw_ball(window, ball);
        }

		//draw_fps(window, fpscounter.getAverage());
		window.display();
    }
    return 0;
}
