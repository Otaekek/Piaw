#include "piawMusic.class.hpp"

sf::Music  piawMusic::music;

void piawMusic::init() {
	if (!music.openFromFile("assets/sound/piaw/music/F-777 - Space Battle.wav")) {
		printf("Error in %s loading media: %s \n", __FILE__, "assets/sound/piaw/music/F-777 - Space Battle.wav");
	}
	music.play();
}

void piawMusic::update() {

}

void piawMusic::shutdown() {
	
}
