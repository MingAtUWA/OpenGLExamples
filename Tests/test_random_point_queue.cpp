#include <iostream>
#include <chrono>

#include "RandomPointQueueByHash.h"
#include "RandomPointQueueByTree.h"

#include "TestsMain.h"

#define LOOP_TIMES 100000

int test_random_point_queue(int argc, char** argv)
{
	using std::chrono::system_clock;

	RandomPointQueueByHash rq_hash;
	RandomPointQueueByTree rq_tree;
	Point2D pt(0.0, 0.0);

	system_clock::time_point start_time, end_time;
	std::chrono::milliseconds duration;

	// hash random queue
	start_time = system_clock::now();
	for (size_t i = 0; i < LOOP_TIMES; i++)
	{
		pt.x = double(i);
		rq_hash.add_point(pt);
	}
	for (size_t i = 0; i < LOOP_TIMES; i++)
	{
		rq_hash.get_point(pt);
	}
	end_time = system_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cout << "hash quene: " << duration.count() << " ms" << std::endl;

	// tree random queue
	start_time = system_clock::now();
	for (size_t i = 0; i < LOOP_TIMES; i++)
	{
		pt.x = double(i);
		rq_tree.add_point(pt);
	}
	for (size_t i = 0; i < LOOP_TIMES; i++)
	{
		rq_tree.get_point(pt);
	}
	end_time = system_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cout << "tree quene: " << duration.count() << " ms" << std::endl;

	return 0;
}
