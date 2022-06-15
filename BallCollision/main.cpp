#include "SFML/Graphics.hpp"
#include "MiddleAverageFilter.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

constexpr int WINDOW_X = 1024;
constexpr int WINDOW_Y = 768;
constexpr int MAX_BALLS = 100;
constexpr int MIN_BALLS = 20;

Math::MiddleAverageFilter<float,100> fpscounter;

struct Ball
{
    sf::Vector2f p;
    sf::Vector2f dir;
    float r = 0;
    float speed = 0;
};

void draw_ball(sf::RenderWindow& window, const Ball& ball)
{
    sf::CircleShape gball;
    gball.setRadius(ball.r);
    gball.setPosition(ball.p.x - ball.r, ball.p.y - ball.r);
    window.draw(gball);
}

void move_ball(Ball& ball, float deltaTime)
{
    float dx = ball.dir.x * ball.speed * deltaTime;
    float dy = ball.dir.y * ball.speed * deltaTime;
    ball.p.x += dx;
    ball.p.y += dy;
}

void draw_fps(sf::RenderWindow& window, float fps)
{
    char c[32];
    snprintf(c, 32, "FPS: %f", fps);
    std::string string(c);
    sf::String str(c);
    window.setTitle(str);
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
        newBall.p.x = rand() % WINDOW_X;
        newBall.p.y = rand() % WINDOW_Y;
        newBall.dir.x = (-5 + (rand() % 10)) / 3.;
        newBall.dir.y = (-5 + (rand() % 10)) / 3.;
        newBall.r = 5 + rand() % 5;
        newBall.speed = 30 + rand() % 30;
        balls.push_back(newBall);
    }

	Ball newBall;
	newBall.p.x = rand() % WINDOW_X;
	newBall.p.y = rand() % WINDOW_Y;
	newBall.speed = 100 + rand() % 30;
	newBall.dir.x = (-5 + (rand() % 10)) / 3.;
	newBall.dir.y = (-5 + (rand() % 10)) / 3.;
	newBall.r = 20 + rand() % 5;
	
	balls.push_back(newBall);

	//Ball newBall2;
	//newBall2.p.x = rand() % WINDOW_X;
	//newBall2.p.y = rand() % WINDOW_Y;
	//newBall2.speed = 100 + rand() % 30;
	//newBall2.dir.x = (-5 + (rand() % 10)) / 3.;
	//newBall2.dir.y = (-5 + (rand() % 10)) / 3.;
	//newBall2.r = 100 + rand() % 5;
	
	//balls.push_back(newBall2);

   // window.setFramerateLimit(60);

    sf::Clock clock;
    float lastime = clock.restart().asSeconds();

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
        /// Данный код является макетом, вы можете его модифицировать по своему усмотрению

		// Check collision with boards
		for (auto& ball : balls)
		{
			if (ball.p.y - ball.r <= 0 || ball.p.y + ball.r >= WINDOW_Y)
				ball.dir.y = -ball.dir.y;
			if (ball.p.x - ball.r <= 0 || ball.p.x + ball.r >= WINDOW_X)
				ball.dir.x = -ball.dir.x;
		}

		// Check collision with balls
		for (int i = 0; i < balls.size()-1; ++i)
		{
			for (int j = i + 1; j < balls.size(); ++j)
			{
				//// Distance between balls
				//sf::Vector2f normal = balls[i].p - balls[j].p;
				//float dist_sqr = normal.x * normal.x + normal.y * normal.y;

				//float radius = balls[i].r + balls[j].r;

				//// If no collision
				//if (dist_sqr >= radius * radius)
				//{
				//	continue;
				//}

				auto offset = balls[i].r - balls[j].r;
				auto delta = balls[i].p - balls[j].p + sf::Vector2f{ offset, offset };
				auto distance = std::sqrt(std::pow(delta.x, 2.0f) + std::pow(delta.y, 2.0f));
				auto radiusSum = balls[i].r + balls[j].r;

				if (distance >= radiusSum)
				{
					continue;
				}

				//auto dist = std::sqrt(dist_sqr);
				//auto entrance = radius - dist;
				//auto dt = 
				//balls[j].p += {normal.x + entrance, normal.y + entrance};


				// Count speed
				auto m1 = std::pow(balls[i].r, 2.0f);
				auto v1 = balls[i].dir;
				auto m2 = std::pow(balls[j].r, 2.0f);
				auto v2 = balls[j].dir;

				// m1v1 + m2v2 = m1u1 + m2u2
				// m1v1^2 + m2v2^2 = m1u1^2 + m2u2^2
				auto u1 = (m1 * v1 - m2 * v1 + 2 * m2 * v2) / (m1 + m2);
				auto u2 = (m1 * v1 + m2 * v2 - m1 * u1) / m2;

				balls[i].dir = u1;
				balls[j].dir = u2;
			}
		}

        for (auto& ball : balls)
        {
            move_ball(ball, deltaTime);
        }

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
