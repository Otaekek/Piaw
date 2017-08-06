#pragma once

#include <piawMap.class.hpp>
#include <list>
#include <piawPhysic.class.hpp>
//#include <piawEnnemyCruiser.class.hpp>
#include <particleSystem.class.hpp>

class piawLineEntity {

	public:

	piawLineEntity();
	~piawLineEntity();

	void set_collider(glm::vec3 scale);
	void render_collider();

	void get_transform();
	void set_asset(uint32_t asset);

	virtual void 	particleOnDie(float n);

	virtual void render();
	virtual void physic_update();

	virtual void update();

	virtual void		getHitBy(piawLineEntity *b) {life--; if (life <= 0 ) kill_me = true;}
	virtual uint32_t	getType() {return 0;}

	void		phy_render() {};
	float		linePos;
	float		upD;
	float		rightD;
	uint32_t	life;
	bool		kill_me;

	protected:

		uint32_t assetHandler;
		uint32_t transformHandler;
		uint32_t renderGoHandler;
		uint32_t physicHandler;
};

class piawLineEntityManager {
public:

	static void init();
	static void push(piawLineEntity *elem);
	static void update();

private:
	
	static  std::list<piawLineEntity*> elems;
};