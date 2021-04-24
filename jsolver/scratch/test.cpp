#include "test.h"
#include "../logging.h"

#include "../maths/matrix.h"


void do_work()
{
	using Mat = Matrix<double>;

	Mat m0{ 5, 5 };
	Mat m1{ 5, 5, 10};

	log()->info("{}", m0);
}