#pragma once

#include <SFML/Graphics.hpp>

class Ball : public sf::CircleShape
{
public:
	void move(float deltaTime);

	void setSpeed(float speed);
	float getSpeed() const;

	void setDirection(const sf::Vector2f &direction);
	const sf::Vector2f& getDirection() const;
	
	void onCollide(Ball& ball);
	bool hasCollide(const Ball& ball);

private:

	float sqrDistance(const Ball& ball);
	void pushAside(Ball& ball);

	sf::Vector2f m_direction;
	float m_speed = 0;
};

