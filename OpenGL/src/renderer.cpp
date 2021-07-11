#include "renderer.h"

Renderer::Renderer()
{
}

void Renderer::draw(ShaderProgram& program, Model& model) const
{
	program.Bind();

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.getEBO());
	glBindVertexArray(model.getVAO());

	glDrawElements(model.getDrawMode(), model.getSize(), GL_UNSIGNED_INT, 0);
}
