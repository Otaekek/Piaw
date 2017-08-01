#include <piawLineEntity.class.hpp>

#pragma once

enum E_ENNEMYTYPE {
	E_CRUISER,
};

class piawEnnemy: public piawLineEntity {

public:
	piawEnnemy();
	~piawEnnemy();
	void speedUpdate();
	float rightSpeed;
	float upSpeed;
	float weaponTimer[12];
};

class piawEnnemySpawner {

public:
	static void init();
	static void update();
	static void shutdown();

};