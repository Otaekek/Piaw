#include "piawLineEntity.class.hpp"

std::list<piawLineEntity*> piawLineEntityManagerelems;

piawLineEntity::piawLineEntity() {
	life = 10;
	obb o = obb();
	kill_me = false;
	transformHandler = transformBuiltin::create();
	renderGoHandler = renderBuiltIn::create();
	physicHandler = piawPhysic::create(o, 0, this);
	piawPhysic::get_collider(physicHandler)->c.init_render();
}

piawLineEntity::~piawLineEntity() {
	transformBuiltin::destroy(transformHandler);
	renderBuiltIn::destroy(renderGoHandler);
	piawPhysic::get_collider(physicHandler)->c.shutdown_render();
	piawPhysic::destroy(physicHandler);
}

void piawLineEntity::update() {

}

void piawLineEntity::physic_update() {
	piawPhysic::push(physicHandler);
}

void piawLineEntity::set_asset(uint32_t asset) {
	assetHandler = asset;
	t_renderGO *rgo = renderBuiltIn::get_renderGO(renderGoHandler);
	rgo->transformHandler = transformHandler;
	rgo->assetHandler = assetHandler;
	rgo->cullOnMe = true;
	rgo->cullMode = GL_BACK;
}

void piawLineEntity::get_transform() {

	t_transform *transform = transformBuiltin::get_transform(transformHandler);

	glm::vec3 p1 = piawMap::get_point_at(linePos),
	p2 = piawMap::get_point_at(linePos + 1);

	float x = fmod(linePos, 1.0);
	glm::vec3 dir = glm::normalize(p2 - p1),
	up = glm::vec3(0, 1, 0),
	right = glm::cross(dir, up), pos = p2 * x + p1 * (1.0f - x);
	transform->position = pos + right * rightD * 54300.0f + up * upD * 34300.0f;
}

void piawLineEntity::set_collider(glm::vec3 scale) {
	piawPhysic::get_collider(physicHandler)->c.r = scale;
	piawPhysic::get_collider(physicHandler)->c.from_quat(transformBuiltin::get_transform(transformHandler)->rotation);
	piawPhysic::get_collider(physicHandler)->c.pos = transformBuiltin::get_transform(transformHandler)->position;
}

void piawLineEntity::render_collider() {
	piawPhysic::get_collider(physicHandler)->c.render();
}

void piawLineEntity::render() {
	renderBuiltIn::render_me(renderGoHandler);
}

void piawLineEntityManager::init() {
}


void piawLineEntityManager::push(piawLineEntity *elem) {
	piawLineEntityManagerelems.push_back(elem);
}

void piawLineEntityManager::update() {
	for (auto it = piawLineEntityManagerelems.begin(); it != piawLineEntityManagerelems.end();) {
		auto tmp = it;
		(*it)->update();
		++it;
		if ((*tmp)->kill_me)
			piawLineEntityManagerelems.erase(tmp);
	}
}
