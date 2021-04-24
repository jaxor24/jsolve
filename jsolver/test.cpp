#include "test.h"
#include "logging.h"

#include "matrix.h"

void do_work()
{
	using Mat = Matrix<double>;

	Mat m0{ 5, 5 };
	Mat m1{ 5, 5, 10};

	// auto m2 = m0 + m1;

	log()->info("{}", m0);
	log()->info("{}", m1);

	log()->info("{}", m0(0, 0));
	log()->info("{}", m1(0, 0));
}