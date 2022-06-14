#include "SFML/Graphics.hpp"
#include "MiddleAverageFilter.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

constexpr int WINDOW_X = 1024;
constexpr int WINDOW_Y = 768;
constexpr int MAX_BALLS = 300;
constexpr int MIN_BALLS = 100;

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
    gball.setPosition(ball.p.x, ball.p.y);
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
