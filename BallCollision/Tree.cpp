#include "Tree.h"

Tree::Tree(const sf::FloatRect &bounds, size_t level, bool isHorizontal)
	: m_bounds{ bounds }, m_level{ level }, m_isHorizontal{ isHorizontal }
{
}

void Tree::subdivide()
{
	if (m_firstNode.get() != nullptr || m_secondNode.get() != nullptr)
		return;

	sf::FloatRect firstRect, secondRect;

	// divide the rectangle in half
	if (m_isHorizontal)
	{
		auto halfOfWidth = m_bounds.width / 2.0f;
		firstRect = sf::FloatRect{ m_bounds.left, m_bounds.top, halfOfWidth, m_bounds.height };                // left
		secondRect = sf::FloatRect{ m_bounds.left + halfOfWidth, m_bounds.top, halfOfWidth, m_bounds.height }; // right
	}
	else
	{
		auto halfOfHeight = m_bounds.height / 2.0f;
		firstRect = sf::FloatRect{ m_bounds.left, m_bounds.top, m_bounds.width, halfOfHeight };                 // up
		secondRect = sf::FloatRect{ m_bounds.left, m_bounds.top + halfOfHeight, m_bounds.width, halfOfHeight }; // down
	}

	m_firstNode = std::make_unique<Tree>(firstRect, m_level + 1, !m_isHorizontal);
	m_secondNode = std::make_unique<Tree>(secondRect, m_level + 1, !m_isHorizontal);
}

void Tree::insert(Ball& ball)
{
	auto firstNode = m_firstNode.get();
	auto secondNode = m_secondNode.get();

	// if node has a child write to child 
	if (firstNode != nullptr && secondNode != nullptr)
	{
		sf::FloatRect objectBounds = ball.getGlobalBounds();

		if (firstNode->getBounds().intersects(objectBounds))
			firstNode->insert(ball);

		if (secondNode->getBounds().intersects(objectBounds))
			secondNode->insert(ball);

		return;
	}

	m_children.push_back(ball);

	// if node has max count of objects divede it
	if (m_children.size() >= maxObjects)
	{
		subdivide();

		for (auto &it : m_children)
			insert(it);

		m_children.clear();
		m_children.shrink_to_fit();
	}
}

void Tree::clear()
{
	m_children.clear();
	m_firstNode = nullptr;
	m_secondNode = nullptr;
}

const sf::FloatRect& Tree::getBounds() const
{
	return m_bounds;
}

const std::vector<std::reference_wrapper<Ball>> &Tree::getNearbyBalls(const sf::FloatRect& bounds) const
{
	auto firstNode = m_firstNode.get();
	if (firstNode && bounds.intersects(firstNode->getBounds()))
		return firstNode->getNearbyBalls(bounds);

	auto secondNode = m_secondNode.get();
	if (secondNode && bounds.intersects(secondNode->getBounds()))
		return secondNode->getNearbyBalls(bounds);

	return m_children;
}
