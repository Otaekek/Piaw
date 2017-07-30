#include "piawLineEntity.class.hpp"

std::list<piawLineEntity*> piawLineEntityManagerelems;

piawLineEntity::piawLineEntity() {
	kill_me = false;
	transformHandler = transformBuiltin::create();
	renderGoHandler = renderBuiltIn::create();
}

piawLineEntity::~piawLineEntity() {

}

void piawLineEntity::update() {

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

	transform->position = pos + right * rightD + up * upD;
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
			piawLineEntityManagerelems.erase(it);
	}
}
