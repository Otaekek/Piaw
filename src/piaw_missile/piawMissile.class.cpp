#include "piawMissile.class.hpp"

piawMissile::piawMissile(): piawLineEntity() {
	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/bullet/Bullet.obj", E_3D));
	transformBuiltin::scale(transformHandler, 100, 100, 100);
}

piawMissile::piawMissile(glm::vec3 parPos, uint32_t parTeam, float parLifeTime, float parSpeed, float parSide): piawLineEntity() {
	set_asset(fileLoader::load_fs_asset_sync("assets/graphic/mesh/bullet/Bullet.3ds", E_3D));
	team = parTeam;
	life = 1;
	linePos = parPos.x;
	upD = parPos.y;
	rightD = parPos.z;
	lifeTime = parLifeTime;
	side = parSide;
	speed = parSpeed;
	transformBuiltin::scale(transformHandler, 2300, 2300, 2300);
}

piawMissile::~piawMissile() {

}

void piawMissile::update() {
	glm::vec3 dir = glm::normalize(piawMap::get_point_at(linePos + 1) - piawMap::get_point_at(linePos));
	dir.y = 0;
	linePos += timeBuiltin::delta_time() * speed * side;
	lifeTime -= timeBuiltin::delta_time();
	if (lifeTime < 0)
		kill_me = true;
	get_transform();
	transformBuiltin::get_transform(transformHandler)->rotation =  transformBuiltin::LookAtObject(dir);
	transformBuiltin::rotate_model(transformHandler, glm::cross(glm::vec3(0, 1, 0), -dir * side), 1.6);
	set_collider(glm::vec3(2000, 2000, 3000));
	render_collider();
	physic_update();
	render();
}

void piawMissile::getHitBy(piawLineEntity *b) {
	((t_sound*)staticMemoryManager::get_data_ptr(fileLoader::load_fs_asset_sync("assets/sound/piaw/soundEffect/cu.wav", E_SOUND)))->sound->play();
	particleOnDie(5000);
	kill_me = true;
}