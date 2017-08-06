#include "piawLineEntity.class.hpp"

std::list<piawLineEntity*> piawLineEntityManagerelems;

piawLineEntity::piawLineEntity() {
	life = 30;
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

void pexplosion(float *d, char *o) {
	d[6] /= 1.7 * 0.8;
	d[7] /= 1.7 * 0.8;
	d[8] /= 2.8 * 0.8;

	d[0] += *(float*)(o) / 0.4f;
	d[1] += *(float*)(o + sizeof(float)) / 0.4f;
	d[2] += *(float*)(o + sizeof(float) * 2) / 0.4f;
}

void piawLineEntity::particleOnDie(float n) {

	float pdata[9] = {0};

	char podata[64];

	t_transform *t = transformBuiltin::get_transform(transformHandler);

	for (int i = 0; i < n; i++) {
		pdata[0] = t->position.x + ((float)rand() / RAND_MAX) * 3450 - 1500;
		pdata[1] = t->position.y + ((float)rand() / RAND_MAX) * 3450 - 1500;
		pdata[2] = t->position.z + ((float)rand() / RAND_MAX) * 3450 - 1500;
		float a =  ((float)rand() / RAND_MAX + 0) * 500.0f - 300.0f;
		float b =  ((float)rand() / RAND_MAX + 0) * 500.0f - 300.0f;
		float c =  ((float)rand() / RAND_MAX + 0) * 500.0f - 300.0f;
		memcpy(podata + sizeof(float) * 0, &a, sizeof(float));
		memcpy(podata + sizeof(float) * 1, &b, sizeof(float));
		memcpy(podata + sizeof(float) * 2, &c, sizeof(float));

		pdata[3] = 182.5;
		pdata[4] = 182.5;
		pdata[5] = 182.5;
		pdata[6] = 20.0f;
		pdata[7] = 4.0f;
		pdata[8] = 4.0f;
		particleSystem::push(pdata, pexplosion, podata);
	}
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
