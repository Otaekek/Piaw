#include "renderBuiltIn.class.hpp"

uint32_t						renderBuiltIn::numInstance = 0;
t_instanceRenderer				renderBuiltIn::instances[4096];

GLuint		renderBuiltIn::init_instancing(uint32_t vaoId)
{
	GLuint instanceVBO;

	glBindVertexArray(vaoId);
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(sizeof(GLfloat)*3));
	glVertexAttribDivisor(4, 1);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(sizeof(GLfloat)*6));
	glVertexAttribDivisor(5, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return (instanceVBO);
}

uint32_t get_node_first_vao(uint32_t n) {

	t_renderMeshData	*mesh;
	t_node *node = (t_node*)staticMemoryManager::get_data_ptr(n);

	if (node->has_mesh)	{
		mesh = (t_renderMeshData*)staticMemoryManager::get_data_ptr(node->meshs);
		return mesh->vaoId;
	}
	return 0;
}

void	renderBuiltIn::stream_transform(uint32_t vao, uint32_t vbo, float *data, uint32_t num) {

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, num * sizeof(float) * 9, data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

t_instanceRenderer renderBuiltIn::create_instancing(uint32_t assetHandler) {

	t_instanceRenderer ir;
	uint32_t vaoId = get_node_first_vao(assetHandler);
	ir.assetHandler = assetHandler;
	ir.vao = vaoId;
	ir.vbo = init_instancing(vaoId);
	return ir;
}

void renderBuiltIn::render_me_instanced(t_instanceRenderer ir, uint32_t num, float *data) {
	//num = 1;
	stream_transform(ir.vao, ir.vbo, data, num);
	ir.num = num;
	instances[numInstance++] = ir;
}

void renderBuiltIn::render_instanced(t_camera *camera) {

	t_node *node;
	t_renderMeshData *mesh;
	glm::mat4 		projMat, viewMat;

	for (int i = 0; i < numInstance; i++) {
		node = (t_node*)staticMemoryManager::get_data_ptr(instances[i].assetHandler);
		mesh = (t_renderMeshData*)staticMemoryManager::get_data_ptr(node->meshs);
		glBindVertexArray(mesh->vaoId);
		viewMat = transformBuiltin::to_mat_cam(camera->transformHandler);
		projMat = transformBuiltin::projection_matrix(80.0f, 1.0f, 10000.0f,
		(float)(mode->width * camera->sizex) / (mode->height * camera->sizey));
		glUseProgram(node->program);
//		face_culling(elem);

		/*Set projection Matrix*/
		GLint uniProj = glGetUniformLocation(node->program, "P");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projMat));

		/*Set camera */
		GLint cam = glGetUniformLocation(node->program, "V");
		glUniformMatrix4fv(cam, 1, GL_FALSE, glm::value_ptr(viewMat));

	//	printf("%d couillzer\n", node->program);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indiceBufferId);
		glDrawElementsInstanced(GL_TRIANGLES, mesh->indiceNum, GL_UNSIGNED_INT, 0, instances[i].num);
 	}
	numInstance = 0;
}
