#include "piawEnnemy.class.hpp"
#include "piawEnnemyCruiser.class.hpp"

piawEnnemy::piawEnnemy() : piawLineEntity(), weaponTimer {0.0f} {

}

piawEnnemy::~piawEnnemy() {

}

void piawEnnemy::speedUpdate() {
	rightD += rightSpeed / 18;
	upD += upSpeed / 18;

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

void piawEnnemySpawner::init() {

}

void piawEnnemySpawner::update() {
	if (!(rand() % 820)) {
		piawLineEntity *elem = new piawEnnemyCruiser(glm::vec3(
			piawMap::playerLinePos + 150 + ((float)rand() / RAND_MAX) * 150
			, (float)(rand() - RAND_MAX / 1.7) / (RAND_MAX / 1.3)
			, (float)(rand() - RAND_MAX / 1.7) / (RAND_MAX / 1.3)));
		piawLineEntityManager::push(elem);
	}
}

void piawEnnemySpawner::shutdown() {

}