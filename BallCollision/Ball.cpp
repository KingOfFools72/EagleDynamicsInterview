#include "Ball.h"

void Ball::setSpeed(float speed)
{
	m_speed = speed;
}

float Ball::getSpeed() const
{
	return m_speed;
}

void Ball::setDirection(const sf::Vector2f& direction)
{
	m_direction = direction;
}

const sf::Vector2f &Ball::getDirection() const
{
	return m_direction;
}

void Ball::move(float deltaTime)
{
	auto delta = getDirection() * (getSpeed() * deltaTime);
	setPosition(getPosition() + delta);
}

float Ball::sqrDistance(const Ball& ball)
{
	//auto offset = getRadius() - ball.getRadius();
	auto delta = getPosition() - ball.getPosition();// +sf::Vector2f{ offset, offset };

	return std::pow(delta.x, 2.0f) + std::pow(delta.y, 2.0f);
}

bool Ball::hasCollide(const Ball& ball)
{
	auto sqrDist = sqrDistance(ball);
	auto radiusSum = getRadius() + ball.getRadius();

	return sqrDist < std::pow(radiusSum, 2.0f);
}

void Ball::pushAside(Ball& ball)
{
	auto dist = std::sqrt(sqrDistance(ball));
	auto radiusSum = getRadius() + ball.getRadius();
	auto occurLength = radiusSum - dist;
	sf::Vector2f normalVect = getPosition() - ball.getPosition();
	sf::Vector2f pushVect = normalVect * (occurLength / dist);

	setPosition(getPosition() + pushVect);
	ball.setPosition(ball.getPosition() - pushVect);
}

void Ball::onCollide(Ball& ball)
{
	auto m1 = std::pow(getRadius(), 2.0f);
	auto v1 = getDirection();
	auto m2 = std::pow(ball.getRadius(), 2.0f);
	auto v2 = ball.getDirection();

	// m1v1 + m2v2 = m1u1 + m2u2
	// m1v1^2 + m2v2^2 = m1u1^2 + m2u2^2
	auto u1 = (m1 * v1 - m2 * v1 + 2 * m2 * v2) / (m1 + m2);
	auto u2 = (m1 * v1 + m2 * v2 - m1 * u1) / m2;

	setDirection(u1);
	ball.setDirection(u2);

	pushAside(ball);
}
