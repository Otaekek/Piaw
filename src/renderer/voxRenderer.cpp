#include <cubeMap.class.hpp>
#include <renderBuiltIn.class.hpp>

GLuint	renderBuiltIn::vox_tex = 0;
const float table[22] = {3, 15, 0, 15, 1, 5, 2, 15, 0, 14, 4, 11, 2, 11, 4, 14, 5, 14, 2, 14, 7, 9};
const float iso_table[22] = {2, 15, 0, 15, 1, 5, 2, 15, 0, 14, 2, 15, 2, 11, 4, 14, 4, 14, 2, 14, 7, 9};

void renderBuiltIn::init_vox()
{
	uint32_t textureID;
	uint32_t instance;

//	textureID = fileLoader::load_fs_asset_sync("./texturePack/171.JPG", 1);
	textureID = fileLoader::load_fs_asset_sync("oui.jpg", 1);
	instance = texture_builtin::create_instance(textureID);
	vox_tex = texture_builtin::convert_to_opengl_parametric(instance, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
}

bool cull(float posx, float posz, t_transform transform, uint32_t h, float d)
{
	glm::vec2 p1;
	glm::vec2 p2;
	glm::vec2 p3;
	glm::vec3 tmp;

	p1.x = posx;
	p1.y = posz;
	tmp = transform.position;
	p2.x = tmp.x;
	p2.y = tmp.z;

	p3 = -p2;
	p1 = glm::normalize(-p2 - p1);
	tmp = transformBuiltin::get_direction(h);
	p2.x = tmp.x;
	p2.y = tmp.z;
	p2 = glm::normalize(p2);
	if ((glm::dot(p1, p2) < d && glm::distance(p3, glm::vec2(posx, posz)) > 2 * SIZE_BLOCK * CHUNK_SIZE))
		return false;
	return (true);
}

void renderBuiltIn::renderVox(t_camera camera, float width, float height)
{
	glm::vec3 pos = {0, 0, 0};
	uint32_t coord[3];
	float fcoord[3];
	uint32_t range = 5;
	uint32_t rangey = 3;

	int32_t			cx[16048];
	int32_t			cy[16048];
	int32_t			cz[16048];
	t_renderVox		*voxList[16048];
	int32_t			size = 0;

	t_renderVox *go;
	t_transform *transform = transformBuiltin::get_transform(camera.transformHandler);
	t_transform t = *transform;
	pos.x = -transform->position.x;
	pos.y = -transform->position.y;
	pos.z = -transform->position.z;
	cubeMap::convert_coord(coord, pos.x, pos.y, pos.z);
	if (fabs(pos.y) > 20068.503906)
		return ;
	glEnable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, vox_tex);
	for (int i = 0; i < range; i++)
		for (int j = 0; j < rangey; j++)
			for (int k = 0; k < range; k++)
			{
				cx[size] = CHUNK_SIZE * (i - range / 2) + coord[0];
				cy[size] = CHUNK_SIZE * (j - rangey / 2) + coord[1];
				cz[size] = CHUNK_SIZE * (k - range / 2) + coord[2];
				cubeMap::convert_coord_to_world(fcoord, cx[size], cy[size],cz[size]);
				t.position.x = fcoord[0];
				t.position.y = fcoord[1];
				t.position.z = fcoord[2];
				if (!cull(transform->position.x, transform->position.z, t, camera.transformHandler, 0.41))
					continue ;
				size++;
			}

	cubeMap::get_renderGOs_thread(voxList, cx, cy, cz, size);
	for (int i = 0;i < size; i++)
		if (voxList[i] != NULL && voxList[i]->vaoId != 0 && cull(transform->position.x, transform->position.z, *transformBuiltin::get_transform(voxList[i]->transformHandler), camera.transformHandler, 0.41))
		{
			render_vox(voxList[i], &camera, renderDataSys::_programm[3]);
		}
	glDisable(GL_CULL_FACE);
}

void			renderBuiltIn::render_vox_mesh(t_renderVox *elem, uint32_t program)
{
	GLuint location;
	uint32_t uniformId;
	uniformId = glGetUniformLocation(renderDataSys::_programm[elem->program], "tables");
	if (elem->program == E_VOX)
		glUniform2fv(uniformId, 11, table);
	else
		glUniform2fv(uniformId, 11, iso_table);
	uniformId = glGetUniformLocation(renderDataSys::_programm[elem->program], "time");
	glUniform1f(uniformId, (float)clock());

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(elem->vaoId);
	//printf("%d\n", elem->vaoId);
	glDrawElements(GL_TRIANGLES, elem->numVertex, GL_UNSIGNED_INT, 0);
}

void 			renderBuiltIn::render_vox(t_renderVox *elem, t_camera *camera, uint32_t program)
{
	glm::mat4 			modelMat, projMat, viewMat;

	modelMat = transformBuiltin::to_mat(elem->transformHandler);
	viewMat = transformBuiltin::to_mat_cam(camera->transformHandler);
	projMat = transformBuiltin::projection_matrix(80.0f, 1.0f, 10000.0f,
		(float)(mode->width * camera->sizex) / (mode->height * camera->sizey));
	glUseProgram(renderDataSys::_programm[elem->program]);

	/*Set projection Matrix*/
	GLint uniProj = glGetUniformLocation(program, "P");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projMat));

	/*Set camera */
	GLint cam = glGetUniformLocation(program, "V");
	glUniformMatrix4fv(cam, 1, GL_FALSE, glm::value_ptr(viewMat));

	/* Set model */
	GLint model = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(modelMat));

	render_vox_mesh(elem, program);
}
