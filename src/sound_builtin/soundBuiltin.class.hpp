#pragma once

#include <staticMemoryManager.class.hpp>
#include <renderDataSys.class.hpp>
#include <dynamicMemoryManager.class.hpp>
#include <SFML/Audio.hpp>

typedef struct		s_sound {
	sf::SoundBuffer *soundBuffer;
	sf::Sound		*sound;
}					t_sound;

class soundBuiltin {

public:

	static void init();
	static void shutdown();

	static void loadSound(void *data);
	static void closeSound(void *data);
};
