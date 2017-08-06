#include "particleSystem.class.hpp"
#include <renderBuiltIn.class.hpp>

float				*particleSystem::data;
uint32_t			particleSystem::size;
t_instanceRenderer	particleSystem::ir;
void				(*particleSystem::farray[MAX_PARTICLE])(float*, char*);
char				*particleSystem::objData;

void particleSystem::init() {
	uint32_t asset = fileLoader::load_fs_asset_sync("assets/graphic/mesh/skyboxVelcor/cube1.obj", E_3D);
	renderDataSys::execute_vao_request();
	ir = renderBuiltIn::create_instancing(asset);
	renderDataSys::set_programm((E_SHADER)6, ir.assetHandler);
	data = (float*)malloc(MAX_PARTICLE * 9 * sizeof(float));
	objData = (char*)malloc(MAX_PARTICLE * 64);
}

void particleSystem::shutdown() {
	free(data);
}

void particleSystem::push(float parData[9], void (*parFarray)(float*, char*), char d[64]) {
	farray[size % MAX_PARTICLE] = parFarray;
	memcpy(&(data[(size % MAX_PARTICLE) * 9]), parData, sizeof(float) * 9);
	memcpy(&(objData[(size % MAX_PARTICLE) * 64]), d, 64);
	size++;
}

void particleSystem::update() {
	uint32_t min = MAX_PARTICLE < size ? MAX_PARTICLE : size;

	for (int i = 0; i < min; i++) {
		(*farray[i])(&(data[i * 9]), &(objData[i * 64]));
	}
	renderBuiltIn::render_me_instanced(ir, min, data);
}