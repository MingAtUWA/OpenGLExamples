#ifndef __Display_Ttf_h__
#define __Display_Ttf_h__

#include <map>

#include "OpenGLShaderUtilities.h"
#include "GlfwApp.h"

class DisplayTtf : public GlfwApp
{
protected:
	struct Character
	{
		GLuint tex_id; // texture id
		glm::ivec2 size; // glyph size
		glm::ivec2 bearing; // offset from baseline to left/top of glyph
		GLuint advance; // horizontal offset to next glyph
	};
	std::map<GLchar, Character> characters;
	
	OpenGLShaderProgram shader;

	GLuint vao, vbo;
	
public:
	DisplayTtf();
	~DisplayTtf() {}

	int init() override;
	int paint() override;
	void destroy() override;

protected:
	void render_text(const char* text, float x_pos, float y_pos, float scale, glm::vec3& color);
};

#endif