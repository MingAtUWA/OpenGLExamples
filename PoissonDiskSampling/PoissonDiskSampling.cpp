#include "BgGrid.h"
#include "RandomPointQueueByHash.h"

#include "PoissonDiskSampling.h"

#define NEW_POINTS_COUNT 30
#define gen_rand_point_around gen_rand_point_around1

PoissonDiskSampling::PoissonDiskSampling() {}

int PoissonDiskSampling::generate_points_in_rect(
	double xl, double xu, double yl, double yu,
	double dist_min)
{
	// init grid
	double cell_size = dist_min / sqrt(2.0); // rule of thumb
	size_t grid_x_num = size_t(ceil((xu - xl) / cell_size));
	size_t grid_y_num = size_t(ceil((yu - yl) / cell_size));
	BgGrid grid;
	grid.init(xl, xu, yl, yu, grid_x_num, grid_y_num);

	// random queue
	RandomPointQueueByHash rq;

	// point list
	size_t appx_pt_num = (xu - xl) * (yu - yl) / (dist_min*dist_min);
	std::vector<Point2D> pts;
	pts.reserve(appx_pt_num * 1.2);

	Point2D pt_tmp;
	// generate the first random point
	pt_tmp.x = RandNum::get_double(xl, xu);
	pt_tmp.y = RandNum::get_double(yl, yu);
	pts.emplace_back(pt_tmp);
	grid.add_point(pts.back());
	rq.add_point(pt_tmp);
	
	// generate other random points
	Point2D cur_pt;
	while (rq.get_point(cur_pt))
	{
		for (size_t i = 0; i < NEW_POINTS_COUNT; ++i)
		{
			// generate random points around
			pt_tmp = gen_rand_point_around(cur_pt, dist_min);
			if (grid.is_in_grid(pt_tmp) &&
				!grid.has_point_nearby(pt_tmp, dist_min))
			{
				pts.emplace_back(pt_tmp);
				grid.add_point(pts.back());
				rq.add_point(pt_tmp);
			}
		}
	}

	// copy into points buffer
	size_t pt_num = pts.size();
	points.resize(pt_num);
	for (size_t p_id = 0; p_id < pt_num; ++p_id)
	{
		Point2D& pt = pts[p_id];
		glm::vec2 &point = points[p_id];
		point.x = pt.x;
		point.y = pt.y;
	}

	return 0;
}

Point2D PoissonDiskSampling::gen_rand_point_around1(Point2D& p, double dist)
{
	double radius = dist * (1.0 + RandNum::get_double());
	double angle = 2.0 * 3.14159265359 * RandNum::get_double();
	return Point2D(p.x + radius * cos(angle),
				   p.y + radius * sin(angle));
}

// to be finished
Point2D PoissonDiskSampling::gen_rand_point_around2(Point2D& p, double dist)
{
	double dist_min2 = dist * dist;
	dist *= 2.0;
	double dist_max2 = dist * dist;
	double pxl = p.x - dist;
	double pxu = p.x + dist;
	double pyl = p.y - dist;
	double pyu = p.y + dist;
	double pt_x, pt_y, dx, dy, dist2;
	while (true)
	{
		pt_x = RandNum::get_double(pxl, pxu);
		pt_y = RandNum::get_double(pyl, pyu);
		dx = pt_x - p.x;
		dy = pt_y - p.y;
		dist2 = dx * dx + dy * dy;
		if (dist2 >= dist_min2 && dist2 <= dist_max2)
			break;
	}
	return Point2D(pt_x, pt_y);
}
