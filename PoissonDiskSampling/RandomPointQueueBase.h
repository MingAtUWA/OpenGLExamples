#ifndef __Random_Point_Queue_Base_h__
#define __Random_Point_Queue_Base_h__

#include "pds_utils.h"

class RandomPointQueueBase
{
public:
	RandomPointQueueBase() {}
	virtual ~RandomPointQueueBase() {}
	virtual void add_point(Point2D &p) = 0;
	virtual bool get_point(Point2D& p) = 0;
	virtual bool is_empty() = 0;
};

#endif