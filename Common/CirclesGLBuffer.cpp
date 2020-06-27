#include <math.h>

#include "circle_mesh_data.h"

#include "CirclesGLBuffer.h"

CirclesGLBuffer::CirclesGLBuffer() :
	vao(0), vbo(0), vbo_inst(0), ebo(0)
{

}

CirclesGLBuffer::~CirclesGLBuffer() { clear(); }

void CirclesGLBuffer::clear()
{
	if (ebo)
	{
		glDeleteBuffers(1, &ebo);
		ebo = 0;
	}
	if (vbo)
	{
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}
	if (vbo_inst)
	{
		glDeleteBuffers(1, &vbo_inst);
		vbo_inst = 0;
	}
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
}

int CirclesGLBuffer::init(
	std::vector<glm::vec2> &pts,
	float pt_area,
	glm::vec3& pt_color
	)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(circle_nodes),
		circle_nodes,
		GL_STATIC_DRAW
		);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(circle_elems),
		circle_elems,
		GL_STATIC_DRAW
		);

	pt_num = pts.size();
	float pt_r = sqrt(pt_area);
	InstData *inst_data = new InstData[pt_num];
	for (size_t p_id = 0; p_id < pt_num; ++p_id)
	{
		glm::vec2 &pt = pts[p_id];
		InstData &id = inst_data[p_id];
		id.x = pt.x;
		id.y = pt.y;
		id.radius = pt_r;
		id.r = pt_color.r;
		id.g = pt_color.g;
		id.b = pt_color.b;
	}
	glGenBuffers(1, &vbo_inst);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_inst);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(InstData) * pt_num,
		inst_data,
		GL_STATIC_DRAW
		);
	delete[] inst_data;

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(InstData), (GLvoid *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(InstData), (GLvoid*)offsetof(InstData, radius));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(InstData), (GLvoid*)offsetof(InstData, r));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);

	return 0;
}

void CirclesGLBuffer::draw(OpenGLShaderProgram& shader)
{
	glBindVertexArray(vao);
	glDrawElementsInstanced(
		GL_TRIANGLES,
		sizeof(circle_elems) / sizeof(circle_elems[0]),
		GL_UNSIGNED_INT,
		nullptr,
		pt_num
		);
}
