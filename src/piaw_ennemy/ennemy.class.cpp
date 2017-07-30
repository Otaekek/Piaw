
#include "ennemy.class.hpp"

double		EnnemyManager::_timer;
Ennemy		EnnemyManager::ennemy_list[MAX_ENNEMY];
uint32_t	EnnemyManager::ennemy_list_size = 0;

Ennemy::Ennemy() : renderGOhandler(0), transformHandler(0) {
	renderGOhandler = 0;
	transformHandler = 0;
}

Ennemy::Ennemy(uint32_t assetHandler, uint32_t parTransformHandler){

	t_renderGO *t;

	_assetHandler = assetHandler;
	transformHandler = parTransformHandler;
	renderGOhandler = renderBuiltIn::create();
	t = (t_renderGO*)renderBuiltIn::get_renderGO(renderGOhandler);
	t->assetHandler = assetHandler;
	t->transformHandler = transformHandler;
	t->cullOnMe = true;
	t->cullMode = GL_BACK;
	t->cameraLayer = 0;
	kill_me = false;
}

Ennemy::~Ennemy() {

}

void Ennemy::die() {
	transformBuiltin::destroy(transformHandler);
	renderBuiltIn::destroy(renderGOhandler);
	kill_me = true;
}

void Ennemy::behave() {

	if (_velocity < 150)
		_velocity += 0.3;
	transformBuiltin::translate(transformHandler,  0, 0, _velocity);
	if (transformBuiltin::get_transform(transformHandler)->position.z > 4000)
		die();
}

void Ennemy::update() {
	if (staticMemoryManager::get_asset_state(renderBuiltIn::get_renderGO(renderGOhandler)->assetHandler) != staticMemoryManager::E_LOADED) {
		return ;
	}
	behave();
	renderBuiltIn::render_me(renderGOhandler);
}

void EnnemyManager::init() {

}

void EnnemyManager::shutdown() {

}
	
void EnnemyManager::create(uint32_t assetHandler, uint32_t transformHandler) {
	ennemy_list[ennemy_list_size++] = Ennemy(assetHandler, transformHandler);
}

void EnnemyManager::spawnUpdate() {

	if (timeBuiltin::get_time() - _timer < 1.0)
		return ;

	uint32_t tbltn = transformBuiltin::create();
	transformBuiltin::translate(tbltn, rand() % 2000 - 1000, rand() % 2000 - 1000, 0);
	create(
		fileLoader::load_fs_asset_sync("./assets/graphic/mesh/starcruiser military/Starcruiser military.obj", E_3D),
		tbltn);
	_timer = timeBuiltin::get_time();
}

void EnnemyManager::update() {
	//spawnUpdate();
	for (int i = 0; i < ennemy_list_size; i++) {
		if (ennemy_list[i].kill_me) {
			ennemy_list[i--] = ennemy_list[--ennemy_list_size];
			continue ;
		}
		ennemy_list[i].update();
	}
}
