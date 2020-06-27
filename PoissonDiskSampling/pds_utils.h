#ifndef __utils_h__
#define __utils_h__

#include <cstdlib>

struct Point2D
{
	double x;
	double y;
	Point2D *next;
	Point2D() {}
	Point2D(double _x, double _y) : x(_x), y(_y) {}
	Point2D(const Point2D &oth) : x(oth.x), y(oth.y) {}
	inline Point2D& operator=(Point2D& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}
};

class RandNum
{
private:
	// singleton
	RandNum() { srand(1); }
	~RandNum() {}
	// no copy
	RandNum(RandNum& other) {}
	RandNum(const RandNum& other) {}
	RandNum& operator=(RandNum& other);
	RandNum& operator=(const RandNum& other);
	
	static RandNum inst;

	// generate random int between [0, RAND_MAX]
	inline int gen_int_rand() { return rand(); }
	// generate random int between [0, max]
	inline int gen_int_rand(int max) { return rand() % (max + 1); }
	// generate random int between [min, max]
	inline int gen_int_rand(int min, int max) { return min + rand() % (max-min+1); }
	// generate random double between [0.0, 1.0]
	inline double gen_double_rand()
	{
		return 1.0 / double(RAND_MAX) * double(rand());
	}
	// generate random double between [0.0, max]
	inline double gen_double_rand(double max)
	{
		return max / double(RAND_MAX) * double(rand());
	}
	// generate random double between [min, max]
	inline double gen_double_rand(double min, double max)
	{
		return min + (max - min) / double(RAND_MAX) * double(rand());
	}

public:
	inline static void set_seed(unsigned int seed) { srand(seed); }
	inline static int get_int() { return inst.gen_int_rand(); }
	inline static int get_int(int max) { return inst.gen_int_rand(max); }
	inline static int get_int(int min, int max) { return inst.gen_int_rand(min, max); }
	inline static double get_double() { return inst.gen_double_rand(); }
	inline static double get_double(double max) { return inst.gen_double_rand(max); }
	inline static double get_double(double min, double max) { return inst.gen_double_rand(min, max); }
};

#endif