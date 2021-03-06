#include <renderBuiltIn.class.hpp>

void renderBuiltIn::renderOnScreen()
{
	GLuint texID;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, mode->width, mode->height);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderDataSys::_programm[E_ONSCREEN]);
	glBindVertexArray(_quadVertexbuffer);
	glBindTexture(GL_TEXTURE_2D, _colorTextureid);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
