#include "mesher.hpp"
#include <mutex>

std::mutex mesher_mut;

float		mesher::vertex_table[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE][3];
float		mesher::uv_table[4 * 2] = {1, 0, 1, 1, 1, 0, 0, 0};
float		mesher::normal_table[3][3][3][3];
float		mesher::ex_table[3][3][3][2][3];
uint32_t	mesher::tex_table[3][3][3][20];
GLuint		mesher::*indice_array;


t_vox_request	mesher::requests[65536];
uint32_t	mesher::request_count = 0;
	
static uint32_t ntable[3][3][3];

void mesher::init_text()
{
	tex_table[1][1][0][0] = 0;
	tex_table[1][1][2][0] = 0;
	tex_table[1][0][1][0] = 0;
	tex_table[1][2][1][0] = 1;
	tex_table[0][1][1][0] = 0;
	tex_table[2][1][1][0] = 0;

	tex_table[1][1][0][1] = 2;
	tex_table[1][1][2][1] = 2;
	tex_table[1][0][1][1] = 2;
	tex_table[1][2][1][1] = 3;
	tex_table[0][1][1][1] = 2;
	tex_table[2][1][1][1] = 2;

	tex_table[1][1][0][2] = 4;
	tex_table[1][1][2][2] = 4;
	tex_table[1][0][1][2] = 4;
	tex_table[1][2][1][2] = 4;
	tex_table[0][1][1][2] = 4;
	tex_table[2][1][1][2] = 4;

	tex_table[1][1][0][3] = 5;
	tex_table[1][1][2][3] = 5;
	tex_table[1][0][1][3] = 5;
	tex_table[1][2][1][3] = 6;
	tex_table[0][1][1][3] = 5;
	tex_table[2][1][1][3] = 5;

	tex_table[1][1][0][4] = 7;
	tex_table[1][1][2][4] = 7;
	tex_table[1][0][1][4] = 8;
	tex_table[1][2][1][4] = 8;
	tex_table[0][1][1][4] = 7;
	tex_table[2][1][1][4] = 7;

	tex_table[1][1][0][5] = 9;
	tex_table[1][1][2][5] = 9;
	tex_table[1][0][1][5] = 9;
	tex_table[1][2][1][5] = 9;
	tex_table[0][1][1][5] = 9;
	tex_table[2][1][1][5] = 9;

	tex_table[1][1][0][6] = 10;
	tex_table[1][1][2][6] = 10;
	tex_table[1][0][1][6] = 10;
	tex_table[1][2][1][6] = 10;
	tex_table[0][1][1][6] = 10;
	tex_table[2][1][1][6] = 10;

}

void mesher::init_table()
{
	init_text();
	for (float i = 0; i < CHUNK_SIZE; i++)
		for (float j = 0; j < CHUNK_SIZE; j++)
			for (float k = 0; k < CHUNK_SIZE; k++)
			{
				vertex_table[(int)i][(int)j][(int)k][0] = (float)(i) / CHUNK_SIZE - 0.5f;
				vertex_table[(int)i][(int)j][(int)k][1] = (float)(j) / CHUNK_SIZE - 0.5f;
				vertex_table[(int)i][(int)j][(int)k][2] = (float)(k) / CHUNK_SIZE - 0.5f;
			}
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
			{
				normal_table[i][j][k][0] = i - 1; 
				normal_table[i][j][k][1] = j - 1;
				normal_table[i][j][k][2] = k - 1;
				ntable[i][j][k] = i * 3 * 3 + j * 3 + k;
			}

	// RIGHT
	ex_table[1][1][0][0][0] = -1.0f / CHUNK_SIZE;
	ex_table[1][1][0][0][1] = 0;
	ex_table[1][1][0][0][2] = 0;

	ex_table[1][1][2][0][0] = -1.0f / CHUNK_SIZE;
	ex_table[1][1][2][0][1] = 0;
	ex_table[1][1][2][0][2] = 0;

	ex_table[0][1][1][0][0] = 0;
	ex_table[0][1][1][0][1] = 0;
	ex_table[0][1][1][0][2] = -1.0f / CHUNK_SIZE;

	ex_table[2][1][1][0][0] = 0;
	ex_table[2][1][1][0][1] = 0;
	ex_table[2][1][1][0][2] = -1.0f / CHUNK_SIZE;

	ex_table[1][0][1][1][0] = 1.0f / CHUNK_SIZE;
	ex_table[1][0][1][1][1] = 0;
	ex_table[1][0][1][1][2] = 0;

	ex_table[1][2][1][1][0] = 1.0f / CHUNK_SIZE;
	ex_table[1][2][1][1][1] = 0;
	ex_table[1][2][1][1][2] = 0;


	// UP
	ex_table[1][1][0][1][0] = 0;
	ex_table[1][1][0][1][1] = -1.0f / CHUNK_SIZE;
	ex_table[1][1][0][1][2] = 0;

	ex_table[1][1][2][1][0] = 0;
	ex_table[1][1][2][1][1] = -1.0f / CHUNK_SIZE;
	ex_table[1][1][2][1][2] = 0;

	ex_table[0][1][1][1][0] = 0;
	ex_table[0][1][1][1][1] = -1.0f / CHUNK_SIZE;
	ex_table[0][1][1][1][2] = 0;

	ex_table[2][1][1][1][0] = 0;
	ex_table[2][1][1][1][1] = -1.0f / CHUNK_SIZE;
	ex_table[2][1][1][1][2] = 0;

	ex_table[1][0][1][0][0] = 0;
	ex_table[1][0][1][0][1] = 0;
	ex_table[1][0][1][0][2] = -1.0f / CHUNK_SIZE;

	ex_table[1][2][1][0][0] = 0;
	ex_table[1][2][1][0][1] = 0;
	ex_table[1][2][1][0][2] = -1.0f / CHUNK_SIZE;
}

mesher::mesher() :face_done{0}
{
	data_array_index = 0;
}

mesher::~mesher()
{

}

bool				mesher::is_occluded(int32_t i, int32_t j, int32_t k, int32_t nx, int32_t ny, int32_t nz)
{
	if (i + nx >= CHUNK_SIZE || j + ny >= CHUNK_SIZE || k + nz >= CHUNK_SIZE || k + nz < 0 || i + nx < 0 || j + ny < 0)
		return false;
	return (chunk.blocks[i + nx][j + ny][k + nz].type != E_EMPTY);
}

void				mesher::propag_meshx(int32_t i, int32_t j, int32_t k, int32_t nx, int32_t ny, int32_t nz, int32_t initi,  int32_t inity, int32_t initz)
{
	if (face_done[ntable[nx + 1][ny + 1][nz + 1]][i + 1][j][k])
		return ; 
	face_done[ntable[nx + 1][ny + 1][nz + 1]][i][j][k] = true;
	if (chunk.blocks[i][j][k].type != c_type || i + 1 > CHUNK_SIZE)
	{
		emit_vertex(initi, inity, initz, i - 1, j, k, nx, ny, nz, chunk.blocks[i - 1][j][k].type, i - 1 - initi + 1, 1);
		return ;
	}
	propag_meshx(i + 1, j, k, nx, ny, nz, initi, inity, initz);
}

void				mesher::propag_meshy(int32_t i, int32_t j, int32_t k, int32_t nx, int32_t ny, int32_t nz, int32_t initi,  int32_t inity, int32_t initz)
{
	if (face_done[ntable[nx + 1][ny + 1][nz + 1]][i][1 + j][k])
		return ;
	face_done[ntable[nx + 1][ny + 1][nz + 1]][i][j][k] = true;
	if (chunk.blocks[i][j][k].type != c_type || j + 1 > CHUNK_SIZE)
	{
				face_done[ntable[nx + 1][ny + 1][nz + 1]][i][j][k] = false;

		emit_vertex(initi, inity, initz, i, j - 1, k, nx, ny, nz, chunk.blocks[i][j - 1][k].type, 0, 0);
		return ;
	}
	propag_meshy(i, j + 1, k, nx, ny, nz, initi, inity, initz);

}

void				mesher::propag_meshz(int32_t i, int32_t j, int32_t k, int32_t nx, int32_t ny, int32_t nz, int32_t initi,  int32_t inity, int32_t initz)
{
	if (face_done[ntable[nx + 1][ny + 1][nz + 1]][i][j][1 + k])
		return ;
	face_done[ntable[nx + 1][ny + 1][nz + 1]][i][j][k] = true;
	if (chunk.blocks[i][j][k].type != c_type || k + 1 > CHUNK_SIZE)
	{
		face_done[ntable[nx + 1][ny + 1][nz + 1]][i][j][k] = false;
		emit_vertex(initi, inity, initz, i, j, k - 1 , nx, ny, nz, chunk.blocks[i][j][k - 1].type,  k - 1 - initz + 1, 1);
		return ;
	}
	propag_meshz(i, j, k + 1, nx, ny, nz, initi, inity, initz);

}

void				mesher::flush_data(t_renderVox *out)
{
	GLuint 	vertexBufferID;

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(1, &(vertexBufferID));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER,  data_array_index * sizeof(float) * 9, data_array, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float)*6));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float)*8));

	out->vaoId = vaoId;
	out->vboId = vertexBufferID;
	out->numVertex = (data_array_index / 4) * 6;

	GLuint indiceBufferId;
	glGenBuffers(1, &(indiceBufferId));
	out->indiceId = indiceBufferId;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  out->numVertex * sizeof(GLuint), indice_array, GL_STATIC_DRAW);
}

int			mesher::mesh(t_chunk & pchunk, float *mem, GLuint *mem_ind, t_renderVox *out)
{
	chunk = pchunk;
	data_array = mem;
	indice_array = mem_ind;
	for (c_type = E_GRASS; c_type != E_LAST; c_type += 1)
	{
		for (int i = 0; i < CHUNK_SIZE; i++)
			for (int j = 0; j < CHUNK_SIZE; j++)
				for (int k = 0; k < CHUNK_SIZE; k++)
				{
					if (chunk.blocks[i][j][k].type != c_type)
						continue ;
					if (!is_occluded(i, j, k, 1, 0, 0))
						propag_meshz(i, j, k, 1, 0, 0, i, j, k);
					if (!is_occluded(i, j, k, -1, 0, 0))
						propag_meshz(i, j, k, -1, 0, 0, i, j, k);
					if (!is_occluded(i, j, k, 0, 1, 0))
						propag_meshz(i, j, k, 0, 1, 0, i, j, k);
					if (!is_occluded(i, j, k, 0, -1, 0))
						propag_meshz(i, j, k, 0, -1, 0, i, j, k);
					if (!is_occluded(i, j, k, 0, 0, 1))
						propag_meshx(i, j, k, 0, 0, 1, i, j, k);
					if (!is_occluded(i, j, k, 0, 0, -1))
						propag_meshx(i, j, k, 0, 0, -1, i, j, k);
			}
			bzero(face_done, sizeof(face_done));
	}
	if (data_array_index == 0)
		return (0);
	flush_data(out);
	return (1);
}

void			mesher::create_vao(float *data_array, GLuint *indice_array, GLuint data_array_index, t_renderVox *out)
{
	GLuint 	vertexBufferID;
	GLuint	vaoId;

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(1, &(vertexBufferID));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER,  data_array_index * sizeof(float) * 12, data_array, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(sizeof(float)*6));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(sizeof(float)*8));

	for (int i = 0; i < 12; i++) {
		glEnableVertexAttribArray(4 + i);
		glVertexAttribPointer(4 + i, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(float) * 12, (void*)(sizeof(float)*(9) + i));
	}
	out->vaoId = vaoId;
	out->vboId = vertexBufferID;
	out->numVertex = (data_array_index / 4) * 6;
	GLuint indiceBufferId;
	glGenBuffers(1, &(indiceBufferId));
	out->indiceId = indiceBufferId;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  out->numVertex * sizeof(GLuint) / (1 + (out->program == E_ISOVOX)), indice_array, GL_STATIC_DRAW);
	free(data_array);
	free(indice_array);
}

int				mesher::get_vertex(t_chunk & pchunk, float *mem, GLuint *mem_ind, uint32_t *outv)
{
	chunk = pchunk;
	data_array = mem;
	indice_array = mem_ind;
	for (c_type = E_GRASS; c_type != E_LAST; c_type += 1)
	{
		for (int i = 0; i < CHUNK_SIZE; i++)
			for (int j = 0; j < CHUNK_SIZE; j++)
				for (int k = 0; k < CHUNK_SIZE; k++)
				{
					if (chunk.blocks[i][j][k].type != c_type)
						continue ;
					if (!is_occluded(i, j, k, 1, 0, 0))
						propag_meshz(i, j, k, 1, 0, 0, i, j, k);
					if (!is_occluded(i, j, k, -1, 0, 0))
						propag_meshz(i, j, k, -1, 0, 0, i, j, k);
					if (!is_occluded(i, j, k, 0, 1, 0))
						propag_meshz(i, j, k, 0, 1, 0, i, j, k);
					if (!is_occluded(i, j, k, 0, -1, 0))
						propag_meshz(i, j, k, 0, -1, 0, i, j, k);
					if (!is_occluded(i, j, k, 0, 0, 1))
						propag_meshx(i, j, k, 0, 0, 1, i, j, k);
					if (!is_occluded(i, j, k, 0, 0, -1))
						propag_meshx(i, j, k, 0, 0, -1, i, j, k);
			}
			bzero(face_done, sizeof(face_done));
	}
	*outv = data_array_index;
	return 1;
}

void		mesher::add_request(float *v, GLuint *ind, GLuint inds, t_renderVox *out)
{
	mesher_mut.lock();
	t_vox_request request;

	request.vertex = v;
	request.indices = ind;
	request.index = inds;
	request.vox = out;
	requests[request_count++] = request;
	if (request_count > 65536)
		request_count = 0;
	mesher_mut.unlock();
}

void		mesher::exec_request()
{
	mesher_mut.lock();
	while (request_count--)
		create_vao(requests[request_count].vertex, requests[request_count].indices, requests[request_count].index, requests[request_count].vox);
	request_count = 0;
	mesher_mut.unlock();
}
