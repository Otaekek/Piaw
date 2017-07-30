#pragma once

#include "cubeMap.class.hpp"
#include <renderDataSys.class.hpp>
#include <mesher.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <transform.class.hpp>

typedef struct {
	float x,y,z;
} float3D;
typedef struct {
	int a,b,c;
} int3D;
typedef struct {
	int np,nt;
	float3D *p;
	int3D *t;
} Mesh;

class iso_mesher {

public:

	iso_mesher();
	~iso_mesher();

	int					mesh(float *sample, float *mem, GLuint *m, t_renderVox *out);
	int					get_vertex(t_chunk & chunk, float *mem, GLuint *m, uint32_t *outv);
	static void			create_vao(float *v, GLuint *ind, GLuint inds, t_renderVox *out);
	static void			init_table();
	uint32_t			convert_to_non_indexed(Mesh *mesh);

private:

	static	int cube_edges[24];
	static	int edge_table[256];
	static void			transvox(t_block *data, int *dims, float level, Mesh *mesh);

	int				buffer[4096];

	uint32_t		vaoId;

	t_chunk			chunk;

	uint32_t		data_array_index;

	GLuint			*indice_array;
	float			*data_array;
	int				c_type;
};
