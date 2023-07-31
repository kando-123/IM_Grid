#include "hex.h"

float inner_radius(float radius)
{
	return .5f * std::sqrt(3.f) * radius;
}

float hex_height(float radius)
{
	return std::sqrt(3.f) * radius;
}

float hex_width(float radius)
{
	return 2.f * radius;
}

// Calculates the real dimensions of a hex grid.
// @param hexes_count: counts of hexes, horizontally and vertically
// @param hex_side: the side of a single hex
sf::Vector2f real_size(const sf::Vector2i& hexes_count, float hex_side)
{
	sf::Vector2f size;
	size.x = static_cast<float>(hexes_count.x / 2) * 3.0f * hex_side;
	size.x += (parity(hexes_count.x) == 0 ? 0.5f : 2.0f) * hex_side;
	size.y = hexes_count.y * hex_height(hex_side);
	if (hexes_count.x > 1)
		size.y += 0.5f * hex_height(hex_side);
	return size;
}

// 0 -> even, 1 -> odd
int parity(int x)
{
	return x & 1;
}

// dir = 0 -> 12 o'clock (up)
// dir = 1 -> 2 o'clock (right up)
// dir = 2 -> 4 o'clock (right down)
// dir = 3 -> 6 o'clock (down)
// dir = 4 -> 8 o'clock (left down)
// dir = 5 -> 10 o'clock (left up)
sf::Vector2i adjacent(int x, int y, int dir)
{
	assert(0 <= dir and dir <= 5);
	switch (dir)
	{
	case 0: return sf::Vector2i(x, y - 1);
	case 1: return sf::Vector2i(x + 1, y + parity(x) - 1);
	case 2: return sf::Vector2i(x + 1, y + parity(x));
	case 3: return sf::Vector2i(x, y + 1);
	case 4: return sf::Vector2i(x - 1, y + parity(x));
	case 5: return sf::Vector2i(x - 1, y + parity(x) - 1);
	}
}