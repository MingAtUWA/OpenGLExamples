#ifndef __Random_Point_Queue_By_Tree_h__
#define __Random_Point_Queue_By_Tree_h__

#include <map>

#include "RandomPointQueueBase.h"

class RandomPointQueueByTree : public RandomPointQueueBase
{
protected:
	typedef std::map<size_t, Point2D> PointBuffer;
	typedef std::pair<size_t, Point2D> PointItem;

	PointBuffer point_buf;
	size_t point_num;

public:
	RandomPointQueueByTree();
	~RandomPointQueueByTree();
	void add_point(Point2D &p) override;
	bool get_point(Point2D &p) override;
	bool is_empty() override;
};

#endif