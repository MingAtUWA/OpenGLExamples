#ifndef __PDS_Result_View_h__
#define __PDS_Result_View_h__

#include "OpenGLShaderUtilities.h"
#include "CirclesGLBuffer.h"
#include "GlfwApp.h"

class PDSResultView : public GlfwApp
{
protected:
	CirclesGLBuffer point_buf;
	OpenGLShaderProgram point_shader;

	void set_square_viewport(int wd, int ht);

public:
	PDSResultView();
	~PDSResultView();

	int init() override;
	int paint() override;
	void destroy() override;
	int resize(int wd, int ht) override;
};

#endif