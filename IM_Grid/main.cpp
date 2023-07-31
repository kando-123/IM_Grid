#include "map.h"

#include <iostream>

// Tudu:
// > szum Perlina do generowania gór i lasów;
// > rozpoznawanie klikniêtego pola;
// > skalowanie.

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 620), "", sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Vector2i size( 15, 9 );
    Map map(static_cast<sf::Vector2f>(window.getSize()));
	map.createMap(size, 0.1f);

	sf::Vector2f window_size = static_cast<sf::Vector2f>(window.getSize());
	float thickness = 2.0f;
	sf::RectangleShape horizontal(sf::Vector2f(window_size.x, thickness));
	horizontal.setOrigin(0.0f, 0.5f * thickness);
	horizontal.setPosition(0.0f, window_size.y / 2.0f);
	horizontal.setFillColor(sf::Color::Red);
	sf::RectangleShape vertical(sf::Vector2f(thickness, window_size.y));
	vertical.setOrigin(0.5f * thickness, 0.0f);
	vertical.setPosition(window_size.x / 2.0f, 0.0f);
	vertical.setFillColor(sf::Color::Red);

	sf::Event e = sf::Event();
	while (window.isOpen())
	{
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (e.key.code)
				{
					case sf::Keyboard::Escape:
						window.close();
					break;
					case sf::Keyboard::Right:
						if (e.key.shift)
							map.shiftAll(-sf::Vector2f(+30.0f, 0.0f));
						else
							map.shiftAll(-sf::Vector2f(+15.0f, 0.0f));
						break;
					case sf::Keyboard::Left:
						if (e.key.shift)
							map.shiftAll(-sf::Vector2f(-30.0f, 0.0f));
						else
							map.shiftAll(-sf::Vector2f(-15.0f, 0.0f));
						break;
					case sf::Keyboard::Up:
						if (e.key.shift)
							map.shiftAll(-sf::Vector2f(0.0f, -30.0f));
						else
							map.shiftAll(-sf::Vector2f(0.0f, -15.0f));
						break;
					case sf::Keyboard::Down:
						if (e.key.shift)
							map.shiftAll(-sf::Vector2f(0.0f, +30.0f));
						else
							map.shiftAll(-sf::Vector2f(0.0f, +15.0f));
						break;
				}
			default:
				break;
			}
		}
		window.clear(sf::Color(0x40, 0x40, 0x40));
		map.drawStuff(window);

		window.draw(vertical);
		window.draw(horizontal);

		window.display();
	}

    return 0;
}