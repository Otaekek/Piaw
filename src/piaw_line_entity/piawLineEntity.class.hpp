#include <piawMap.class.hpp>
#include <list>
#include <oob.class.hpp>

#pragma once

class piawLineEntity {

	public:

	piawLineEntity();
	~piawLineEntity();

	void get_transform();
	void set_asset(uint32_t asset);
	virtual void render();

	virtual void update();


	float	linePos;
	float	upD;
	float	rightD;
	bool	kill_me;

	protected:

		uint32_t assetHandler;
		uint32_t transformHandler;
		uint32_t renderGoHandler;
};

class piawLineEntityManager {
public:

	static void init();
	static void push(piawLineEntity *elem);
	static void update();

private:
	
	static  std::list<piawLineEntity*> elems;
};