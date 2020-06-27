#ifndef __Random_Point_Queue_By_Hash_h__
#define __Random_Point_Queue_By_Hash_h__

#include <unordered_map>

#include "RandomPointQueueBase.h"

class RandomPointQueueByHash : public RandomPointQueueBase
{
protected:
	typedef std::unordered_map<size_t, Point2D> PointBuffer;
	typedef std::pair<size_t, Point2D> PointItem;
	
	PointBuffer point_buf;
	size_t point_num;

public:
	RandomPointQueueByHash();
	~RandomPointQueueByHash();
	void add_point(Point2D &p) override;
	bool get_point(Point2D &p) override;
	bool is_empty() override;
};

#endif