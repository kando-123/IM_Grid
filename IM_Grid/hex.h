#pragma once
#include <cassert>
#include <SFML/Graphics.hpp>

float inner_radius(float radius);
float hex_height(float radius);
float hex_width(float radius);

sf::Vector2f real_size(const sf::Vector2i& hexes_count, float hex_side);

int parity(int x);
sf::Vector2i adjacent(int x, int y, int dir);

enum class FieldType
{
	Undefined,
	Water, Land, Mounts
};

struct Hex
{
	FieldType type;
	sf::CircleShape shape;
	// sf::Texture texture;
	// std::unique_ptr<Building> building_ptr;
	// int player_index;
};
