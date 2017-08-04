
#include "soundBuiltin.class.hpp"


void soundBuiltin::init() {

}

void soundBuiltin::shutdown() {

}

void soundBuiltin::loadSound(void *data) {

	t_loadHeader	*loadheader;
	t_sound			*sound;

	loadheader = (t_loadHeader*)data;
	staticMemoryManager::alloc_asset(loadheader->ref, sizeof(t_sound));
	sound = (t_sound*)staticMemoryManager::get_data_ptr(loadheader->ref);
	sound->sound = new sf::Sound;
	sound->soundBuffer = new sf::SoundBuffer;
	sound->soundBuffer->loadFromFile(loadheader->path);
	sound->sound->setBuffer(*(sound->soundBuffer));
	staticMemoryManager::set_asset_state(staticMemoryManager::E_LOADED, loadheader->ref);
}

void soundBuiltin::closeSound(void *data) {

}
