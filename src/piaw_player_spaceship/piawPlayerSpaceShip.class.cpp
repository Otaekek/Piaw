#include "piawPlayerSpaceShip.class.hpp"
#include <particleSystem.class.hpp>

piawPlayerSpaceShip::piawPlayerSpaceShip(): piawLineEntity() {
	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/starcruiser military/Starcruiser military.obj", E_3D));
//	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/viper/viper.obj", E_3D));


	//transformBuiltin::scale(transformHandler, 0.5, 0.5, 0.5);
	transformBuiltin::scale(transformHandler, 30.5, 30.5, 30.5);
}

piawPlayerSpaceShip::~piawPlayerSpaceShip() {

}

void pupdate(float *d) {

}

void piawPlayerSpaceShip::update() {
	linePos = piawMap::playerLinePos + 80.6;
	float pdata[9] = {100};
	upD = 0;
	rightD = 000;
	get_transform();
	glm::vec3 p1 = piawMap::get_point_at((uint32_t)linePos + 51), p2 = piawMap::get_point_at((uint32_t)(linePos) + 52);

	t_transform *t = transformBuiltin::get_transform(transformHandler);
	for (int i = 0; i < 70; i++) {
	pdata[0] = t->position.x + ((float)rand() / RAND_MAX) * 950;
	pdata[1] = t->position.y+ ((float)rand() / RAND_MAX) * 950;
	pdata[2] = t->position.z+ ((float)rand() / RAND_MAX) * 950;
	pdata[3] = 22.5;
	pdata[4] = 22.5;
	pdata[5] = 22.5;
	pdata[6] = 10.0f;
	pdata[7] = 10.0f;
	pdata[8] = 10.0f;
	particleSystem::push(pdata, pupdate);
}
	p1 = glm::normalize((p1 - p2));
	transformBuiltin::get_transform(transformHandler)->rotation = transformBuiltin::LookAtObject(-p1);
	render();
}
