#include <cstdio>
#include "time.class.hpp"
#include <renderBuiltIn.class.hpp>

uint32_t	timeBuiltin::fps_count;
double		timeBuiltin::deltaTime;
double		timeBuiltin::timeSpeed;
double		timeBuiltin::lastTime;
double		timeBuiltin::fps;

void	timeBuiltin::init()
{
	deltaTime = 0;
	timeSpeed = 1;
	lastTime = clock();
	fps = 0;
	fps_count = 0;
}

void	timeBuiltin::shutdown()
{

}

void	timeBuiltin::update()
{
	static double tmp_fps = 0;

	deltaTime = (glfwGetTime() - lastTime) * timeSpeed;
	tmp_fps += (glfwGetTime() - lastTime);
	lastTime = glfwGetTime();
	fps_count++;
	if (fps_count == FPS_COUNT)
	{
		fps = tmp_fps;
		tmp_fps = 0;
		fps_count = 0;
	}
}

double	timeBuiltin::get_time()
{
	return glfwGetTime();
}

double	timeBuiltin::delta_time()
{
	return (deltaTime);
}

void	timeBuiltin::setSpeed(double speed)
{
	timeSpeed = speed;
}

double timeBuiltin::get_fps()
{
	return (1.0f / (fps / FPS_COUNT));
}
