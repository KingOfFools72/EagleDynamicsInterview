#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics/Rect.hpp>

#include "Ball.h"

class Tree
{
public:
	Tree(const sf::FloatRect &bounds, size_t level = 0, bool isHorizontal = false);

	void subdivide();
	void insert(Ball& ball);
	void clear();

	const sf::FloatRect& getBounds() const;
	const std::vector<std::reference_wrapper<Ball>>& getNearbyBalls(const sf::FloatRect &bounds) const;

private:
	std::vector<std::reference_wrapper<Ball>> m_children;

	std::unique_ptr<Tree> m_firstNode, m_secondNode;
	const bool m_isHorizontal; // true - horizontal
	const size_t m_level;

	sf::FloatRect m_bounds;
	
	const size_t maxObjects = 10;
};

