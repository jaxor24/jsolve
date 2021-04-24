#include "test.h"

#include "logging.h"
#include "matrix.h"

void do_work()
{
	using Matr = Matrix<double>;

	Matr m{ 2, 2, 0.0 };
	m(0, 0) = 1;
	m(0, 1) = 2;
	m(1, 0) = 3;
	m(1, 1) = 4;
	auto transpose = m.make_transpose();

	auto m1 = m.make_transpose();
	//auto m3 = m0 + m1;
	//auto m4 = m3 - m1;

	log()->info("m0: {}", m);
	log()->info("m1: {}", m1);
}