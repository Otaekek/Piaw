
#include "piawPhysic.class.hpp"

uint32_t	piawPhysic::colliders[MAX_COL];
uint32_t	piawPhysic::colliders_size;
uint32_t	piawPhysic::cluster_id = 0;


void piawPhysic::init() {
	cluster_id = dynamicMemoryManager::cluster_init(sizeof(t_collider), 65536);
}

void piawPhysic::shutdown() {

}

t_collider *piawPhysic::get_collider(uint32_t h) {
	return (t_collider*)dynamicMemoryManager::get_ptr(h);
}

void piawPhysic::destroy(uint32_t k) {
		dynamicMemoryManager::clear_data(k, cluster_id);
}

void piawPhysic::update() {
	t_collider *a, b;
	piawLineEntity *oa, ob;

	for (int i = 0; i < colliders_size; ++i) {
		for (int j = i + 1; j < colliders_size - 1; ++j) {
			if (get_collider(colliders[i])->c.intersect(get_collider(colliders[j])->c)) {
				get_collider(colliders[i])->o->getHitBy(get_collider(colliders[j])->o);
				get_collider(colliders[j])->o->getHitBy(get_collider(colliders[i])->o);
			}
		}
	}
	colliders_size = 0;
}


void piawPhysic::push(uint32_t h) {
	colliders[colliders_size++] = h;
}

uint32_t piawPhysic::create(obb &obb, uint32_t mask, piawLineEntity *o) {

	t_collider *c;

	uint32_t h = dynamicMemoryManager::create_slot(cluster_id);
	c = (t_collider*)dynamicMemoryManager::get_ptr(h);
	c->c = obb;
	c->mask = mask;
	c->o = o;
	return h;
}
