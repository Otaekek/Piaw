#include <time.h>
#include "piawMap.class.hpp"
#include <piawMissile.class.hpp>

uint32_t				piawMap::playerTransformHandler;
double					piawMap::playerLinePos = 0.0;
uint32_t				piawMap::renderGoQ[MAX_RENDERGO] = {0};
uint32_t				piawMap::renderGoQHead = 0;
uint32_t				piawMap::pHead = 0;
t_instanceRenderer		piawMap::ir;
float					piawMap::cubeData[MAX_RENDERGO * 9];
uint32_t				colorDefilerIndex = 0;

t_noise *n = noiseNew();


void piawMap::init() {
	uint32_t asset = fileLoader::load_fs_asset_sync("assets/graphic/mesh/skyboxVelcor/cube.obj", E_3D);
	renderDataSys::execute_vao_request();
	ir = renderBuiltIn::create_instancing(asset);
	renderDataSys::set_programm((E_SHADER)6, ir.assetHandler);
}

void piawMap::shutdown() {

}

float piawMap::piawScale(glm::vec3 p1, glm::vec3 p2, uint32_t i) {
	return (280);
}

void piawMap::update() {

	colorDefilerIndex++;
	playerTransformHandler = basicFPSControlManagerBuiltin::get_main_handler();
	t_transform *ptrs = transformBuiltin::get_transform(playerTransformHandler);
	if (timeBuiltin::delta_time() > 0)
		playerLinePos += timeBuiltin::delta_time() * 300;
	t_transform *t = transformBuiltin::get_transform(playerTransformHandler);
	glm::vec3 p1 = get_point_at(playerLinePos), p2 = get_point_at(playerLinePos + 1);
	while (pHead < playerLinePos + (3 * 120 * 1.5)) {
		sample_point(pHead++);
	}

	float x = fmod(playerLinePos, 1.0);
	t->position = p2 * x + p1 * (1.0f - x);
	t->position *= -1;


	glm::vec3 vt = p1-p2;
	vt.y = 0;
	t->rotation = transformBuiltin::transformBuiltin::LookAt(vt);
	const int min = MAX_RENDERGO < renderGoQHead ? MAX_RENDERGO : renderGoQHead;
	for (int i = 0; i < min; i++) {
		t_transform *t = transformBuiltin::get_transform(renderBuiltIn::get_renderGO(renderGoQ[i % MAX_RENDERGO])->transformHandler);
		cubeData[i * 9 + 0] = t->position.x + 00 * (float)rand() / RAND_MAX;;
		cubeData[i * 9 + 1] = t->position.y + 00 * (float)rand() / RAND_MAX;;
		cubeData[i * 9 + 2] = t->position.z + 00 * (float)rand() / RAND_MAX;;
		float s = piawScale(t->position, -ptrs->position, i) * 2;
//		s = 60 * (float)rand() / RAND_MAX;
		//s = 60;
 		cubeData[i * 9 + 3] = s;
		cubeData[i * 9 + 4] = s;
		cubeData[i * 9 + 5] = s;

	}
	renderBuiltIn::render_me_instanced(ir, min, cubeData);
}

glm::vec3 piawMap::get_point_at (uint32_t offset) {

	glm::vec3 t;

	return glm::vec3(200000 * sin((float)offset / 200) , 10000 * cos((float)offset / 150) * 2, offset * 1000) / 0.6f;
}

uint32_t piawMap::push_renderGO(uint32_t assetHandler, uint32_t partransformHandlers) {

	t_renderGO		*rg;
	uint32_t		rghdlr = renderBuiltIn::create();

	rg = renderBuiltIn::get_renderGO(rghdlr);
	rg->assetHandler = assetHandler;
	rg->transformHandler = partransformHandlers;
	rg->cullOnMe = true;
	rg->cullMode = GL_BACK;
	if (renderGoQHead > MAX_RENDERGO) {
		transformBuiltin::destroy(renderBuiltIn::get_renderGO(renderGoQ[(renderGoQHead) % MAX_RENDERGO])->transformHandler);
		renderBuiltIn::destroy((renderGoQ[(renderGoQHead) % MAX_RENDERGO]));
	}
	renderGoQ[renderGoQHead % MAX_RENDERGO] = rghdlr;
	renderGoQHead = (renderGoQHead + 1);
}


uint32_t piawMap::sample_point(uint32_t parPHead) {

	const uint32_t numCube = 200;
uint32_t cube = fileLoader::get_fs(std::string("assets/graphic/mesh/skyboxVelcor/cube.obj"));
 	for (int i = 0; i < numCube; i++) {
 			float prob = 0.9f;
 			glm::vec3 pos = get_point_at(parPHead);
 			glm::vec3 dir = glm::normalize(get_point_at(parPHead + 1) - pos);
 			glm::vec3 up = glm::vec3(0, 1, 0);
			glm::vec3 right = glm::normalize(glm::cross(dir, up));
			glm::vec3 p = pos + right * (float)cos((float)(i) / ((float)numCube / 6.28)) * 54300.0f + up * (float)sin((float)(i) / ((float)numCube / 6.28)) * 34300.0f;
		//	p -= pos - pos / 0.001f;
 			if ((noise3(n, p.x / 11000, p.y / 11000, p.z / 10100) + 1.0) / 1.0f >= prob)
 				continue ;
 		//	p += pos - pos / 0.001f;
 			uint32_t thandler = transformBuiltin::create();
 			t_transform *transform = transformBuiltin::get_transform(thandler);
 			transformBuiltin::scale(thandler, 10, 10,10);
 			transform->position = p;
 	 		push_renderGO(cube, thandler);
 	 	//	cubeData[9 * (renderGoQHead % MAX_RENDERGO) + 6] = (1.0f + cos((float)parPHead / 100)) / 2.0f;
 	 	//	cubeData[9 * (renderGoQHead % MAX_RENDERGO) + 7] = (1.0f + sin((float)parPHead / 100)) / 2.0f;
  		//	cubeData[9 * (renderGoQHead % MAX_RENDERGO) + 8] = ((float)cos((float)(i) / ((float)numCube / 6.28))  + 1)/ 2.0f;

			cubeData[9 * (renderGoQHead % MAX_RENDERGO) + 6] = (1.0f + cos((float)parPHead / 100)) /1.4f;
			cubeData[9 * (renderGoQHead % MAX_RENDERGO) + 7] = fmod((right.x + p.z + up.y  + colorDefilerIndex)/ 80000, 1.0f) / 1.2;
			cubeData[9 * (renderGoQHead % MAX_RENDERGO) + 8] = fmod((right.z * p.x + up.x * colorDefilerIndex) / 80000, 1.0f) / 1.2;
	}
}
