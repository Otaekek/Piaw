#include "surfaceNetMesher.hpp"
#include <gtc/quaternion.hpp>

int iso_mesher::cube_edges[24];

int iso_mesher::edge_table[256];

static const glm::vec3 normalList[8] = {{-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1}, {1, -1, -1}, {1, -1, 1}, {1, 1, -1}, {1, 1, 1}};

//#define USE_SHARP_NORMAL

//uint32_t	iso_mesher::tex_table[20];

iso_mesher::iso_mesher()
{

}

iso_mesher::~iso_mesher()
{

}

float3D				deduce_normal(float3D v1, float3D v2, float3D v3)
{
	glm::vec3 c1, c2, c3, t1, t2, norm;

	c1.x = v1.x;
	c1.y = v1.y;
	c1.z = v1.z;
	c2.x = v2.x;
	c2.y = v2.y;
	c2.z = v2.z;
	c3.x = v3.x;
	c3.y = v3.y;
	c3.z = v3.z;

	t1 = c2 - c1;
	t2 = c3 - c1;
	
	/*
		Set Vector U to (Triangle.p2 minus Triangle.p1)
	Set Vector V to (Triangle.p3 minus Triangle.p1)

	Set Normal.x to (multiply U.y by V.z) minus (multiply U.z by V.y)
	Set Normal.y to (multiply U.z by V.x) minus (multiply U.x by V.z)
	Set Normal.z to (multiply U.x by V.y) minus (multiply U.y by V.x)
	*/
	norm.x = (t1.y * t2.z) - (t1.z * t2.y);
	norm.y = (t1.z * t2.x) - (t1.x * t2.z);
	norm.z = (t1.x * t2.y) - (t1.y * t2.x);
	norm = glm::normalize(norm);
	return float3D {norm.x, norm.y, norm.z};
}

int					iso_mesher::mesh(float *sample, float *mem, GLuint *m, t_renderVox *out)
{
	// To implement
}

int					iso_mesher::get_vertex(t_chunk & chunk, float *mem, GLuint *m, uint32_t *outv)
{
	Mesh mesh;

	t_chunk y;

	for (int a = 0; a < CHUNK_SIZE + 2; a++)
		for (int b = 0; b < CHUNK_SIZE + 2; b++)
			for (int c = 0; c < CHUNK_SIZE + 2; c++)
				y.blocks[c][b][a] = chunk.blocks[a][b][c];
	int dims[3] = {CHUNK_SIZE + 2, CHUNK_SIZE + 2, CHUNK_SIZE + 2};
	mesh.p = (float3D*)mem;
	mesh.t = (int3D*)m;

	transvox((t_block*)y.blocks, dims, 50, &mesh);
	*outv = mesh.np;

	convert_to_non_indexed(&mesh);	
	return (1);	
}

void				iso_mesher::create_vao(float *v, GLuint *ind, GLuint inds, t_renderVox *out)
{

}

void				iso_mesher::init_table()
{
	int i,j,p,em,k = 0;
	for(i=0; i<8; ++i) {
		for(j=1; j<=4; j=j<<1) {
			p = i^j;
			if(i <= p) {
				cube_edges[k++] = i;
				cube_edges[k++] = p;
			}
		}
	}
	for(i=0; i<256; ++i) {
		em = 0;
		for(j=0; j<24; j+=2) {
			int a = !(i & (1<<cube_edges[j]));
			int b = !(i & (1<<cube_edges[j+1]));
			em |= a != b ? (1 << (j >> 1)) : 0;
		}
		edge_table[i] = em;
	}
}

void getUvs(float3D v1, float3D v2, float3D v3, float3D vnorm, glm::vec2 out[3])
{
	glm::quat rotation;
	const float scalF = 10.5;
	glm::vec3 a, b, c, normal;

	a.x = v1.x;
	a.y = v1.y;
	a.z = v1.z;
	b.x = v2.x;
	b.y = v2.y;
	b.z = v2.z;
	c.x = v3.x;
	c.y = v3.y;
	c.z = v3.z;
	normal.x = vnorm.x;
	normal.y = vnorm.y;
	normal.z = vnorm.z;

	rotation = glm::inverse(transformBuiltin::LookAt(-normal));

	a = rotation * a;
	b = rotation * b;
	c = rotation * c;
	out[0] = glm::vec2(a.x, a.y) * scalF;
	out[1] = glm::vec2(b.x, b.y) * scalF;
	out[2] = glm::vec2(c.x, c.y) * scalF;
}

void			sum(float3D *a, float3D b)
{
	a->x += b.x;
	a->y += b.y;
	a->z += b.z;
}

void	__attribute__ ((hot))	iso_mesher::transvox(t_block *data, int *dims, float level, Mesh *mesh) 
{
	float3D *vertices=mesh->p;
	int3D *faces=mesh->t;
	int n = 0;
	float x[3];
	int R[3];
	glm::vec2 uv[3];
	float grid[8] = {0};
	int buf_no = 1;
	int buffer_length=0;
	int vertices_length=0;
	int faces_length=0;
	int	i,j,k;
	unsigned char type[8];
	R[0]=1;
	R[1]=dims[0]+1;
	R[2]=(CHUNK_SIZE+3)*(CHUNK_SIZE+3);
	int buffer[(CHUNK_SIZE+3)*(CHUNK_SIZE+3) * 20] = {0};
 
	for(x[2]=0; x[2]<dims[2]-1; ++x[2])
	{
		int m = 1 + (dims[0]+1) * (1 + buf_no * (dims[1]+1));
		for(x[1]=0; x[1]<dims[1]-1; ++x[1], ++n, m+=2)
		for(x[0]=0; x[0]<dims[0]-1; ++x[0], ++n, ++m)
		{
			int mask = 0, g = 0, idx = n;
			for(k=0; k<2; ++k, idx += dims[0]*(dims[1]-2))
			for(j=0; j<2; ++j, idx += dims[0]-2)      
			for(i=0; i<2; ++i, ++g, ++idx)
			{
				float p = -level;
				if (data[idx].type != E_EMPTY)
					p += 100;
				type[g] = data[idx].type;
				grid[g] = p;
				mask |= (p < 0) ? (1<<g) : 0;
			}
			if(mask == 0 || mask == 0xff)
				continue;
			int edge_mask = edge_table[mask];
			float3D v = {0.0,0.0,0.0};
			int e_count = 0;
			for(i=0; i<12; ++i)
			{
				if(!(edge_mask & (1<<i)))
					continue;
				++e_count;
				int e0 = cube_edges[ i<<1 ];       //Unpack vertices
				int e1 = cube_edges[(i<<1)+1];
				float g0 = grid[e0];                 //Unpack grid values
				float g1 = grid[e1];
				float t  = g0 - g1;                  //Compute point of intersection
				if(fabs(t) > 1e-6)
					t = g0 / t;
				else
					continue;
				k=1;
				for(j=0; j<3; ++j)
				{
					int a = e0 & k;
					int b = e1 & k;
					if(a != b)
						((float*)&v)[j] += a ? 1.0 - t : t;
					else
						((float*)&v)[j] += a ? 1.0 : 0;
					k=k<<1;
				}
			}
			float s = 1.0F / e_count;

			for(i=0; i<3; ++i)
				((float*)&v)[i] = (x[i] + s * (((float*)&v)[i])) / (CHUNK_SIZE) - 0.5f;
			buffer[m] = vertices_length / 4;
			float tmp = v.x;
		//	v.x = v.z;
		//	v.z = tmp;
			vertices[vertices_length++]=v;
			vertices_length += 2;
			memcpy(&vertices[vertices_length], &type, sizeof(*type) * 8);
			vertices_length++;
			for(i=0; i<3; ++i)
			{
				if(!(edge_mask & (1<<i)) )
					continue;
				int iu = (i+1)%3;
				int iv = (i+2)%3;
				if(x[iu] == 0 || x[iv] == 0)
					continue;
				int du = R[iu];
				int dv = R[iv];
				float3D v1, v2, v3, norm;

				if(mask & 1)
				{
					v1 = vertices[(buffer[m]) * 4];
					v2 = vertices[(buffer[m - du - dv]) * 4];
					v3 = vertices[(buffer[m - du]) * 4];
					norm = deduce_normal(v1, v2, v3);

					sum(&vertices[(buffer[m]) * 4 + 1], norm);
					sum(&vertices[(buffer[m - du - dv]) * 4 + 1], norm);
					sum(&vertices[(buffer[m - du]) * 4 + 1], norm);
					faces[faces_length++]=(int3D){buffer[m], buffer[m-du-dv], buffer[m-du]};
					
					v1 = vertices[(buffer[m]) * 4];
					v2 = vertices[(buffer[m - dv]) * 4];
					v3 = vertices[(buffer[m - du - dv]) * 4];

					norm = deduce_normal(v1, v2, v3);

					sum(&vertices[(buffer[m]) * 4 + 1], norm);
					sum(&vertices[(buffer[m - dv]) * 4 + 1], norm);
					sum(&vertices[(buffer[m - du - dv]) * 4 + 1], norm);

					faces[faces_length++]=(int3D){buffer[m], buffer[m-dv], buffer[m-du-dv]};
				}
				else
				{
					v1 = vertices[(buffer[m]) * 4];
					v2 = vertices[(buffer[m - du - dv]) * 4];
					v3 = vertices[(buffer[m - dv]) * 4];

					norm = deduce_normal(v1, v2, v3);
					sum(&vertices[(buffer[m]) * 4 + 1], norm);
					sum(&vertices[(buffer[m - du - dv]) * 4 + 1], norm);
					sum(&vertices[(buffer[m - dv]) * 4 + 1], norm);

					faces[faces_length++]=(int3D){buffer[m], buffer[m-du-dv], buffer[m-dv]};
					
					
					v1 = vertices[(buffer[m]) * 4];
					v2 = vertices[(buffer[m - du]) * 4];
					v3 = vertices[(buffer[m - dv - du]) * 4];
					norm = deduce_normal(v1, v2, v3);
					sum(&vertices[(buffer[m]) * 4 + 1], norm);
					sum(&vertices[(buffer[m - du]) * 4 + 1], norm);
					sum(&vertices[(buffer[m - dv - du]) * 4 + 1], norm);
					faces[faces_length++]=(int3D){buffer[m], buffer[m-du], buffer[m-du-dv]};
				}
			}
		}
		usleep(200);
		n+=dims[0];
		buf_no ^= 1;
		R[2]=-R[2];
	}
	mesh->np=vertices_length * 2;
	mesh->nt=faces_length;
}

void	 normalize_float3d(float3D *a)
{
	a->y += 0.00000001;
	float b = sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
	a->x /= b;
	a->y /= b;
	a->z /= b;
}

void		load_material(float3D *final, float3D normal, unsigned char type[8], GLubyte mats[12])
{
	float3D n = normal;
	bool reach = false;
	float sum = 0;
	glm::vec3 k = {normal.x, normal.y, normal.z};

	float max = 0;
	int addr;
	for (int i = 0; i < 3; i++)
	{
		if ((fabs(((float*)&n)[i])) > max)
		{
			max = fabs(((float*)&n)[i]);
			addr = i;
		}
	}
	for (int i = 0; i < 3; i++)
		if (i != addr)
			((float*)&n)[i] = 0;		
	for (int i = 0; i < 8; i++)
	{
		if (type[i] <= E_EMPTY || type[i] >= E_LAST)
			continue ;
		sum += 1;
	}
	bzero(mats, sizeof(GLubyte) * 12);
	for (int i = 0; i < 8; i++)
	{
		if (type[i] <= E_EMPTY || type[i] >= E_LAST)
			continue ;
		int t = mesher::tex_table[1 + (int)roundf(n.x)][1 + (int)roundf(n.y)][1 + (int)roundf(n.z)][type[i] - 1];
		mats[t] += (255.0f) / sum;
	}
}

uint32_t iso_mesher::convert_to_non_indexed(Mesh *mesh)
{
	float3D *final = (float3D*)malloc(mesh->nt * 27 * sizeof(float3D));
	GLubyte mats[12] = {0};
 	float3D n;
	glm::vec2 out[3];

	for (int i = 0; i < mesh->nt; i++)
	{

		// position
		final[i * 12 + 0] = mesh->p[mesh->t[i].a * 4];
		final[i * 12 + 4] = mesh->p[mesh->t[i].b * 4];
		final[i * 12 + 8] = mesh->p[mesh->t[i].c * 4];

		n = deduce_normal(final[i * 12], final[i * 12 + 4], final[i * 12 + 8]);
		getUvs(final[i * 12], final[i * 12 + 4], final[i * 12 + 8], n, out);

		final[i * 12 + 1] = n;
		final[i * 12 + 5] = n;
		final[i * 12 + 9] = n;


	#ifndef USE_SHARP_NORMAL		
		final[i * 12 + 1] = mesh->p[mesh->t[i].a * 4 + 1];
		final[i * 12 + 5] = mesh->p[mesh->t[i].b * 4 + 1];
		final[i * 12 + 9] = mesh->p[mesh->t[i].c * 4 + 1];

		normalize_float3d(&final[i * 12 + 1]);
		normalize_float3d(&final[i * 12 + 5]);
		normalize_float3d(&final[i * 12 + 9]);
		
	#endif


//		final[i * 12 + 2].z = mesher::tex_table[1 + (int)roundf(final[i * 12 + 1].x)][1 + (int)roundf(final[i * 12 + 1].y)][1 + (int)roundf(final[i * 12 + 1].z)][(int)mesh->p[mesh->t[i].a * 4 + 2].z];
//		mats[(int)final[i * 12 + 2].z] = 255;
//		if ((int)final[i * 12 + 2].z == 0)
//			printf("%d %d %d %f %f %f\n", 1 + (int)roundf(final[i * 12 + 1].x), 1 + (int)roundf(final[i * 12 + 1].y), 1 + (int)roundf(final[i * 12 + 1].z),final[i * 12 + 1].x,final[i * 12 + 1].y,final[i * 12 + 1].z);
		load_material(&final[i * 12 + 2], final[i * 12 + 1], (unsigned char*)&(mesh->p[mesh->t[i].a * 4 + 3]), mats);
		memcpy(&final[i * 12 + 3], mats, sizeof(mats));
		bzero(mats, sizeof(mats));

//		final[i * 12 + 6].z = mesher::tex_table[1 + (int)roundf(final[i * 12 + 5].x)][1 + (int)roundf(final[i * 12 + 5].y)][1 + (int)roundf(final[i * 12 + 5].z)][(int)mesh->p[mesh->t[i].b * 4 + 2].z];
//		mats[(int)final[i * 12 + 6].z] = 255;
		load_material(&final[i * 12 + 6], final[i * 12 + 5], (unsigned char*)&(mesh->p[mesh->t[i].b * 4 + 3]), mats);
		memcpy(&final[i * 12 + 7], mats, sizeof(mats));
		bzero(mats, sizeof(mats));

//		final[i * 12 + 10].z = mesher::tex_table[1 + (int)roundf(final[i * 12 + 9].x)][1 + (int)roundf(final[i * 12 + 9].y)][1 + (int)roundf(final[i * 12 + 9].z)][(int)mesh->p[mesh->t[i].c * 4 + 2].z];
//		mats[(int)final[i * 12 + 10].z] = 255;
		load_material(&final[i * 12 + 10], final[i * 12 + 9], (unsigned char*)&(mesh->p[mesh->t[i].c * 4 + 3]), mats);
		memcpy(&final[i * 12 + 11], mats, sizeof(mats));
		bzero(mats, sizeof(mats));

		final[i * 12 + 2].x = out[0].x;
		final[i * 12 + 2].y = out[0].y;
		
		final[i * 12 + 6].x = out[1].x;
		final[i * 12 + 6].y = out[1].y;

		final[i * 12 + 10].x = out[2].x;
		final[i * 12 + 10].y = out[2].y;


		mesh->t[i].a = i * 3 + 0;
		mesh->t[i].b = i * 3 + 1;
		mesh->t[i].c = i * 3 + 2;
	}
	memcpy(mesh->p, final, mesh->nt * 9 * 4 * sizeof(float));
	free(final);
}
