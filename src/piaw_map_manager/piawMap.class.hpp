#pragma once

#include <transform.class.hpp>
#include <basicFPSControlBuiltin.class.hpp>
#include <time.class.hpp>
#include <fileLoader.class.hpp>
#include <noise.h>

#define MAX_RENDERGO (44960)

class piawMap {

public:

	static void			init();
	static void			update();
	static void			shutdown();


	static uint32_t		sample_point(uint32_t parPHead);
	static glm::vec3	get_point_at(uint32_t offset);
	static uint32_t		playerTransformHandler;
	static uint32_t		push_renderGO(uint32_t assetHandler, uint32_t transformHandlers);

	static double	playerLinePos;

private:

	static float piawScale(glm::vec3 p1, glm::vec3 p2, uint32_t i);
	static t_instanceRenderer			ir;
	static uint32_t	pHead;
	static uint32_t	renderGoQ[MAX_RENDERGO];
	static uint32_t	renderGoQHead;
	static float	cubeData[MAX_RENDERGO * 9];
};
