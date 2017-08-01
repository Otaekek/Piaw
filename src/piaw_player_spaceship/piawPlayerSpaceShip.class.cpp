#include "piawPlayerSpaceShip.class.hpp"
#include <particleSystem.class.hpp>

piawPlayerSpaceShip::piawPlayerSpaceShip(): missileTimer {0}, piawLineEntity() {
//	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/starcruiser military/Starcruiser military.obj", E_3D));
	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/starcruiser military/s.obj", E_3D));
	transformBuiltin::scale(transformHandler, 80.5, 80.5, 80.5);
	_camDist = 32.6;
	rightD = 0;
	upD = 0;
    missileSoundBuffer.loadFromFile("assets/sound/piaw/soundEffect/piou.wav");
	missileSound.setBuffer(missileSoundBuffer);
}


piawPlayerSpaceShip::piawPlayerSpaceShip(float camDist): missileTimer {0},  piawLineEntity() {
	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/starcruiser military/s.obj", E_3D));
	transformBuiltin::scale(transformHandler, 90.5, 90.5, 90.5);
	_camDist = camDist;
}

piawPlayerSpaceShip::~piawPlayerSpaceShip() {

}

void pupdate(float *d) {
	d[6] /= 1.7 * 1;
	d[7] /= 1.7 * 1;
	d[8] /= 2.8 * 1;
}

void piawPlayerSpaceShip::generate_particle(glm::vec3 dir, t_transform *t) {

	float pdata[9] = {100};

	for (int i = 0; i < timeBuiltin::delta_time() * 95000; i++) {
		pdata[0] = t->position.x + ((float)rand() / RAND_MAX) * 3450 + dir.x * 10 * (i + 2000);
		pdata[1] = t->position.y+ ((float)rand() / RAND_MAX) * 3450 + dir.y * 10 * (i + 2000);
		pdata[2] = t->position.z+ ((float)rand() / RAND_MAX) * 3450 + dir.z * 10 * (i + 2000);
		pdata[3] = 52.5;
		pdata[4] = 52.5;
		pdata[5] = 52.5;
		pdata[6] = 20.0f;
		pdata[7] = 4.0f;
		pdata[8] = 4.0f;
		particleSystem::push(pdata, pupdate);
	}
}

void piawPlayerSpaceShip::key_update() {
	float d = timeBuiltin::delta_time();

	if (inputBuiltin::key_pressed[GLFW_KEY_W])
		upSpeed += d;
	if (inputBuiltin::key_pressed[GLFW_KEY_S])
		upSpeed -= d;
	if (inputBuiltin::key_pressed[GLFW_KEY_D])
		rightSpeed += d;
	if (inputBuiltin::key_pressed[GLFW_KEY_A])
		rightSpeed -= d;
	if (inputBuiltin::key_pressed[GLFW_KEY_P] && timeBuiltin::get_time() - missileTimer[0] > 0.07f) {
		missileTimer[0] = timeBuiltin::get_time();
		piawMissile* elem = new piawMissile(glm::vec3(linePos + 6, upD, rightD + 0.1), 0, 2.5f, 800, 1);
		piawLineEntityManager::push(elem);
		elem = new piawMissile(glm::vec3(linePos + 6, upD, rightD - 0.1), 0, 2.5f, 800, 1);
		piawLineEntityManager::push(elem);
		missileSound.play();
	}
	if (rightSpeed >= 1.0f)
		rightSpeed = 1.0f;
	if (upSpeed >= 1.0f)
		upSpeed = 1.0f;
}

void piawPlayerSpaceShip::speedUpdate() {
	rightD += rightSpeed / 18;
	upD += upSpeed / 18;

	if (rightD > 0.8f) {
		rightD = 0.8f;
		rightSpeed = 0;
	}

	if (rightD < -0.8f) {
		rightD = -0.8f;
		rightSpeed = 0;
	}

	if (upD > 0.8f) {
		upD = 0.8f;
		upSpeed = 0;
	}

	if (upD < -0.8f) {
		upD = -0.8f;
		upSpeed = 0;
	}

	rightSpeed /= 1.01f;
	upSpeed /= 1.01f;
}

void piawPlayerSpaceShip::update() {
	linePos = piawMap::playerLinePos + _camDist;
	get_transform();
	glm::vec3 p1 = piawMap::get_point_at((uint32_t)linePos + 51), p2 = piawMap::get_point_at((uint32_t)(linePos) + 52);
	t_transform *t = transformBuiltin::get_transform(transformHandler);
	p1 = glm::normalize((p1 - p2));
	glm::vec3 right = glm::cross(glm::vec3(0, 1, 0), -p1);
	key_update();
	speedUpdate();
	transformBuiltin::get_transform(transformHandler)->rotation = transformBuiltin::LookAtObject(-p1);
	transformBuiltin::rotate(transformHandler, -p1, -rightSpeed);
	transformBuiltin::rotate(transformHandler, right, upSpeed);
	generate_particle(p1, t);
	render();
}
