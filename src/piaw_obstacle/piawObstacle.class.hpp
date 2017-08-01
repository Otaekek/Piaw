#include <piawLineEntity.class.hpp>

#pragma once

class piawObstacle : public piawLineEntity {

public:

	piawObstacle();
	piawObstacle(uint32_t assetHandler, glm::vec3 pos, float lifeTime);

	~piawObstacle();

	void update();

	float lifeTime;
};

class piawObstacleSpawner {

public:

	static void init();
	static void shutdown();
	static void update();
};