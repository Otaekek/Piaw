#include "mesher.hpp"

void				mesher::emit_vertex(int32_t i, int32_t j, int32_t k, int32_t ipos, int32_t jpos, int32_t kpos, int32_t nx, int32_t ny, int32_t nz, e_block type, uint32_t tcoord, uint32_t tcoordy)
{
	float ex[9];

	ex[0] = ex_table[nx + 1][ny + 1][nz + 1][0][0] / 2;
	ex[1] = ex_table[nx + 1][ny + 1][nz + 1][0][1] / 2;
	ex[2] = ex_table[nx + 1][ny + 1][nz + 1][0][2] / 2;

	ex[3] = ex_table[nx + 1][ny + 1][nz + 1][1][0] / 2;
	ex[4] = ex_table[nx + 1][ny + 1][nz + 1][1][1] / 2;
	ex[5] = ex_table[nx + 1][ny + 1][nz + 1][1][2] / 2;

	ex[6] = normal_table[nx + 1][ny + 1][nz + 1][0] / CHUNK_SIZE / 2;
	ex[7] = normal_table[nx + 1][ny + 1][nz + 1][1] / CHUNK_SIZE / 2;
	ex[8] = normal_table[nx + 1][ny + 1][nz + 1][2] / CHUNK_SIZE / 2;

	if (nx > 0 || ny > 0 || nz < 0)
	{
		indice_array[6 * (data_array_index / 4)] = data_array_index + 0;
		indice_array[6 * (data_array_index / 4) + 1] = data_array_index + 1;
		indice_array[6 * (data_array_index / 4) + 2] = data_array_index + 2;
		indice_array[6 * (data_array_index / 4) + 3] = data_array_index + 2;
		indice_array[6 * (data_array_index / 4) + 4] = data_array_index + 3;
		indice_array[6 * (data_array_index / 4) + 5] = data_array_index + 0;
	}
	else
	{
		indice_array[6 * (data_array_index / 4) + 5] = data_array_index + 0;
		indice_array[6 * (data_array_index / 4) + 4] = data_array_index + 1;
		indice_array[6 * (data_array_index / 4) + 3] = data_array_index + 2;
		indice_array[6 * (data_array_index / 4) + 2] = data_array_index + 2;
		indice_array[6 * (data_array_index / 4) + 1] = data_array_index + 3;
		indice_array[6 * (data_array_index / 4) + 0] = data_array_index + 0;		
	}
	data_array[0 + data_array_index * 12] = vertex_table[i][j][k][0] + ex[0] + ex[3] + ex[6];
	data_array[1 + data_array_index * 12] = vertex_table[i][j][k][1] + ex[1] + ex[4] + ex[7];
	data_array[2 + data_array_index * 12] = vertex_table[i][j][k][2] + ex[2] + ex[5] + ex[8];
	data_array[3 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][0];
	data_array[4 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][1];
	data_array[5 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][2];
	data_array[6 + data_array_index * 12] = 0;
	data_array[7 + data_array_index * 12] = 0;
	data_array[8 + data_array_index * 12] = (float)tex_table[nx + 1][ny + 1][nz + 1][(int)type-1];

	data_array_index++;

	data_array[0 + data_array_index * 12] = vertex_table[i][j][k][0] + ex[0] - ex[3] + ex[6];
	data_array[1 + data_array_index * 12] = vertex_table[i][j][k][1] + ex[1] - ex[4] + ex[7];
	data_array[2 + data_array_index * 12] = vertex_table[i][j][k][2] + ex[2] - ex[5] + ex[8];
	data_array[3 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][0];
	data_array[4 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][1];
	data_array[5 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][2];
	data_array[6 + data_array_index * 12] = 0;
	data_array[7 + data_array_index * 12] = tcoordy;
	data_array[8 + data_array_index * 12] = (float)tex_table[nx + 1][ny + 1][nz + 1][(int)type-1];

	data_array_index++;

	data_array[0 + data_array_index * 12] = vertex_table[ipos][jpos][kpos][0] - ex[0] - ex[3] + ex[6];
	data_array[1 + data_array_index * 12] = vertex_table[ipos][jpos][kpos][1] - ex[1] - ex[4] + ex[7];
	data_array[2 + data_array_index * 12] = vertex_table[ipos][jpos][kpos][2] - ex[2] - ex[5] + ex[8];
	data_array[3 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][0];
	data_array[4 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][1];
	data_array[5 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][2];
	data_array[6 + data_array_index * 12] = tcoord;
	data_array[7 + data_array_index * 12] = tcoordy;
	data_array[8 + data_array_index * 12] = (float)tex_table[nx + 1][ny + 1][nz + 1][(int)type-1];
	data_array_index++;

	data_array[0 + data_array_index * 12] = vertex_table[ipos][jpos][kpos][0] - ex[0] + ex[3] + ex[6];
	data_array[1 + data_array_index * 12] = vertex_table[ipos][jpos][kpos][1] - ex[1] + ex[4] + ex[7];
	data_array[2 + data_array_index * 12] = vertex_table[ipos][jpos][kpos][2] - ex[2] + ex[5] + ex[8];
	data_array[3 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][0];
	data_array[4 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][1];
	data_array[5 + data_array_index * 12] = normal_table[nx + 1][ny + 1][nz + 1][2];
	data_array[6 + data_array_index * 12] = tcoord;
	data_array[7 + data_array_index * 12] = 0;
	data_array[8 + data_array_index * 12] = (float)tex_table[nx + 1][ny + 1][nz + 1][(int)type-1];

	data_array_index++;
}
