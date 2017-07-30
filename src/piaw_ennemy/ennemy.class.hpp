#pragma once

#include <renderBuiltIn.class.hpp>
#include <transform.class.hpp>
#include <time.class.hpp>

#define MAX_ENNEMY 4096

class Ennemy {

public:

	Ennemy();
	Ennemy(uint32_t assetHandler, uint32_t transformHandler);
	~Ennemy();

	void		update();
	void		die();
	bool		kill_me;

private:
	void 		behave();
	uint32_t	_assetHandler;
	uint32_t	transformHandler;
	uint32_t	renderGOhandler;
	float 		_velocity;
};

class EnnemyManager {

public:

	static void init();
	static void shutdown();
	static void create(uint32_t assetHandler, uint32_t transformHandler);
	static void update();
	static void spawnUpdate();

private:

	static double 	_timer;
	static uint32_t	ennemy_list_size;
	static Ennemy	ennemy_list[MAX_ENNEMY];
};
