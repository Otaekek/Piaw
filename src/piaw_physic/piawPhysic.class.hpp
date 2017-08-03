#pragma once

#include <transform.class.hpp>
#include <oob.class.hpp>

#define MAX_COL 4096

typedef struct	s_collider {
	obb			obb;
	uint32_t	handler;
}				t_collider;

class piawPhysic {

public:

	static void init();
	static void shutdown();
	static void update();

	static void push(uint32_t handler);

	static void create(glm::vec3 pos, glm::vec3 axis[3], );


private:

	static uint32_t colliders[MAX_COL];
	static uint32_t	colliders_size;
	static uint32_t	cluster_id;

};
