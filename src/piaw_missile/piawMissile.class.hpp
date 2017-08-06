#include <piawLineEntity.class.hpp>

#pragma once

class piawMissile: public piawLineEntity {

public:

	static void init();

	piawMissile();
	piawMissile(glm::vec3 parPos, uint32_t parTeam, float parLifeTime, float speed, float side);
	~piawMissile();
	void update();

	void getHitBy(piawLineEntity *b);
	static uint32_t StaticAssetHandler;

	uint32_t team;
	float speed;
	float side;
	float lifeTime;
};