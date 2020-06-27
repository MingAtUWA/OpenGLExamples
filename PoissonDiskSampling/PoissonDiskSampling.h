#ifndef __Poisson_Disk_Sampling_h__
#define __Poisson_Disk_Sampling_h__

#include <vector>
#include <glm/glm.hpp>

#include "pds_utils.h"

class PoissonDiskSampling
{
protected:
	std::vector<glm::vec2> points;

	Point2D gen_rand_point_around1(Point2D &p, double dist_min);
	Point2D gen_rand_point_around2(Point2D& p, double dist_min);

public:
	PoissonDiskSampling();
	~PoissonDiskSampling() { clear(); }
	inline void clear() { points.clear(); }

	inline std::vector<glm::vec2> &get_points() { return points; }
	
	int generate_points_in_rect(
		double xl, double yl, double xu, double yu,
		double dist_min);
};

#endif