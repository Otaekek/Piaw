#include "piawEnnemy.class.hpp"
#include "piawEnnemyCruiser.class.hpp"

piawEnnemy::piawEnnemy() : piawLineEntity(), weaponTimer {0.0f} {
	isDying = false;
}

piawEnnemy::~piawEnnemy() {
}

void piawEnnemy::speedUpdate() {
	rightD += rightSpeed / 18;
	upD += upSpeed / 18;

	if (isDying)
		return ;
	if (rightD > 0.6f) {
		rightD = 0.6f;
		rightSpeed = 0;
	}

	if (rightD < -0.6f) {
		rightD = -0.6f;
		rightSpeed = 0;
	}

	if (upD > 0.6f) {
		upD = 0.6f;
		upSpeed = 0;
	}

	if (upD < -0.6f) {
		upD = -0.6f;
		upSpeed = 0;
	}

	rightSpeed /= 1.01f;
	upSpeed /= 1.01f;
}

void piawEnnemy::getHitBy(piawLineEntity *b) {
	if (isDying)
		return ;
	life--;
	if (life == 0) {
		isDying = true;
		((t_sound*)staticMemoryManager::get_data_ptr(fileLoader::load_fs_asset_sync("assets/sound/piaw/soundEffect/boom.wav", E_SOUND)))->sound->play();
	}
}

void piawEnnemySpawner::init() {

}

void piawEnnemySpawner::update() {
	if (!(rand() % 120)) {
		piawLineEntity *elem = new piawEnnemyCruiser(glm::vec3(
			piawMap::playerLinePos + 150 + ((float)rand() / RAND_MAX) * 150
			, (float)(rand() - RAND_MAX / 1.7) / (RAND_MAX / 1.3)
			, (float)(rand() - RAND_MAX / 1.7) / (RAND_MAX / 1.3)));
		piawLineEntityManager::push(elem);
	}
}

void piawEnnemySpawner::shutdown() {

}