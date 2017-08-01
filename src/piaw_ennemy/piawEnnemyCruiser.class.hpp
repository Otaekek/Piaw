#include "piawEnnemy.class.hpp"
#pragma once
class piawEnnemyCruiser : public piawEnnemy {

public:

	piawEnnemyCruiser();
	piawEnnemyCruiser(glm::vec3 pos);
	~piawEnnemyCruiser();
	void update();
	void shoot();
};