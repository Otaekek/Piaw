#include "piawEnnemyCruiser.class.hpp"
#include <piawMissile.class.hpp>
piawEnnemyCruiser::piawEnnemyCruiser() {

}

piawEnnemyCruiser::piawEnnemyCruiser(glm::vec3 pos): piawEnnemy() {
	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/darkfight/dark_fighter_6/dark_fighter_6.3ds", E_3D));
//	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/starcruiser military/s.obj", E_3D));
	transformBuiltin::scale(transformHandler, 600, 600, 600);
	linePos = pos.x;
	upD = pos.y;
	rightD = pos.z;
}

piawEnnemyCruiser::~piawEnnemyCruiser() {

}
void piawEnnemyCruiser::shoot() {
	if (timeBuiltin::get_time() - weaponTimer[0] < 0.2f)
		return ;
	piawMissile* elem = new piawMissile(glm::vec3(linePos - 6, upD, rightD), 1, 2.5f, 500, -1);
	piawLineEntityManager::push(elem);
	weaponTimer[0] = timeBuiltin::get_time();
}

void piawEnnemyCruiser::update() {
	linePos += timeBuiltin::delta_time() * 200;
	get_transform();
	glm::vec3 p = piawMap::get_point_at(linePos + 0) - piawMap::get_point_at(linePos + 1);
	p.y = 0;
	transformBuiltin::get_transform(transformHandler)->rotation = transformBuiltin::LookAtObject(p);
	transformBuiltin::rotate_model(transformHandler, glm::vec3(0, 1, 0), -1.35);
	transformBuiltin::rotate_model(transformHandler, p, -1.6);
	if (!(rand() % 100)) {
		rightSpeed += (float)rand() / RAND_MAX - 0.5;
		upSpeed += (float)rand() / RAND_MAX - 0.5;
	}
	shoot();
	transformBuiltin::rotate_model(transformHandler, -p, -rightSpeed);
	speedUpdate();
	render();
}
