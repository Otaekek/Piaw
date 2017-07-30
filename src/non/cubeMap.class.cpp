#include "cubeMap.class.hpp"
#include <fileLoader.class.hpp>
#include "mesher.hpp"
#include "surfaceNetMesher.hpp"

uint32_t cubeMap::surface_net = 0;

int init_perlin();
e_block get_id_at(uint32_t x, uint32_t y, uint32_t z, uint32_t seedz, t_noise *noise);

std::mutex mut;
uint32_t 	cubeMap::clusterID = 0;
uint32_t 	cubeMap::meshClusterId = 0;
t_noise		*cubeMap::noise;
GLuint		*cubeMap::idbuffer;

uint32_t	cubeMap::hyperClusterId;
uint32_t	cubeMap::hyper_chunks[HYPER_CHUNK_SIZE][HYPER_CHUNK_SIZE_Y][HYPER_CHUNK_SIZE] = {0};

float		*cubeMap::mem;

uint32_t	cubeMap::hyper_queue[MAX_ACTIVE_HYPER_CHUNK * 2][3] = {0};
uint32_t	cubeMap::hyper_qHead = 0;
uint32_t	cubeMap::hyper_qTail = 0;


void		cubeMap::init()
{
	clusterID = dynamicMemoryManager::cluster_init(sizeof(t_chunk), MAX_ACTIVE_CHUNK * MAX_ACTIVE_HYPER_CHUNK / 1.5);
	hyperClusterId = dynamicMemoryManager::cluster_init(sizeof(t_hyper_chunk), MAX_ACTIVE_HYPER_CHUNK);
	mesher::init_table();
	iso_mesher::init_table();
//	mem = (float*)malloc(sizeof(float) * 3966309);
//	idbuffer = (GLuint*)malloc(sizeof(float) * 3966309);
	printf("%d\n", (sizeof(float) * 3966309 * 6) * 8);
	printf("%lu %lu\n", (MAX_ACTIVE_CHUNK * MAX_ACTIVE_HYPER_CHUNK) * sizeof(t_chunk) + MAX_ACTIVE_HYPER_CHUNK * sizeof(t_hyper_chunk), MAX_ACTIVE_CHUNK * MAX_ACTIVE_HYPER_CHUNK);
	noise = noiseNew();
}

void		cubeMap::shutdown()
{
	free(mem);
	free(noise);
}

t_block		*cubeMap::get_block_at(uint32_t i, uint32_t j, uint32_t k)
{
	t_chunk *chunk;
	chunk = get_unloaded_chunk((i / CHUNK_SIZE) * CHUNK_SIZE, (j / CHUNK_SIZE) * CHUNK_SIZE, (k / CHUNK_SIZE) * CHUNK_SIZE);
	return &(chunk->blocks[i % CHUNK_SIZE][j % CHUNK_SIZE][k % CHUNK_SIZE]);
}

void			cubeMap::init_hyper_chunk(uint32_t i, uint32_t j, uint32_t k)
{
	hyper_chunk_deleter_update();
	hyper_chunks[i][j][k] = dynamicMemoryManager::create_slot(hyperClusterId);
	dynamicMemoryManager::zerofy(hyper_chunks[i][j][k], sizeof(t_hyper_chunk));
	hyper_chunk_deleter_add(i, j, k);
}

void			cubeMap::delete_hyper_chunk(uint32_t id[3])
{
	t_hyper_chunk *hyper_chunk = (t_hyper_chunk*)dynamicMemoryManager::get_ptr(hyper_chunks[id[0]][id[1]][id[2]]);
	for (int i = hyper_chunk->qTail; i != hyper_chunk->qHead; i++)
	{
		if (i > MAX_ACTIVE_CHUNK)
			i = 0;
		delete_chunk(hyper_chunk->queue[i], hyper_chunk);
	}
	dynamicMemoryManager::clear_data(hyper_chunks[id[0]][id[1]][id[2]], hyperClusterId);
	hyper_chunks[id[0]][id[1]][id[2]] = 0;
}

void			cubeMap::hyper_chunk_deleter_add(uint32_t i, uint32_t j, uint32_t k)
{
	hyper_queue[hyper_qHead][0] = i;
	hyper_queue[hyper_qHead][1] = j;
	hyper_queue[hyper_qHead][2] = k;
	hyper_qHead++;
	if (hyper_qHead > MAX_ACTIVE_HYPER_CHUNK)
		hyper_qHead = 0;
}

void			cubeMap::hyper_chunk_deleter_update()
{
	static bool treshOut = false;

	if (hyper_qHead >= MAX_ACTIVE_HYPER_CHUNK)
		treshOut = true;
	if (treshOut)
	{
		delete_hyper_chunk(hyper_queue[hyper_qTail]);
		hyper_qTail++;
		if (hyper_qTail > MAX_ACTIVE_HYPER_CHUNK)
			hyper_qTail = 0;
	}
}

void			cubeMap::subscribe_chunk(uint32_t i, uint32_t j, uint32_t k, uint32_t ci, uint32_t cj, uint32_t ck)
{
	t_chunk *chunk;
	t_renderVox *rg;
	t_node *node;
	t_hyper_chunk *hyper_chunk;

	float coord[3];
	uint32_t ckid[3];
	convert_coord_to_world(coord, (ci / CHUNK_SIZE) * CHUNK_SIZE, (cj / CHUNK_SIZE) * CHUNK_SIZE, (ck / CHUNK_SIZE) * CHUNK_SIZE);
	hyper_chunk = get_hyper_chunk_at(ci, cj, ck);
	chunk_deleter_update(hyper_chunk);
	hyper_chunk->chunks[i][j][k] = dynamicMemoryManager::create_slot(clusterID);
	ckid[0] = i;
	ckid[1] = j;
	ckid[2] = k;
	chunk_deleter_add(ckid, hyper_chunk);
	dynamicMemoryManager::zerofy(hyper_chunk->chunks[i][j][k], sizeof(t_chunk));
	chunk = (t_chunk*)dynamicMemoryManager::get_ptr(hyper_chunk->chunks[i][j][k]);
	chunk->renderGO = renderBuiltIn::createVox();
	rg = (t_renderVox*)dynamicMemoryManager::get_ptr(chunk->renderGO);
	rg->transformHandler = transformBuiltin::create();
	chunk->init = 0;
	chunk->meshed = 0;
	rg->vaoId = 0;
	transformBuiltin::scale(rg->transformHandler, CHUNK_SIZE * SIZE_BLOCK,CHUNK_SIZE * SIZE_BLOCK,CHUNK_SIZE * SIZE_BLOCK);
	transformBuiltin::translate(rg->transformHandler, coord[0], coord[1], coord[2]);
}

void			cubeMap::init_chunk(uint32_t i, uint32_t j, uint32_t k, uint32_t ci, uint32_t cj, uint32_t ck)
{
	t_chunk *chunk;
	t_renderVox *rg;
	t_node *node;
	t_hyper_chunk *hyper_chunk;

	exit(0);
	float coord[3];
	uint32_t ckid[3];
	convert_coord_to_world(coord, (ci / CHUNK_SIZE) * CHUNK_SIZE, (cj / CHUNK_SIZE) * CHUNK_SIZE, (ck / CHUNK_SIZE) * CHUNK_SIZE);
	hyper_chunk = get_hyper_chunk_at(ci, cj, ck);
	chunk_deleter_update(hyper_chunk);
	hyper_chunk->chunks[i][j][k] = dynamicMemoryManager::create_slot(clusterID);
	ckid[0] = i;
	ckid[1] = j;
	ckid[2] = k;
	chunk_deleter_add(ckid, hyper_chunk);
	dynamicMemoryManager::zerofy(hyper_chunk->chunks[i][j][k], sizeof(t_chunk));
	chunk = (t_chunk*)dynamicMemoryManager::get_ptr(hyper_chunk->chunks[i][j][k]);
	chunk->renderGO = renderBuiltIn::createVox();
	rg = (t_renderVox*)dynamicMemoryManager::get_ptr(chunk->renderGO);
	rg->transformHandler = transformBuiltin::create();
	chunk->init = 1;
	chunk->meshed = 0;
	transformBuiltin::scale(rg->transformHandler, CHUNK_SIZE * SIZE_BLOCK,CHUNK_SIZE * SIZE_BLOCK,CHUNK_SIZE * SIZE_BLOCK);
	transformBuiltin::translate(rg->transformHandler, coord[0], coord[1], coord[2]);
	ci = (ci / CHUNK_SIZE) * CHUNK_SIZE;
	cj = (cj / CHUNK_SIZE) * CHUNK_SIZE;
	ck = (ck / CHUNK_SIZE) * CHUNK_SIZE;
	for (int a = 0; a < CHUNK_SIZE; a++)
		for (int b = 0; b < CHUNK_SIZE; b++)
			for (int c = 0; c < CHUNK_SIZE; c++)
				chunk->blocks[a][b][c].type = get_id_at(a + ci, b + cj, c + ck, 0, noise);
}

t_hyper_chunk *cubeMap::get_hyper_chunk_at(uint32_t i, uint32_t j, uint32_t k)
{
	uint32_t		ref;

	ref = hyper_chunks[i / (CHUNK_SIZE * MAX_CHUNK_SIZE)][j / (CHUNK_SIZE * MAX_CHUNK_SIZE_Y)][k / (CHUNK_SIZE * MAX_CHUNK_SIZE)];
	if (ref == 0)
	{
		
		init_hyper_chunk(i / (CHUNK_SIZE * MAX_CHUNK_SIZE), j / (CHUNK_SIZE * MAX_CHUNK_SIZE_Y),k / (CHUNK_SIZE * MAX_CHUNK_SIZE));
		ref = hyper_chunks[i / (CHUNK_SIZE * MAX_CHUNK_SIZE)][j / (CHUNK_SIZE * MAX_CHUNK_SIZE_Y)][k / (CHUNK_SIZE * MAX_CHUNK_SIZE)];
		
	}
	return (t_hyper_chunk*)dynamicMemoryManager::get_ptr(ref); 
}

t_chunk		*cubeMap::get_unloaded_chunk(int32_t i, int32_t j, int32_t k)
{
	uint32_t		ref;
	t_hyper_chunk	*hyper_chunk;


	i = i / CHUNK_SIZE * CHUNK_SIZE;
	j = j / CHUNK_SIZE * CHUNK_SIZE;
	k = k / CHUNK_SIZE * CHUNK_SIZE;

	hyper_chunk = get_hyper_chunk_at(i, j, k);
	t_chunk			*ret;

	uint32_t a = (i / CHUNK_SIZE) % MAX_CHUNK_SIZE;
	uint32_t b = (j / CHUNK_SIZE);
	uint32_t c = (k / CHUNK_SIZE) % MAX_CHUNK_SIZE;
	ref = hyper_chunk->chunks[a][b][c];
	if (ref == 0)
	{
		subscribe_chunk(a, b, c, i, j, k);
		ref = hyper_chunk->chunks[a][b][c];
	}
	ret = (t_chunk*)dynamicMemoryManager::get_ptr(ref);
	if (ret->surface_net != surface_net && !ret->meshing && ret->meshed)
	{
		ret->meshed = 0;
				release_video_memory(ret);
	}
	return (ret);
}

t_chunk			*cubeMap::get_chunk_at(uint32_t i, uint32_t j, uint32_t k)
{
	uint32_t	ref;
	t_chunk		*c;

	c = get_unloaded_chunk(i, j, k);
	if (!c->init)
		init_chunk_blocks(c, i, j, k);
	return (c);
}

void		cubeMap::set_block_at(uint32_t i, uint32_t j, uint32_t k, t_block block)
{
	t_mesh_job_data jbdt;
	t_chunk *chunk = get_chunk_at(i / CHUNK_SIZE * CHUNK_SIZE, j / CHUNK_SIZE * CHUNK_SIZE, k / CHUNK_SIZE * CHUNK_SIZE);
	t_block *b = get_block_at(i, j, k);
	*b = block;
}

void		cubeMap::convert_coord(uint32_t out_coord[3], float x, float y, float z)
{
	x /= SIZE_BLOCK;
	y /= SIZE_BLOCK;
	z /= SIZE_BLOCK;

	x += (HYPER_CHUNK_SIZE * MAX_CHUNK_SIZE * CHUNK_SIZE) / 2;
	y += (HYPER_CHUNK_SIZE_Y * MAX_CHUNK_SIZE_Y * CHUNK_SIZE) / 2;
	z += (HYPER_CHUNK_SIZE * MAX_CHUNK_SIZE * CHUNK_SIZE) / 2;
	
	out_coord[0] = x;
	out_coord[1] = y;
	out_coord[2] = z;
}

void		cubeMap::convert_coord_to_world(float out_coord[3], uint32_t i, uint32_t j, uint32_t k)
{
	out_coord[0] = i;
	out_coord[1] = j;
	out_coord[2] = k;

	out_coord[0] -= (MAX_CHUNK_SIZE * CHUNK_SIZE * HYPER_CHUNK_SIZE) / 2 - 8;
	out_coord[1] -= (MAX_CHUNK_SIZE_Y * CHUNK_SIZE * HYPER_CHUNK_SIZE_Y) / 2 - 8;
	out_coord[2] -= (MAX_CHUNK_SIZE * CHUNK_SIZE * HYPER_CHUNK_SIZE) / 2 - 8;

	out_coord[0] *= SIZE_BLOCK;
	out_coord[1] *= SIZE_BLOCK;
	out_coord[2] *= SIZE_BLOCK;
}

uint32_t		cubeMap::create_assetHandler()
{
	return (dynamicMemoryManager::create_slot(meshClusterId));
}

void		cubeMap::delete_chunk(uint32_t id[3], t_hyper_chunk *hyper_chunk)
{
	t_chunk *chunk;
	t_renderVox *rg;

	if (hyper_chunk->chunks[id[0]][id[1]][id[2]] != 0)
	{
		chunk = (t_chunk*)dynamicMemoryManager::get_ptr(hyper_chunk->chunks[id[0]][id[1]][id[2]]);
		rg = (t_renderVox*)dynamicMemoryManager::get_ptr(chunk->renderGO);

		if (chunk->meshed)
		{
			glDeleteBuffers(1, &(rg->vboId));
			glDeleteBuffers(1, &(rg->indiceId));
			glDeleteVertexArrays(1, &(rg->vaoId));
		}
		transformBuiltin::destroy(rg->transformHandler);
		renderBuiltIn::destroyVox(chunk->renderGO);
		dynamicMemoryManager::clear_data(hyper_chunk->chunks[id[0]][id[1]][id[2]], clusterID);
	}
	hyper_chunk->chunks[id[0]][id[1]][id[2]] = 0;
}

void		cubeMap::chunk_deleter_update(t_hyper_chunk *hyper_chunk)
{
	if (hyper_chunk->qHead >= MAX_ACTIVE_CHUNK)
		hyper_chunk->treshOut = true;
	if (hyper_chunk->treshOut)
	{
		delete_chunk(hyper_chunk->queue[hyper_chunk->qTail], hyper_chunk);
		hyper_chunk->qTail++;
		if (hyper_chunk->qTail > MAX_ACTIVE_CHUNK)
			hyper_chunk->qTail = 0;
	}
}

void		cubeMap::chunk_deleter_add(uint32_t id[3], t_hyper_chunk *hyper_chunk)
{
	memcpy(hyper_chunk->queue[hyper_chunk->qHead], id, 3 * sizeof(uint32_t));
	hyper_chunk->qHead++;
	if (hyper_chunk->qHead > MAX_ACTIVE_CHUNK)
		hyper_chunk->qHead = 0;
}

t_renderVox	*cubeMap::get_renderGO(int32_t i, int32_t j, int32_t k)
{
	t_chunk		*chunk;
	t_renderVox	*ret;

	if (i >= MAX_CHUNK_SIZE * CHUNK_SIZE * HYPER_CHUNK_SIZE || j >= (HYPER_CHUNK_SIZE_Y * MAX_CHUNK_SIZE_Y * CHUNK_SIZE) || k >= HYPER_CHUNK_SIZE * MAX_CHUNK_SIZE * CHUNK_SIZE || i < 0 || j < 0 || k < 0)
		return (NULL);
	chunk = get_unloaded_chunk(i, j, k);
	if (chunk == NULL)
		return NULL;
	if (!chunk->meshed || !chunk->init || chunk->meshing)
		return NULL;
	ret = (t_renderVox*)dynamicMemoryManager::get_ptr(chunk->renderGO);
	return (ret);
}

void cubeMap::init_chunk_blocks(t_chunk *out, uint32_t i, uint32_t j, uint32_t k)
{
	out->init = 1;
	return ;
	if (!out->init)
	{
		for (int a = 0; a < CHUNK_SIZE + 2; a++)
			for (int b = 0; b < CHUNK_SIZE + 2; b++)
				for (int c = 0; c < CHUNK_SIZE + 2; c++)
				{
					out->blocks[a][b][c].type = E_EMPTY;
					if (a % 2 && b % 2 && c % 2)
						out->blocks[a][b][c].type = E_ROCK;	
					//out->blocks[a][b][c].type = get_id_at(a + i, b + j, c + k, 0, noise);
				}
	}
	out->init = 1;
}

void cubeMap::job_init_chunk_blocks(void *data)
{
	t_mesh_job_data mesh_data;

	mesh_data = *(t_mesh_job_data*)data;
	init_chunk_blocks(mesh_data.chunk, mesh_data.i, mesh_data.j, mesh_data.k);
}

void		cubeMap::release_video_memory(t_chunk *c)
{
	t_renderVox *rg;

	rg = (t_renderVox*)dynamicMemoryManager::get_ptr(c->renderGO);
	glDeleteBuffers(1, &(rg->vboId));
	glDeleteBuffers(1, &(rg->indiceId));
	glDeleteVertexArrays(1, &(rg->vaoId));
	rg->vaoId = 0;
}

void		cubeMap::mesh(void *data)
{
	t_mesh_job_data mesh_data;
	GLuint			*id_mem;
	float 			*v_mem;
	uint32_t		size_array;
	iso_mesher		iso_mesher;
	mesher			m;

	mesh_data = *(t_mesh_job_data*)data;
	mesh_data.chunk->meshing = 1;
	mesh_data.chunk->surface_net = surface_net;
	if (surface_net == 1)
	{
		if (!mesh_data.chunk->meshed)
		{
			((t_renderVox*)(dynamicMemoryManager::get_ptr(mesh_data.chunk->renderGO)))->program = E_ISOVOX;
			id_mem = (GLuint*)malloc(sizeof(float) * 3966309 * 3 * 4);
			v_mem = (float*)malloc(sizeof(float) * 3966309 * 3 * 4);
			iso_mesher.get_vertex(*(mesh_data.chunk), v_mem, id_mem, &size_array);
			if (size_array == 0)
			{
				free(id_mem);
				free(v_mem);
				((t_renderVox*)(dynamicMemoryManager::get_ptr(mesh_data.chunk->renderGO)))->vaoId = 0;
			}
			else
				mesher::add_request(v_mem, id_mem, size_array, (t_renderVox*)dynamicMemoryManager::get_ptr(mesh_data.chunk->renderGO));
		}
	}
	else
	{
		if (!mesh_data.chunk->meshed)
		{
			((t_renderVox*)(dynamicMemoryManager::get_ptr(mesh_data.chunk->renderGO)))->program = E_VOX;
			id_mem = (GLuint*)malloc(sizeof(float) * 3966309);
			v_mem = (float*)malloc(sizeof(float) * 3966309);
			m.get_vertex(*(mesh_data.chunk), v_mem, id_mem, &size_array);
			if (size_array == 0)
			{
				free(id_mem);
				free(v_mem);
				((t_renderVox*)(dynamicMemoryManager::get_ptr(mesh_data.chunk->renderGO)))->vaoId = 0;
			}
			else
			{
				mesher::add_request(v_mem, id_mem, size_array, (t_renderVox*)dynamicMemoryManager::get_ptr(mesh_data.chunk->renderGO));
			}
		}	
	}
	mesh_data.chunk->meshed = 1;
	mesh_data.chunk->meshing = 0;
}

void		cubeMap::create_job(t_chunk *chunk, uint32_t i, uint32_t j, uint32_t k)
{
	t_mesh_job_data jbdata;
	t_job job;
	jbdata.chunk = chunk;
	jbdata.i = (i / CHUNK_SIZE) * CHUNK_SIZE;
	jbdata.j = (j / CHUNK_SIZE) * CHUNK_SIZE;
	jbdata.k = (k / CHUNK_SIZE) * CHUNK_SIZE;
	memcpy(job.data, &jbdata, sizeof(t_mesh_job_data));
	job.fptr = cubeMap::mesh;
	if (jobHandler::slot_avalaible())
		jobHandler::push_job(job);
	else
		job.fptr(&jbdata);

}

void		cubeMap::create_job_init_chunk(t_chunk *chunk, uint32_t i, uint32_t j, uint32_t k)
{
	t_mesh_job_data jbdata;
	t_job job;
	jbdata.chunk = chunk;
	jbdata.i = (i / CHUNK_SIZE) * CHUNK_SIZE;
	jbdata.j = (j / CHUNK_SIZE) * CHUNK_SIZE;
	jbdata.k = (k / CHUNK_SIZE) * CHUNK_SIZE;
	memcpy(job.data, &jbdata, sizeof(t_mesh_job_data));
	job.fptr = cubeMap::job_init_chunk_blocks;
	if (jobHandler::slot_avalaible())
		jobHandler::push_job(job);
	else
		job.fptr(&jbdata);
}

void		cubeMap::get_renderGOs_thread(t_renderVox *out[], int32_t i[], int32_t j[], int32_t k[], uint32_t size)
{
	t_chunk			*to_init[size];
	t_chunk			*tmp;
	uint32_t		to_init_coord[size][3];
	uint32_t		count = 0;
	uint32_t		count_tmp;

	for (uint32_t c = 0; c < size; c++)
	{
		if (i[c] >= MAX_CHUNK_SIZE * CHUNK_SIZE * HYPER_CHUNK_SIZE || j[c] >= (HYPER_CHUNK_SIZE_Y * MAX_CHUNK_SIZE_Y * CHUNK_SIZE) || k[c] >= HYPER_CHUNK_SIZE * MAX_CHUNK_SIZE * CHUNK_SIZE || i[c] < 0 || j[c] < 0 || k[c] < 0)
			continue ;
		tmp = get_unloaded_chunk(i[c], j[c], k[c]);
		if ((!tmp->init || !tmp->meshed) && !tmp->meshing && count < 1024)
		{
			if (!tmp->init)
				continue ;
			to_init[count] = tmp;
			to_init_coord[count][0] = i[c];
			to_init_coord[count][1] = j[c];	
			to_init_coord[count][2] = k[c];
			tmp->meshing = 1;
			if (!tmp->init)
				create_job_init_chunk(to_init[count], to_init_coord[count][0],
					to_init_coord[count][1],
					to_init_coord[count][2]);
			count++;
		}
	}
	if (count)
	{
		while (1)
		{
			int32_t t = 0;
			for (t = 0; t < count; t++)
				if (to_init[t]->init == 0)
					break ;
			if (t == count)
				break ;
			usleep(1000);
		}
		while (count--)
		{
			create_job(to_init[count], to_init_coord[count][0],
				to_init_coord[count][1],
				to_init_coord[count][2]);
		}
	}
	mesher::exec_request();
	for (uint32_t c = 0; c < size; c++)
		out[c] = get_renderGO(i[c], j[c], k[c]);
}
