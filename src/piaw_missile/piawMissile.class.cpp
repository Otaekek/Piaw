#include "piawMissile.class.hpp"

piawMissile::piawMissile(): piawLineEntity() {
	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/bullet/Bullet.obj", E_3D));
	transformBuiltin::scale(transformHandler, 100, 100, 100);
}

piawMissile::piawMissile(glm::vec3 parPos, uint32_t parTeam, float parLifeTime): piawLineEntity(){
	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/bullet/Bullet.3ds", E_3D));
	team = parTeam;
	linePos = parPos.x;
	upD = parPos.y;
	rightD = parPos.z;
	lifeTime = parLifeTime;
	transformBuiltin::scale(transformHandler, 2300, 2300, 2300);
}

piawMissile::~piawMissile() {

}

void piawMissile::update() {
	glm::vec3 dir = glm::normalize(piawMap::get_point_at(linePos + 1) - piawMap::get_point_at(linePos));
	dir.y = 0;
	linePos += timeBuiltin::delta_time() * 400;
	lifeTime -= timeBuiltin::delta_time();
	if (lifeTime < 0)
		kill_me = true;
	get_transform();
	transformBuiltin::get_transform(transformHandler)->rotation =  transformBuiltin::LookAtObject(dir);
	transformBuiltin::rotate_model(transformHandler, glm::cross(glm::vec3(0, 1, 0), -dir), 1.6);
	render();
}