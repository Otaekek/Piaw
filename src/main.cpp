
#include <sys/time.h>
#include "memory_allocator/stackAllocator.class.hpp"
#include "static_memory_manager/staticMemoryManager.class.hpp"
#include "job_system/jobSystem.hpp"
#include "file_loader/fileLoader.class.hpp"
#include <cstdlib>
#include <iostream>
#include "job_system/jobHandler.class.hpp"
#include "../assimp/include/assimp/Importer.hpp"
#include <unistd.h>
#include "memory_allocator/poolAllocator.class.hpp"
#include "dynamic_memory_manager/dynamicMemoryManager.class.hpp"
#include <transform.class.hpp>
#include <iostream>
#include <glfw3.h>
#include <list>
#include <vector>
#include <ctime>
#include <renderBuiltIn.class.hpp>
#include <basicFPSControlBuiltin.class.hpp>
#include <inputBuiltin.class.hpp>
#include <texture_builtin.class.hpp>
#include <basicMeshFactory.class.hpp>
#include <basicLightFactory.class.hpp>
#include <terrainGenerationBuiltin.class.hpp>
#include <mapEditorBuiltin.class.hpp>
#include <directorySmartLoader.class.hpp>
#include <runtimeLibrary.class.hpp>
#include <GUIEventSystem.class.hpp>
#include <time.class.hpp>
#include <time.h>
#include <piawMap.class.hpp>
#include <piawLineEntity.class.hpp>
#include <particleSystem.class.hpp>
#include <piawMusic.class.hpp>
#include <piawObstacle.class.hpp>
#include <piawEnnemy.class.hpp>
#include <oob.class.hpp>
#include <piawPhysic.class.hpp>

void init()
{
	srand(clock());
	dynamicMemoryManager::init();
	texture_builtin::init();
	transformBuiltin::init();
	fileLoader::init();
	renderBuiltIn::init();
	jobHandler::init();
	renderDataSys::init();
	obb::init();
	inputBuiltin::init();
//	voxPhysic::init();
//	cubeMap::init();
	piawPhysic::init();
	basicFPSControlManagerBuiltin::init();
	piawMap::init();
	particleSystem::init();
//	mapEditorBuiltin::init();
	timeBuiltin::init();
	piawMusic::init();
	runtimeLibrary::create_lib("assets/levelBuilder/startEngine");
}

void update()
{
	timeBuiltin::update();
	renderBuiltIn::update();
	inputBuiltin::update();
///	voxPhysic::update();
	basicFPSControlManagerBuiltin::update();
	piawMap::update();
	piawLineEntityManager::update();
	particleSystem::update();
	piawObstacleSpawner::update();
	piawEnnemySpawner::update();
	piawPhysic::update();
	staticMeshManager::update();
	terrainGenerationBuiltin::update();
	basicLightFactory::update();
//	mapEditorBuiltin::update();
	renderBuiltIn::swap_buffer();
}

int main(int ac, char **av)
{
	init();
	while (42)
	{
		update();
		printf("%lf\n", timeBuiltin::get_fps());
	}
}
