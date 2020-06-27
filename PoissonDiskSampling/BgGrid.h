#ifndef __Bg_Grid_h__
#define __Bg_Grid_h__

#include "pds_utils.h"

class BgGrid
{
protected:
	struct Cell
	{
		size_t x_id;
		size_t y_id;
		Point2D* top;
	};

	double xl, yl;
	double xu, yu;
	double hx, hy;
	size_t x_num, y_num;
	Cell* cells;

public:
	BgGrid() : x_num(0), y_num(0), cells(nullptr) {}
	~BgGrid() { clear(); }

	inline Cell& get_cell(size_t x_id, size_t y_id)
	{
		return cells[y_id * x_num + x_id];
	}

	int init(double _xl, double _xu,
			 double _yl, double _yu,
			 size_t _x_num, size_t _y_num);

	void clear();

	bool add_point(Point2D& p);

	bool is_in_grid(Point2D &p);
	bool has_point_nearby(Point2D& p, double dist);
};

#endif