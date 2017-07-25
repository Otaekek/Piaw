#include "basicFPSControlBuiltin.class.hpp"
#include <time.class.hpp>


basicFPSControl::basicFPSControl()
{

}

basicFPSControl::basicFPSControl(float posx, float posy, float posz, float camxpos, float camypos, float camxsize, float camysize)
{
	t_camera *camera;
	t_renderGO *renderGOdata;
	rigidBody  *rgbd;

	_cameraHandler = renderBuiltIn::create_camera();
	camera = renderBuiltIn::get_camera(_cameraHandler);
	camera->posx = camxpos;
	camera->posy = camypos;
	camera->sizex = camxsize;
	camera->sizey = camysize;
	camera->transformHandler = transformBuiltin::create();
	_transformHandler = camera->transformHandler;
	rotx = 0;
	roty = 0;
	relx = 0;
	rely = 0;
	inputBuiltin::disable_cursor();
	renderGO = renderBuiltIn::create();
	renderGOdata = (t_renderGO*)dynamicMemoryManager::get_ptr(renderGO);
	renderGOdata->transformHandler = transformBuiltin::create();
	transformBuiltin::scale(renderGOdata->transformHandler, 5, 5, 5);
	_renderGOTransform = renderGOdata->transformHandler;
	renderGOdata->assetHandler = fileLoader::load_fs_asset_sync("./assets/graphic/mesh/shtouill/pumba.obj", E_3D);
	transformBuiltin::translate(_renderGOTransform, posx, posy, posz);	
	_rigidBodyHandler = voxPhysic::create();
	rgbd = voxPhysic::get_rigidBody(_rigidBodyHandler);
	bzero(rgbd, sizeof(rgbd));
	rgbd->transform = _renderGOTransform;
	rgbd->type = E_SPHERE;
	rgbd->hitbox.sphere.radius = 8;
	rgbd->weight = 120;
	rgbd->is_airborne = false;
	voxPhysic::push(_rigidBodyHandler);
}

basicFPSControl::~basicFPSControl()
{

}

void 		basicFPSControl::update()
{
	behave();
	render();
}

void 		basicFPSControl::render()
{
	renderBuiltIn::add_camera(_cameraHandler);
	renderBuiltIn::render_me(renderGO);
}

void		basicFPSControl::mouse_update(float mouseX, float mouseY)
{
	mouseX = mouseX / (renderBuiltIn::get_mode()->width) - 0.5;
	mouseY = mouseY / (renderBuiltIn::get_mode()->height) - 0.5;
	rotx -= mouseX - relx;
	roty -= mouseY - rely;
	if (roty < -0.4)
		roty = -0.4;
	if (roty > 0.4)
		roty = 0.4;
	rely = mouseY;
	relx = mouseX;
	transformBuiltin::euler_angle(_renderGOTransform, -rotx, 0);
}

void 		basicFPSControl::behave()
{
	t_transform *t;
	rigidBody *rgbd = voxPhysic::get_rigidBody(_rigidBodyHandler);
	glm::vec3 up = glm::vec3(0, 1, 0);

	t = transformBuiltin::get_transform(_renderGOTransform);
	glm::vec3 direction = transformBuiltin::get_direction(_renderGOTransform);
	glm::vec3 crossDirection = -cross(up, direction);
	crossDirection *= 100;
	direction *= 100;
	if (inputBuiltin::key_pressed[GLFW_KEY_LEFT_SHIFT])
	{
		crossDirection *= 20;
		direction *= 20;
	}
	if (inputBuiltin::key_pressed[GLFW_KEY_R])
	{
		cubeMap::surface_net = !cubeMap::surface_net;
		inputBuiltin::key_pressed[GLFW_KEY_R] = false;
	}
	if (rgbd->is_airborne)
	{
	//	direction /= 2;
	//	crossDirection /= 2;
	}

	glm::vec3 patchSpeed = glm::vec3(0, 0, 0);
	if (inputBuiltin::key_pressed[GLFW_KEY_W])
	{
		patchSpeed.x += -direction.x;
		patchSpeed.z += direction.z;
	}		
	if (inputBuiltin::key_pressed[GLFW_KEY_S])
		patchSpeed += glm::vec3(direction.x, -direction.y, -direction.z);
	if (inputBuiltin::key_pressed[GLFW_KEY_D])
		patchSpeed += glm::vec3(crossDirection.x, crossDirection.y, -crossDirection.z);
	if (inputBuiltin::key_pressed[GLFW_KEY_A])
		patchSpeed += glm::vec3(-crossDirection.x, -crossDirection.y, crossDirection.z);
	if (inputBuiltin::key_pressed[GLFW_KEY_SPACE] && !rgbd->is_airborne)
	{
		inputBuiltin::key_pressed[GLFW_KEY_SPACE] = false;
		rgbd->speed.y = 200;
	}
	rgbd->speed.x = patchSpeed.x;
	rgbd->speed.z = patchSpeed.z;
	//transformBuiltin::translate(_renderGOTransform, -crossDirection.x, -crossDirection.y, crossDirection.z);
	if (inputBuiltin::key_pressed[GLFW_KEY_ESCAPE])
		shutdown::exit_jojishi();
	transformBuiltin::reset(_transformHandler);
	direction = glm::normalize(direction);
	transformBuiltin::translate(_transformHandler, -t->position.x - direction.x * 100 - 0, -t->position.y + direction.y * 30 - 40,-t->position.z + direction.z * 100);
	transformBuiltin::rotate(_transformHandler, glm::vec3(0, 1,0), atan2(direction.x, -direction.z));
	transformBuiltin::rotate_model(_transformHandler, glm::vec3(1, 0, 0), roty - 0.4);
}

basicFPSControl		basicFPSControlManagerBuiltin::elems[MAX_ELEM];
uint32_t			basicFPSControlManagerBuiltin::numElem = 0;

void 		basicFPSControlManagerBuiltin::shutdow()
{

}

void 		basicFPSControlManagerBuiltin::update()
{
	for (uint32_t i = 0; i < numElem; i++)
		elems[i].update();
}

void 		basicFPSControlManagerBuiltin::mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	for (uint32_t i = 0; i < numElem; i++)
		elems[i].mouse_update(xpos, ypos);
	//glfwSetCursorPos(renderBuiltIn::get_window(), renderBuiltIn::get_mode()->width / 2, renderBuiltIn::get_mode()->height / 2);
}

void 		basicFPSControlManagerBuiltin::create(float posx, float posy, float posz,float camxpos, float camypos, float camxsize, float camysize)
{
	basicFPSControl control(posx, posy, posz, camxpos, camypos, camxsize, camysize);

	elems[numElem++] = control;
}

void 		basicFPSControlManagerBuiltin::init()
{
	inputBuiltin::add_mouse_move_callback(basicFPSControlManagerBuiltin::mouse_move_callback);
}

uint32_t	basicFPSControlManagerBuiltin::get_main_handler()
{
	return (elems[0]._transformHandler);
}
