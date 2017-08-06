#pragma once
#include <renderBuiltIn.class.hpp>

#define MAX_PARTICLE 46096

class particleSystem {

public:

	static void init();
	static void shutdown();
	static void update();
	static void push(float parData[9], void (*parFarray)(float*, char*), char podata[64]);

private:

	static float	*data;
	static char		*objData;
	static uint32_t size;
	static t_instanceRenderer ir;
	static void (*farray[MAX_PARTICLE])(float*, char*);
};