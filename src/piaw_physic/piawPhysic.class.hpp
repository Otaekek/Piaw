#pragma once

#include <transform.class.hpp>
#include <oob.class.hpp>
#include <piawLineEntity.class.hpp>

class piawLineEntity;

#define MAX_COL 4096

typedef struct		s_collider {
	obb				c;
	uint32_t		mask;
	piawLineEntity	*o;
	glm::vec3		dir;
}					t_collider;

class piawPhysic {

public:

	static void init();
	static void shutdown();
	static void update();

	static void destroy(uint32_t k);
	static void push(uint32_t handler);

	static uint32_t create(obb &obb, uint32_t mask, piawLineEntity *o);

	static t_collider *get_collider(uint32_t h);

private:

	static uint32_t colliders[MAX_COL];
	static uint32_t	colliders_size;
	static uint32_t	cluster_id;

};
