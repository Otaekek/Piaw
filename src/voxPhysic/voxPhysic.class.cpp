#include "voxPhysic.class.hpp"
#include <time.class.hpp>

uint32_t		voxPhysic::rbdyList[MAX_RGBDY] = {0};
uint32_t		voxPhysic::id_to_index[524280] = {0};

uint32_t		voxPhysic::rbdyListSize = 0;

bool			voxPhysic::voxEnabled = false;

uint32_t		voxPhysic::clusterId;

glm::vec3 ClosestPointOnAABB(glm::vec3 Point, aabb xBox)
{
	glm::vec3 xClosestPoint;
	xClosestPoint.x = (Point.x < xBox.Min.x)? xBox.Min.x : (Point.x > xBox.Max.x)? xBox.Max.x : Point.x;
	xClosestPoint.y = (Point.y < xBox.Min.y)? xBox.Min.y : (Point.y > xBox.Max.y)? xBox.Max.y : Point.y;
	xClosestPoint.z = (Point.z < xBox.Min.z)? xBox.Min.z : (Point.z > xBox.Max.z)? xBox.Max.z : Point.z;

	return xClosestPoint;
}

bool Intersect(glm::vec3 spos, float radius, aabb xBox, glm::vec3& xNcoll, float& fDcoll)
{
	// get closest point on box from sphere centre
	glm::vec3 xClosest = ClosestPointOnAABB(spos, xBox);

	// find the separation
	glm::vec3 xDiff = spos - xClosest;

	// check if points are far enough
	float fDistSquared = glm::length(xDiff);

	if (fDistSquared > sqrt(radius * radius))
	{
		return false;
	}

	float fDist = sqrt(fDistSquared);

	// collision depth
	fDcoll = radius - fDist;
   
	// normal of collision (going towards the sphere centre)
	xNcoll = xDiff  / fDist;

	return true;    
}

t_collision voxPhysic::sphere_collider(rigidBody *rgbd, glm::vec3 d)
{
	t_transform *transform = transformBuiltin::get_transform(rgbd->transform);

	float radius = rgbd->hitbox.sphere.radius;
	glm::vec3 pos = transform->position + d;// - radius;
	uint32_t coord[3];
	t_collision ret;
	glm::vec3 xNcoll;
	float fDcoll;
	ret.collided = false;
	float fcoord[3];
	t_block *block;
	for (int i = -1; SIZE_BLOCK * (i - 2) < 2 * radius; i++)
		for (int j = -1; SIZE_BLOCK * (j - 2) < 2 * radius; j++)
			for (int k = -1; SIZE_BLOCK * (k - 2) < 2 * radius; k++)
			{
				glm::vec3 t = {pos.x + i * SIZE_BLOCK, pos.y + j * SIZE_BLOCK, pos.z + k * SIZE_BLOCK};
				cubeMap::convert_coord(coord, t.x, t.y, t.z);
				cubeMap::convert_coord_to_world(fcoord, coord[0], coord[1], coord[2]);
				glm::vec3 r = {fcoord[0] - 8 * SIZE_BLOCK, fcoord[1] - 8 * SIZE_BLOCK, fcoord[2] - 8 * SIZE_BLOCK};
				block = cubeMap::get_block_at(coord[0], coord[1], coord[2]);
				aabb aabb(glm::vec3(-SIZE_BLOCK/2 + r.x,-SIZE_BLOCK/2 + r.y,-SIZE_BLOCK/2 + r.z), glm::vec3(SIZE_BLOCK / 2 + r.x, SIZE_BLOCK /2 + r.y, SIZE_BLOCK/2 + r.z));

				if (block->type != E_EMPTY && Intersect(pos, radius, aabb, xNcoll, fDcoll))
				{
					ret.collided = true;
					ret.pos = r;
					ret.tdir = d;
					memcpy(ret.bcoord, coord, sizeof(uint32_t) * 3);
					return ret;
				}
			}
	return ret;
}

void voxPhysic::default_collider_handle(rigidBody *rgbd, t_collision c)
{
	rgbd->speed -= c.tdir;
}

void voxPhysic::collide_vox(rigidBody *rgbd)
{
	t_collision c;
	glm::vec3 tdir[3] = {glm::vec3{rgbd->speed.x, 0, 0}, glm::vec3{0, rgbd->speed.y, 0},glm::vec3{0, 0, rgbd->speed.z}};
	for (int i = 0; i < 3; i++)
		if (rgbd->type == E_SPHERE)
			if ((c = sphere_collider(rgbd, tdir[i])).collided)
				default_collider_handle(rgbd, c);
}

void 		voxPhysic::init()
{
	clusterId = dynamicMemoryManager::cluster_init(sizeof(rigidBody), MAX_RGBDY);
}

uint32_t	voxPhysic::create()
{
	return dynamicMemoryManager::create_slot(clusterId);
}

void		voxPhysic::destroy(uint32_t handler)
{
	dynamicMemoryManager::clear_data(clusterId, handler);
}

rigidBody	*voxPhysic::get_rigidBody(uint32_t handler)
{
	return (rigidBody*)dynamicMemoryManager::get_ptr(handler);
}

void voxPhysic::shutdown()
{

}

void voxPhysic::push(uint32_t rbdHandler)
{
	rbdyList[rbdyListSize++] = rbdHandler;
	id_to_index[rbdHandler] = rbdyListSize - 1;
}

void voxPhysic::remove(uint32_t rbdHandler)
{
	rbdyList[id_to_index[rbdHandler]] = rbdyList[rbdyListSize--];
	id_to_index[rbdyList[rbdyListSize + 1]] = id_to_index[rbdHandler];
}

void	voxPhysic::update()
{
	rigidBody *rgbd;
	glm::vec3 tmpspeed;
	for (uint32_t i = 0; i < rbdyListSize; i++)
	{
		rgbd = get_rigidBody(rbdyList[i]);
		//tmpspeed = rgbd->speed * (float)timeBuiltin::delta_time();
		rgbd->speed *= (float)timeBuiltin::delta_time();
		collide_vox(get_rigidBody(rbdyList[i]));
		transformBuiltin::translate(rgbd->transform, rgbd->speed.x, rgbd->speed.y, rgbd->speed.z);
		//rgbd->speed = tmpspeed / (float)timeBuiltin::delta_time();
		rgbd->speed += rgbd->accel * (float)timeBuiltin::delta_time();;
		if (rgbd->type == E_SPHERE)
			rgbd->is_airborne = (!sphere_collider(rgbd, glm::vec3(0, rgbd->speed.y - 1 , 0)).collided || rgbd->speed.y > 0);
		if (rgbd->is_airborne)
			rgbd->accel += glm::vec3(0, -5, 0);
		else
			rgbd->accel = glm::vec3(0, 0, 0);
		rgbd->speed /= (float)timeBuiltin::delta_time();
	}
}

rigidBody::rigidBody()
{

}

rigidBody::~rigidBody()
{

}
