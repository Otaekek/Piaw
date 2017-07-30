#include "cubeMap.class.hpp"

e_block cave(uint32_t x, uint32_t y, uint32_t z, uint32_t seedz, t_noise *noise)
{
	float a = noise3(noise, (float)x / CHUNK_SIZE,(float)y/ CHUNK_SIZE,(float)z / CHUNK_SIZE);
	a /= 2;
	a += 0.5;
	a *= 16;
	if (a  + 2 < y % CHUNK_SIZE || a - 2 > y % CHUNK_SIZE)
		return (E_DIRT);
	return (E_EMPTY);
}

e_block hill(uint32_t x, uint32_t y, uint32_t z, uint32_t seedz, t_noise *noise)
{
	/*
		float da = noise2(noise, (float)(x % CHUNK_SIZE) * 30 ,(float)(z % CHUNK_SIZE) * 30);
	da *= 5;
	
	if (y % CHUNK_SIZE < da)
		return E_GRASS;
	return E_EMPTY;
	if (!((x) % 16) &&!(y % 16)&&!(z % 16))
	{
		return E_GRASS;
	}
	return E_EMPTY;*/
	float a = noise2(noise, (float)x / 300,(float)z/300);
	a /= 2;
	a += 0.5;
	a *= 7;
	float r = noise2(noise, (float)(x + 78) / 150,(float)(z + 98)/150);
	r /= 2;
	r += 0.5;
	r *= 9;
	float b = noise2(noise, (float)x / 100,(float)z/100);
	b *= 5;

	float t = noise2(noise, (float)x / 100,(float)z/100);
	t *= 8;

	float c = noise2(noise, (float)x / 10,(float)z/10);
	c *= 1;

	float d = noise2(noise, (float)x * 30 ,(float)z * 30);
	d *= 5;
	
	float u = noise2(noise, (float)(x / 7) ,(float)(z / 7));
	u *= 50;
	float arbre = noise2(noise, (float)(x / 7),(float)(z / 7));
	arbre += 0.5;
	arbre *= 12;
	a = c + a + b + r;
	a /= 3;
	a = pow(a, 2.5);
	a -= t - 4;
//	a -= t - 5;
	y -= 4;
	if (y < 257)
		return E_ROCK;
	if (y % (8 * CHUNK_SIZE) < (a))// || y == 257)
	{
		if (((int)b  << (int)d ^ (int)r) > 148)
			return (E_ROCK);
		if (y % (8 * CHUNK_SIZE) > CHUNK_SIZE / 2 + d + 10)
			return (E_SNOW);
		if (y % (8 * CHUNK_SIZE) < 6)
			return E_SAND;
		float q = noise3(noise, (float)x / 10,(float)z / 10, (float)y/10);
		q /= 2;
		q += 0.5;
		q *= 25;
		if (q > 12)
			return E_EMPTY;
		return (E_GRASS);
	}
	else if (arbre > 16.6 && y % (8 * CHUNK_SIZE) < arbre + 0 && !((4 + x) % 7) && !((4 + z) % 7))
		return (E_WOOD);
	else if (arbre > 16.6 && y % (8 * CHUNK_SIZE) < arbre + 0 && y % (8 * CHUNK_SIZE) > arbre / 2 + 5)
	{
		return E_LEAF;
	}
	return (E_EMPTY);
}

e_block island(uint32_t x, uint32_t y, uint32_t z, uint32_t seedz, t_noise *noise)
{
	float a = noise2(noise, (float)x / 300,(float)z/300);
	a /= 2;
	a += 0.5;
	a *= 7;
	float b = noise2(noise, (float)x / 100,(float)z/100);
	b *= 5;

	float t = noise2(noise, (float)x / 100,(float)z/100);
	t *= 3;

	float c = noise2(noise, (float)x / 10,(float)z/10);
	c *= 1;

	float d = noise2(noise, (float)x * 30 ,(float)z * 30);
	d *= 5;

	a = c + a + b;
	a = pow(a, 0.2);
	a *= 20;
//	y -= 20;
	if (y % (5 * CHUNK_SIZE) < a || y == 241)
	{
		y += 6;
		if (y % (4 * CHUNK_SIZE) < 14 - d)
			return E_SAND;
		if (y % (4 * CHUNK_SIZE) < 17 - t)
			return E_DIRT;
		if (y < 260)
			return E_ROCK;
		return (E_GRASS);
	}
//	else if (d > 3 && y % (1 * CHUNK_SIZE) < a + d && !(x % 4) && !(z % 4))
//		return (E_WOOD);
	return (E_EMPTY);
}

e_block snow(uint32_t x, uint32_t y, uint32_t z, uint32_t seedz, t_noise *noise)
{
	float a = noise2(noise, (float)x / 300,(float)z/300);
	a /= 2;
	a += 0.5;
	a *= 7;
	float b = noise2(noise, (float)x / 100,(float)z/100);
	b *= 5;

	float t = noise2(noise, (float)x / 100,(float)z/100);
	t *= 3;

	float c = noise2(noise, (float)x / 10,(float)z/10);
	c *= 1;

	float d = noise2(noise, (float)x * 30 ,(float)z * 30);
	d *= 5;

	a = c + a + b;
	a = pow(a, 0.2);
	a *= 26;
	if (y % (5 * CHUNK_SIZE) < a || y == 270)
	{
		return (E_SNOW);
	}
	return (E_EMPTY);
}

e_block desert(uint32_t x, uint32_t y, uint32_t z, uint32_t seedz, t_noise *noise)
{
	float a = noise2(noise, (float)x / 300,(float)z/300);
	a /= 2;
	a += 0.5;
	a *= 7;
	float b = noise2(noise, (float)x / 100 + 60,(float)z/100 + 60);
	b *= 3;

	float r = noise2(noise, (float)x / 100 + 30,(float)z/100 + 30);
	r *= 3;

	float t = noise2(noise, (float)x / 100,(float)z/100);
	t *= 3;

	float c = noise2(noise, (float)x / 10,(float)z/10);
	c *= 1;

	float d = noise2(noise, (float)x * 30 ,(float)z * 30);
	d *= 5;

	a = c + a + b + r;
	a = pow(a, 1.6);
	a *= 3;
	if (y % (5 * CHUNK_SIZE) < a || y == 270)
	{
		if (y == 270 || y < 270 + c + t)
			return (E_SAND);
		return (E_LEAF);
	}
	return (E_EMPTY);
}

e_block sky(uint32_t x, uint32_t y, uint32_t z, uint32_t seedz, t_noise *noise)
{

	return E_ROCK;
	return E_EMPTY;

}

e_block mountain(uint32_t x, uint32_t y, uint32_t z, uint32_t seedz, t_noise *noise)
{
	float a = noise2(noise, (float)x / 300,(float)z/300);
	a /= 2;
	a += 0.5;
	a *= 7;
	float b = noise2(noise, (float)x / 100,(float)z/100);
	b *= 5;

	float t = noise2(noise, (float)x / 100,(float)z/100);
	t *= 1;

	float c = noise2(noise, (float)x / 10,(float)z/10);
	c *= 1;

	float d = noise2(noise, (float)x * 30 ,(float)z * 30);
	d *= 5;

//	a = b + c;
	a = c + a + b;
	a /= 2;
	a = pow(a, 1.9);
	a -= t - 4;
//	a -= t - 5;
	y -= 4;
	if (y % (4 * CHUNK_SIZE) < (a) || y == 257)
	{
		if (((int)b ^ (int)(d / 3) << (int)b) > 248)
			return (E_ROCK);
		if (y % (4 * CHUNK_SIZE) > CHUNK_SIZE / 2 + d + 10)
			return (E_SNOW);
		if (y % (4 * CHUNK_SIZE) < 6)
			return E_SAND;
		float q = noise3(noise, (float)x / 10,(float)z / 10, (float)y/10);
		q /= 2;
		q += 0.5;
		q *= 40;
		if (q < y % (4 * CHUNK_SIZE))
			return E_EMPTY;
		return (E_GRASS);
	}
	else if (d > 3 && y % (4 * CHUNK_SIZE) < a + d && !(x % 6) && !(z % 6))
		return (E_WOOD);
	return (E_EMPTY);
}

int get_biom(uint32_t x, uint32_t y, uint32_t z, uint32_t seedz)
{
	if (y < 334 && y > 240)
		return (1);
	//if (y >= 334)
	//	return 8;
	return (3);
}

e_block get_id_at(uint32_t x, uint32_t y, uint32_t z, uint32_t seedz, t_noise *noise)
{
	if (!(rand() % 16000))
		return E_ROCK;
	return E_EMPTY;
	int biom = get_biom(x, y, z, seedz);
	if (biom == 0)
		return (hill(x, y, z, seedz, noise));
	if (biom == 1)
		return (mountain(x, y, z, seedz, noise));
	if (biom == 2)
		return (cave(x, y, z, seedz, noise));
	if (biom == 4)
		return (island(x, y, z, seedz, noise));
	if (biom == 5)
		return (desert(x, y, z, seedz, noise));
	if (biom == 6)
		return (snow(x, y, z, seedz, noise));
	if (biom == 8)
		return (sky(x, y, z, seedz, noise));
	if (biom == 3)
		return (E_EMPTY);
}
