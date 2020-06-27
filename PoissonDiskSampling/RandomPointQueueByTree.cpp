#include "RandomPointQueueByTree.h"

RandomPointQueueByTree::RandomPointQueueByTree() :
	point_num(0)
{

}

RandomPointQueueByTree::~RandomPointQueueByTree()
{

}

void RandomPointQueueByTree::add_point(Point2D& p)
{
	point_buf.insert(PointItem(point_num, p));
	++point_num;
}

bool RandomPointQueueByTree::get_point(Point2D& p)
{
	if (point_num == 0)
		return false;

	size_t p_id = RandNum::get_int(point_num-1);
	auto p_iter = point_buf.find(p_id);
	p = p_iter->second;
	point_buf.erase(p_iter);
	if (p_id != point_num - 1)
	{
		// swap the last item with this one
		auto lst_iter = point_buf.find(point_num - 1);
		Point2D lst_p = lst_iter->second;
		point_buf.erase(lst_iter);
		point_buf.insert(PointItem(p_id, lst_p));
	}
	--point_num;
	return true;
}

bool RandomPointQueueByTree::is_empty()
{
	return point_num != 0;
}
