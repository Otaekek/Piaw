#pragma once

#include <transform.class.hpp>
#include <renderBuiltIn.class.hpp>

#define DEBUG_PHYSIC

class obb {

public:

	static void init();

	obb();
	obb(glm::vec3 parR, glm::vec3 parAxis[3], glm::vec3 parPos);
	void init_render();
	void shutdown_render();
	~obb();

	bool		intersect(obb &b);

	glm::vec4	minSphere();
	glm::vec4	maxSphere();
	uint32_t	render();

	void		from_quat(glm::quat b);

	glm::vec3	r;
	glm::vec3	axis[3];
	glm::vec3	pos;

private:

	bool		s_a_t(obb &b);

	void		set_transform();

	uint32_t	renderGoHandler;
	uint32_t	transformHandler;

	static uint32_t assetHandler;
};