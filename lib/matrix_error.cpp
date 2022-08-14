#include "matrix_error.h"


MatrixError::MatrixError(const std::string& message)
	:
	std::runtime_error(message)
{}

MatrixError::~MatrixError() = default;
