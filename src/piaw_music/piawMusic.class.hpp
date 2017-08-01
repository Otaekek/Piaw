#include <SFML/Audio.hpp>

#pragma once


class piawMusic {

public:

	static void init();
	static void update();
	static void shutdown();

	static sf::Music music;
};