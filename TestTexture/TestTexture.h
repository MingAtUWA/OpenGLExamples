#ifndef __Test_Texture_h__
#define __Test_Texture_h__

#include "GlfwApp.h"
#include "OpenGLShaderUtilities.h"

class TestTexture : public GlfwApp
{
public:
	enum TextureDim
	{
		one_d,
		two_d
	};

protected:
	TextureDim tex_dim;

	GLuint vao, vbo, ebo;

	OpenGLShaderProgram shader_1d;
	OpenGLShaderProgram shader_2d;

	GLuint texture1d;
	GLuint texture2d1, texture2d2;

public:
	TestTexture();
	~TestTexture() {}

	inline void set_texture_dim(TextureDim dim) { tex_dim = dim; }

	int init() override;
	int paint() override;
	void destroy() override;
};

#endif