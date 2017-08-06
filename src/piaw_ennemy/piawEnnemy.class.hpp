#pragma once
#include <piawLineEntity.class.hpp>
#include <particleSystem.class.hpp>

enum E_ENNEMYTYPE {
	E_CRUISER,
};

class piawEnnemy: public piawLineEntity {

public:
	piawEnnemy();
	~piawEnnemy();
	void	speedUpdate();
	float	rightSpeed;
	float	upSpeed;
	float	weaponTimer[12];
	void	getHitBy(piawLineEntity *b);
	bool isDying;
};

class piawEnnemySpawner {

public:
	static void init();
	static void update();
	static void shutdown();

};