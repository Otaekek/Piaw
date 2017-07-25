#pragma once

#include <cubeMap.class.hpp>

#define MAX_RGBDY 65536

enum e_collider_type {
	E_SPHERE,
	E_BOX,
	E_CAPS,
};

typedef struct s_sphere
{
	float		radius;
}				t_sphere;

typedef struct s_box
{
	float 		p[4];
}				t_box;

typedef union	e_hitbox {
	t_sphere	sphere;
	t_box		box;
}				t_hitbox;

typedef struct	s_collision {
	glm::vec3	pos;
	t_block		*block; // lifetime shoud be < at the scope of the collider handler;
	bool		collided;
	uint32_t	bcoord[3];
	glm::vec3	tdir;
}				t_collision;

class aabb {
public: 
	aabb() {};
	aabb(glm::vec3 min, glm::vec3 max):Min(min), Max(max) {};
	~aabb() {};
	glm::vec3 Min;
	glm::vec3 Max;
};

class rigidBody {

public:

	rigidBody();
	~rigidBody();

	uint32_t			transform;
	e_collider_type 	type;
	t_hitbox			hitbox;
	glm::vec3			accel;
	glm::vec3			speed;
	float				weight;
	uint64_t			mask;
	bool				is_airborne;
};

class voxPhysic {

public:

	static void			init();
	static void			shutdown();

	static void			update();
	
	static uint32_t		create();
	static void			destroy(uint32_t handler);
	static rigidBody	*get_rigidBody(uint32_t handler);

	static void			push(uint32_t rbdHandler);
	static void			remove(uint32_t	rbdHandler);

private:

	static void 		default_collider_handle(rigidBody *rgbd, t_collision c);

	static uint32_t		rbdyList[MAX_RGBDY];
	static uint32_t		id_to_index[524280];

	static uint32_t		rbdyListSize;

	static bool			voxEnabled;

	static uint32_t		clusterId;

	static t_collision			sphere_collider(rigidBody *rgbd, glm::vec3 d);
	static void					collide_vox(rigidBody *rgbd);

};
