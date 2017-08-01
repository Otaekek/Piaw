#include <piawLineEntity.class.hpp>

#pragma once

class piawMissile: public piawLineEntity {

public:

	static void init();

	piawMissile();
	piawMissile(glm::vec3 parPos, uint32_t parTeam, float parLifeTime);
	~piawMissile();
	void update();

	static uint32_t StaticAssetHandler;

	uint32_t team;
	float lifeTime;
};