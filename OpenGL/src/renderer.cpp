#include "renderer.h"

#include <iostream>
Renderer::Renderer()
{
}

void Renderer::draw(ShaderProgram& program, Model& model) const
{

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.getEBO());
	glBindVertexArray(model.getVAO());
	void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	float data2[720];
	memcpy(data2, data, sizeof(float) * 180);
	for (int i = 0; i != 36; ++i)
	{
		for (int j = 0; j != 5; ++j)
		{
			std::cout << data2[i * 5 + j] << ' ';
		}
		std::cout << std::endl;
	}*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.getEBO());
	glBindVertexArray(model.getVAO());
	program.Bind();

	if (model.isIndexed())
		glDrawElements(model.getDrawMode(), 6, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(model.getDrawMode(), 0, 36);
}
