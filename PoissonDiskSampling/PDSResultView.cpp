#include "PoissonDiskSampling.h"

#include "PDSResultView.h"

PDSResultView::PDSResultView()
{

}

PDSResultView::~PDSResultView()
{

}

void PDSResultView::set_square_viewport(int wd, int ht)
{
	int padding;
	if (wd >= ht)
	{
		padding = (wd - ht) / 2;
		glViewport(padding, 0, ht, ht);
	}
	else
	{
		padding = (ht - wd) / 2;
		glViewport(0, padding, wd, wd);
	}
}

int PDSResultView::init()
{
	// generate point with poisson disk sampling method
	PoissonDiskSampling pds;
	pds.generate_points_in_rect(-1.0, 1.0, -1.0, 1.0, 0.02);
	
	set_square_viewport(width, height);

	point_shader.create("../../Shaders/circles_shader.vert",
						"../../Shaders/circles_shader.frag");
	point_shader.use();
	glm::mat4 proj_mat = glm::ortho(-1.0, 1.0, -1.0, 1.0);
	point_shader.set_uniform("proj_mat", proj_mat);

	// draw point buffer
	point_buf.init(pds.get_points(), 1.0e-4,
				   glm::vec3(1.0f, 1.0f, 0.804f));
	
	return 0;
}

int PDSResultView::paint()
{
	point_shader.use();
	point_buf.draw(point_shader);
	return 0;
}

void PDSResultView::destroy()
{

}

int PDSResultView::resize(int wd, int ht)
{
	set_square_viewport(wd, ht);
	return 0;
}
