#ifndef RENDERBUILTIN_CLASS_HPP
#define RENDERBUILTIN_CLASS_HPP

#define GLFW_INCLUDE_GLEXT

#include <glm.hpp>
#include <staticMemoryManager.class.hpp>
#include <dynamicMemoryManager.class.hpp>
#include <vector>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <glfw3.h>
#include <transform.class.hpp>
#include <meshData.hpp>
#include <jobHandler.class.hpp>
#include <gtc/type_ptr.hpp>
#include <jojishi.hpp>
#include "renderDataSys.class.hpp"

#define MAX_SUBSCRIBE 65536

#define E_CONIC 0
#define E_ORTH 1

typedef struct	s_render {
	uint32_t	assetHandler;
	uint32_t	transformHandler;
	uint64_t	cameraLayer;
	bool		cullOnMe;
	GLenum		cullMode;
}				t_renderGO;

typedef struct	s_vox
{
	uint32_t	transformHandler;
	uint32_t	vaoId;
	uint32_t	vboId;
	uint32_t	indiceId;
	uint32_t	numVertex;
	uint32_t	program;
}				t_renderVox;

typedef struct	s_instanceRenderer {
	uint32_t	assetHandler;
	uint32_t	vao;
	uint32_t	vbo;
	uint32_t	num;
}				t_instanceRenderer;

class renderBuiltIn {

public :

	static	void					init();
	static	void					shutdown();

	static	uint32_t				create();
	static	void					destroy(uint32_t ref);
	static	t_renderGO				*get_renderGO(uint32_t ref);
	static	void					render_me(uint32_t assetHandler);

	static	uint32_t				create_camera();
	static	void					destroy_camera(uint32_t ref);
	static	t_camera				*get_camera(uint32_t ref);
	static	void					add_camera(uint32_t camHandler);


	static	void					update();

	static GLFWwindow*				get_window();

	static GLFWvidmode				*get_mode();

	static void						modify_skybox(uint32_t handler);
	static t_renderGO				*get_skyboxGO();
	static void						modify_skybox_light(float f);

	static uint32_t					create_light();
	static void						destroy_light(uint32_t ref);
	static void						add_light(uint32_t ref);
	static t_light					*get_light(uint32_t ref);

	static uint32_t					createVox();
	static void						destroyVox(uint32_t ref);

	static void						swap_buffer();

	static GLuint					init_instancing(uint32_t vaoId);
	static void						stream_transform(uint32_t vao, uint32_t vbo, float *data, uint32_t n);
	static t_instanceRenderer		create_instancing(uint32_t assetHandler);
	static void						render_me_instanced(t_instanceRenderer ir, uint32_t num, float *data);
	static void						render_instanced(t_camera *camera);

private :

	static uint32_t					vox_tex;
	static GLuint					roof_tex;

	static GLFWvidmode*				mode;
	static GLFWwindow*				window;

	static uint32_t					cluster_id;
	static uint32_t					list[MAX_SUBSCRIBE];
	static uint32_t					sizeList;

	static uint32_t					camCluster_id;
	static uint32_t					_cameras[512];
	static uint32_t					numCamera;

	static	uint32_t				vox_cluster_id;

	static float					skybox_light;
	static uint32_t					skyboxGO;

	static uint32_t					lightClusterId;
	static uint32_t					_numLight;
	static uint32_t					_lightsHandlers[4096];

	static GLuint					_colorTextureid;
	static GLuint					_frameBufferid;
	static GLuint					_quadVertexbuffer;
	static void						render(t_camera *camera);
	static void						render_node(t_node node, t_renderGO *elem, uint32_t program, uint32_t numInstance);
	static void						render_mesh(t_renderMeshData *mesh, t_renderGO *elem, uint32_t program, uint32_t numInstance);

	static void						render_object(t_renderGO *elem, t_camera *camera);
	static void						render_skybox(t_camera *camera);

	static void						push_light(t_renderGO *elem, GLuint program);

	static void						face_culling(t_renderGO *go);

	static void						create_framebuffer();
	static void						create_onScreenRendering_data();
	static void						renderOnScreen();

	static void						init_vox();
	static void						renderVox(t_camera camera, float width, float height);

	static void						render_vox_mesh(t_renderVox *elem, uint32_t program);
	static void 					render_vox(t_renderVox *elem, t_camera *camera, uint32_t program);
	static uint32_t					numInstance;
	static t_instanceRenderer					instances[4096];
};

#endif
