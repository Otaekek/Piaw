
#include "piawObstacle.class.hpp"
#include <piawLineEntity.class.hpp>
#include <piawMap.class.hpp>

piawObstacle::piawObstacle() {

}

piawObstacle::piawObstacle(uint32_t assetHandler, glm::vec3 pos, float parLifeTime) : piawLineEntity() {
	lifeTime = parLifeTime;
	set_asset(assetHandler);
	linePos = pos.x;
	upD = pos.y;
	rightD = pos.z;
	transformBuiltin::scale(transformHandler, 10000, 10000, 10000);
}

piawObstacle::~piawObstacle() {

}

void piawObstacle::update() {
	lifeTime -= timeBuiltin::delta_time();
	get_transform();
	set_collider(glm::vec3(10000, 10000, 10000));
	render_collider();
	physic_update();
	render();
//	if (life)
}

void piawObstacleSpawner::init() {

}

void piawObstacleSpawner::shutdown() {

}

void piawObstacleSpawner::update() {
	if (!(rand() % 120)) {
		piawLineEntity *elem = new piawObstacle(fileLoader::load_fs_asset_sync("assets/graphic/mesh/rock/Rock_6_FREE/Rock_6/Rock_6.obj", E_3D), glm::vec3(piawMap::playerLinePos + 632, (float)(rand() - RAND_MAX / 2) / (RAND_MAX / 2), (float)(rand() - RAND_MAX / 1.7) / (RAND_MAX / 1.7)), 100000.0f);
		piawLineEntityManager::push(elem);
	}
}
