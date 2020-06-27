#ifndef __Circle_GL_Buffer_h__
#define __Circle_GL_Buffer_h__

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "OpenGLShaderUtilities.h"

// use opengl instancing
class CirclesGLBuffer
{
protected:
	struct InstData
	{
		GLfloat x, y;
		GLfloat radius;
		GLfloat r, g, b;
	};
	
	size_t pt_num;
	GLuint vao, vbo, vbo_inst, ebo;

public:
	CirclesGLBuffer();
	~CirclesGLBuffer();
	void clear();

	int init(std::vector<glm::vec2> &pts,
		float pt_area, glm::vec3 &pt_color);

	void draw(OpenGLShaderProgram &shader);
};

#endif