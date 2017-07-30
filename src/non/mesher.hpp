#pragma once

#include "cubeMap.class.hpp"
#include <renderDataSys.class.hpp>

typedef struct	s_vox_request
{
	float		*vertex;
	GLuint		*indices;
	GLuint		index;
	t_renderVox	*vox;
}				t_vox_request;

class mesher {

public:
	mesher();
	~mesher();

	int					mesh(t_chunk & chunk, float *mem, GLuint *m, t_renderVox *out);
	int					get_vertex(t_chunk & chunk, float *mem, GLuint *m, uint32_t *outv);
	static void			create_vao(float *v, GLuint *ind, GLuint inds, t_renderVox *out);
	static void			init_table();

	static void			add_request(float *v, GLuint *ind, GLuint inds, t_renderVox *out);
	static void			exec_request();
	
	static t_vox_request	requests[65536];
	static uint32_t		request_count;
	static			uint32_t	tex_table[3][3][3][20];

private:

	uint32_t		vaoId;

	t_chunk			chunk;
	bool			face_done[60][CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

	static			float vertex_table[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE][3];
	static 			float uv_table[4 * 2];
	static 			float normal_table[3][3][3][3];
	static			float ex_table[3][3][3][2][3];

	uint32_t		data_array_index;

	GLuint			*indice_array;
	float			*data_array;
	int				c_type;

	void			propag_meshx(int32_t i, int32_t j, int32_t k, int32_t nx, int32_t ny, int32_t nz, int32_t initi,  int32_t inity, int32_t initz);
	void			propag_meshy(int32_t i, int32_t j, int32_t k, int32_t nx, int32_t ny, int32_t nz, int32_t initi,  int32_t inity, int32_t initz);
	void			propag_meshz(int32_t i, int32_t j, int32_t k, int32_t nx, int32_t ny, int32_t nz, int32_t initi,  int32_t inity, int32_t initz);
	bool			is_occluded(int32_t i, int32_t j, int32_t k, int32_t nx, int32_t ny, int32_t nz);
	void			emit_vertex(int32_t i, int32_t j, int32_t k, int32_t ipos, int32_t jpos, int32_t kpos, int32_t nx, int32_t ny, int32_t nz, e_block type, uint32_t a, uint32_t b);
	void			flush_data(t_renderVox *out);
	static void 	init_text();
};
