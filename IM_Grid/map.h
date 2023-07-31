#pragma once
#include "hex.h"
#include <memory>
#include <vector>

const float c_hex_side = 30.0f;
const float c_chunk_side = 6.0f * c_hex_side;
const int c_number_of_octaves = 3;

class Map
{
private:

	sf::Vector2i m_size;
	float m_hex_side = c_hex_side;
	std::vector<std::vector<Hex>> m_grid;

	sf::Vector2f m_window_size, m_shift, m_scale = sf::Vector2f(1.0f, 1.0f);

	inline void makeGrid(const sf::Vector2i& size);
	inline void generateTerrain(float water_percentage);

public:
	
	Map(const sf::Vector2f& window_size);

	void createMap(const sf::Vector2i& size, float water_percentage);

	void drawStuff(sf::RenderWindow& window);
	void shiftAll(const sf::Vector2f& offset);
};