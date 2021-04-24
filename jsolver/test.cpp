#include "test.h"
#include "logging.h"

#include "matrix.h"

void do_work()
{
	using Mat = Matrix<double>;

	Mat m0{ 5, 5 };
	Mat m1{ 3, 5, 10.5};

	auto m2 = m1.transpose();
	auto m3 = m0 + m1;
	auto m4 = m3 - m1;

	log()->info("m0: {}", m0);
	log()->info("m1: {}", m1);
	log()->info("m2: {}", m2);

	log()->info("{}", m0(0, 0));
	log()->info("{}", m1(0, 0));
}