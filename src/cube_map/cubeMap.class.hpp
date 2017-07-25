#pragma once

#include "noise.h"
#include <dynamicMemoryManager.class.hpp>
#include <renderDataSys.class.hpp>
#include <renderBuiltIn.class.hpp>
#include <staticMemoryManager.class.hpp>
#include <jobHandler.class.hpp>
#include <renderDataSys.class.hpp>

#define MAX_ACTIVE_CHUNK 4096 * 2
#define MAX_ACTIVE_HYPER_CHUNK 8
#define CHUNK_SIZE 16
#define	MAX_CHUNK_SIZE 32
#define MAX_CHUNK_SIZE_Y 512 / CHUNK_SIZE
#define HYPER_CHUNK_SIZE 512
#define HYPER_CHUNK_SIZE_Y 1
#define SIZE_BLOCK 20.0f

enum e_block : unsigned char {
	E_EMPTY = 0,
	E_GRASS,
	E_DIRT,
	E_ROCK,
	E_SNOW,
	E_WOOD,
	E_SAND,
	E_LEAF,
	E_LAST
};

typedef struct	s_block
{
	e_block		type;
}				t_block;

typedef struct	s_chunk
{
	t_block		blocks[CHUNK_SIZE + 2][CHUNK_SIZE + 2][CHUNK_SIZE + 2];
	int			init;
	int			meshing;
	int			meshed;
	int			surface_net;
	uint32_t	renderGO;
}				t_chunk;

typedef struct	s_hyper_chunk
{
	uint32_t	chunks[MAX_CHUNK_SIZE][MAX_CHUNK_SIZE_Y][MAX_CHUNK_SIZE];
	uint32_t	queue[MAX_ACTIVE_CHUNK][3];
	uint32_t	qHead;
	uint32_t	qTail;
	bool		treshOut;
}				t_hyper_chunk;


typedef struct		s_mesh_job_data {
	t_chunk			*chunk;
	uint32_t		i;
	uint32_t		j;
	uint32_t		k;
}					t_mesh_job_data;

class cubeMap {

public:

	static void			init();
	static void			shutdown();
	static t_block		*get_block_at(uint32_t i, uint32_t j, uint32_t k);
	static void			set_block_at(uint32_t i, uint32_t j, uint32_t k, t_block block);
	static void			convert_coord(uint32_t out_coord[3], float x, float y, float z);
	static t_renderVox	*get_renderGO(int32_t i, int32_t j, int32_t k);
	static t_chunk		*get_chunk_at(uint32_t i, uint32_t j, uint32_t k);
	static void			convert_coord_to_world(float out_coord[3], uint32_t i, uint32_t j, uint32_t k);
	static void			get_renderGOs_thread(t_renderVox *out[], int32_t i[], int32_t j[], int32_t k[], uint32_t size);
	static uint32_t		surface_net;

private:

	static void			release_chunk_video_memory(t_chunk *c);
	static void			sample_chunks( t_chunk *out, uint32_t i, uint32_t j, uint32_t k);

	static void			create_job_init_chunk(t_chunk *chunk, uint32_t i, uint32_t j, uint32_t k);
	
	static void			init_chunk(uint32_t i, uint32_t j, uint32_t k, uint32_t ci, uint32_t cj, uint32_t ck);
	static uint32_t		clusterID;
	static uint32_t		hyperClusterId;
	static uint32_t		hyper_chunks[HYPER_CHUNK_SIZE][HYPER_CHUNK_SIZE_Y][HYPER_CHUNK_SIZE];
	static uint32_t		meshClusterId;
	static uint32_t		create_assetHandler();
	static int			meshChunk(t_chunk *chunck);
	static uint32_t		hyperqHead;
	static uint32_t		hyperqTail;
	static uint32_t		hyperqueue[MAX_ACTIVE_HYPER_CHUNK * 4][3];

	static void			delete_chunk(uint32_t id[3], t_hyper_chunk *c);
	static void			chunk_deleter_add(uint32_t id[3], t_hyper_chunk *c);
	static void			chunk_deleter_update(t_hyper_chunk *c);

	static float		*mem;
	static GLuint		*idbuffer;
	static t_noise		*noise;

	static	void				init_hyper_chunk(uint32_t i, uint32_t j, uint32_t k);
	static	t_hyper_chunk		*get_hyper_chunk_at(uint32_t i, uint32_t j, uint32_t k);

	static	uint32_t			hyper_queue[MAX_ACTIVE_HYPER_CHUNK * 2][3];
	static	uint32_t			hyper_qHead;
	static	uint32_t			hyper_qTail;

	static void			delete_hyper_chunk(uint32_t id[3]);
	static void			hyper_chunk_deleter_add(uint32_t i, uint32_t j, uint32_t k);
	static void			hyper_chunk_deleter_update();
	
	static void			update_chunk(t_chunk *chunk);
	static t_chunk		*get_unloaded_chunk(int32_t i, int32_t j, int32_t k);

	static void			subscribe_chunk(uint32_t i, uint32_t j, uint32_t k, uint32_t ci, uint32_t cj, uint32_t ck);

	static void			mesh(void *data);
	static void			create_job(t_chunk *chunk, uint32_t i, uint32_t j, uint32_t k);
	static void			release_video_memory(t_chunk *c);

	static void 		init_chunk_blocks(t_chunk *out, uint32_t i, uint32_t j, uint32_t k);
	static void			job_init_chunk_blocks(void *data);
};
