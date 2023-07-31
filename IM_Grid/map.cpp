#include "map.h"
#include "hex.h"
#include "perlin-noiser.h"
#include <map>

#include <iostream>

const sf::Color c_land_color = sf::Color(0x00, 0xC0, 0x00);
const sf::Color c_water_color = sf::Color(0x00, 0x20, 0xB0);

void Map::makeGrid(const sf::Vector2i& size)
{
	assert (size.x > 0 && size.y > 0);

	m_grid.clear();
	m_size = size;
	m_grid.reserve(size.x);

	sf::Vector2f position;
	float half_height = inner_radius(m_hex_side);
	sf::Vector2f shift(1.5f * m_hex_side, hex_height(m_hex_side));

	Hex pattern;
	pattern.shape.setPointCount(6);
	pattern.shape.setRadius(m_hex_side);
	pattern.shape.setOrigin(m_hex_side, m_hex_side);
	pattern.shape.setRotation(30.0f);
	pattern.shape.setFillColor(sf::Color::White);
	pattern.shape.setOutlineColor(sf::Color::Black);
	pattern.shape.setOutlineThickness(-2.0f);
	pattern.type = FieldType::Undefined;

	for (int i = 0; i < size.x; ++i)
	{
		if (parity(i) == 1)
			position.y = half_height;
		else
			position.y = 0.0f;
		m_grid.emplace_back(size.y, pattern);
		for (int j = 0; j < size.y; ++j)
		{
			m_grid[i][j].shape.setPosition(position);
			position.y += shift.y;
		}
		position.x += shift.x;
	}

	m_shift = 0.5f * m_window_size - 0.5f * real_size(m_size, m_hex_side);
	m_shift += sf::Vector2f(c_hex_side, inner_radius(c_hex_side));
}

void Map::generateTerrain(float water_percentage)
{
	assert(water_percentage >= 0.0f && water_percentage < 1.0f);
	sf::Vector2f dimensions = real_size(m_size, m_hex_side);

	if (water_percentage == 0.0f)
	{
		for (int i = 0; i < m_size.x; ++i)
		{
			for (int j = 0; j < m_size.y; ++j)
			{
				m_grid[i][j].type = FieldType::Land;
				m_grid[i][j].shape.setFillColor(c_land_color);
			}
		}
		return;
	}
	
	PerlinNoiser generator(real_size(m_size, m_hex_side), c_chunk_side);
	generator.setOctavesCount(c_number_of_octaves);
	generator.setValueRange(0.0f, 100.0f);
	auto my_smoothstep = [](float t) -> float
	{
		return (0.03 - 0.0002 * t) * t * t;
	};
	generator.setSmoothstep(my_smoothstep);

	sf::Vector2f half_hex(0.5f * hex_width(m_hex_side), 0.5f * hex_height(m_hex_side));
	std::vector<std::vector<sf::Vector2f>> coords;
	coords.reserve(m_size.x);
	for (int i = 0; i < m_size.x; ++i)
	{
		coords.emplace_back(m_size.y);
		for (int j = 0; j < m_size.y; ++j)
			coords[i][j] = m_grid[i][j].shape.getPosition() + half_hex;
	}

	std::vector<std::vector<float>> results;
	generator.generateNoise(coords, results);
	coords.clear();
	std::map<int, int> counts;
	for (int i = 0; i < m_size.x; ++i)
	{
		for (int j = 0; j < m_size.y; ++j)
		{
			int index = static_cast<int>(std::floor(results[i][j]));
			++counts[index];
		}
	}

	// Is this mechanism OK?
	int water_surface = static_cast<int>(water_percentage * static_cast<float>(m_size.x * m_size.y));
	int sum = 0;
	std::map<int, int>::iterator it = counts.begin();
	while(it != counts.end())
	{
		sum += it->second;
		if (sum > water_surface)
			break;
		++it;
	}
	if (it->first == 100)
		--it;
	float limit = static_cast<float>(it->first + 1);
	for (int i = 0; i < m_size.x; ++i)
	{
		for (int j = 0; j < m_size.y; ++j)
		{
			if (results[i][j] < limit)
			{
				m_grid[i][j].type = FieldType::Water;
				m_grid[i][j].shape.setFillColor(c_water_color);
			}
			else
			{
				m_grid[i][j].type = FieldType::Land;
				m_grid[i][j].shape.setFillColor(c_land_color);
			}
		}
	}
}

Map::Map(const sf::Vector2f& window_size)
{
	m_window_size = window_size;
}

void Map::createMap(const sf::Vector2i& size, float land_part)
{
	makeGrid(size);
	

	generateTerrain(land_part);
}

void Map::drawStuff(sf::RenderWindow& window)
{
	for (int i = 0; i < m_grid.size(); ++i)
	{
		for (int j = 0; j < m_grid[i].size(); ++j)
		{
			sf::CircleShape copy = m_grid[i][j].shape;
			copy.move(m_shift);
			window.draw(copy);
		}
	}
}

void Map::shiftAll(const sf::Vector2f& offset)
{
	m_shift += offset;
}

