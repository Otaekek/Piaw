#pragma once

#define FPS_COUNT 20
#include <time.h>
#include <stdint.h>

class timeBuiltin {

public:

	static void		init();
	static void		shutdown();
	static void		update();

	static double	get_time();
	static double	delta_time();
	static void		setSpeed(double speed);

	static double	get_fps();

private:

	static uint32_t		fps_count;
	static double		fps;
	static double		deltaTime;
	static double		timeSpeed;
	static double		lastTime;
	static double		gettime();
};