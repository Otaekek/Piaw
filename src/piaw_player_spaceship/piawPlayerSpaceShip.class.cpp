#include "piawPlayerSpaceShip.class.hpp"
#include <particleSystem.class.hpp>
#include <oob.class.hpp>

piawPlayerSpaceShip::piawPlayerSpaceShip(): missileTimer {0}, piawLineEntity() {
	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/starcruiser military/Starcruiser military.obj", E_3D));
//	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/starcruiser military/s.obj", E_3D));
	transformBuiltin::scale(transformHandler, 80.5, 80.5, 80.5);
	_camDist = 42.6;
	rightD = 0;
	life = 5000;
	upD = 0;
	rot = 0;
	soundHandler = fileLoader::load_fs_asset_sync("assets/sound/piaw/soundEffect/piou.wav", E_SOUND);
}


piawPlayerSpaceShip::piawPlayerSpaceShip(float camDist): missileTimer {0},  piawLineEntity() {
	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/starcruiser military/s.obj", E_3D));
	transformBuiltin::scale(transformHandler, 90.5, 90.5, 90.5);
	_camDist = camDist;
}

piawPlayerSpaceShip::~piawPlayerSpaceShip() {

}

void pupdate(float *d, char *od) {
	d[6] /= 1.7 * 1.1;
	d[7] /= 1.7 * 1.1;
	d[8] /= 2.8 * 1.1;
}

void piawPlayerSpaceShip::generate_particle(glm::vec3 dir, t_transform *t) {

	float	pdata[9] = {100};
	char	podata[64] = {0};
	float plifeTime = 1.0f;

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
		memcpy(podata, &plifeTime, sizeof(float));
		particleSystem::push(pdata, pupdate, podata);
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
	if (inputBuiltin::key_pressed[GLFW_KEY_L]) {
		rot += d * 10;
		rot = fmod(rot, 6.28);
	}
	if (inputBuiltin::key_pressed[GLFW_KEY_P] && timeBuiltin::get_time() - missileTimer[0] > 0.07f) {
		missileTimer[0] = timeBuiltin::get_time();
		piawMissile* elem = new piawMissile(glm::vec3(linePos + 6, upD, rightD + 0.1), 0, 2.5f, 800, 1);
		piawLineEntityManager::push(elem);
		elem = new piawMissile(glm::vec3(linePos + 6, upD, rightD - 0.1), 0, 2.5f, 800, 1);
		piawLineEntityManager::push(elem);
		((t_sound*)staticMemoryManager::get_data_ptr(soundHandler))->sound->play();
//		missileSound.play();
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
	if (rot > 0)
		rot -= 0.04f;
}

void piawPlayerSpaceShip::update() {
	glm::vec3 aat[3] = {glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)};

	linePos = piawMap::playerLinePos + _camDist;
	get_transform();
	glm::vec3 p1 = piawMap::get_point_at((uint32_t)linePos + 51), p2 = piawMap::get_point_at((uint32_t)(linePos) + 52);
	t_transform *t = transformBuiltin::get_transform(transformHandler);
	p1 = glm::normalize((p1 - p2));
	glm::vec3 right = glm::cross(glm::vec3(0, 1, 0), -p1);
	key_update();
	speedUpdate();
	transformBuiltin::get_transform(transformHandler)->rotation = transformBuiltin::LookAtObject(-p1);
	transformBuiltin::rotate(transformHandler, -p1, -(rightSpeed));
	transformBuiltin::rotate(transformHandler, right, upSpeed);
	transformBuiltin::rotate_model(transformHandler, p1, rot);
	generate_particle(p1, t);

	set_collider(glm::vec3(10000, 1000, 10000));
	render_collider();
	physic_update();
	render();
}
