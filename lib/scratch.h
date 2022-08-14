// auto row(std::size_t row = 0)
//{
//	auto start = col_iterator{ std::begin(m_data), std::end(m_data), n_cols() };
//	for (std::size_t i = 0; i < row; ++i) { start.next_row(); };
//	auto end = col_iterator{ std::end(m_data), std::end(m_data), n_cols() };
//	return IterableWrapper<col_iterator>{ start, end };
// }
//
// auto column(std::size_t col = 0)
//{
//	auto start = row_iterator{ std::begin(m_data), std::end(m_data), n_cols() };
//	for (std::size_t i = 0; i < col; ++i) { start.next_col(); };
//	auto end = row_iterator{ std::end(m_data), std::end(m_data), n_cols() };
//	return IterableWrapper<row_iterator>{ start, end };
// }
//
// auto enumerate_rows(std::size_t col = 0)
//{
//	auto start = row_iterator{ std::begin(m_data), std::end(m_data), n_cols() };
//	for (std::size_t i = 0; i < col; ++i) { start.next_col(); };
//	auto end = row_iterator{ std::end(m_data), std::end(m_data), n_cols() };
//	return IterableWrapper<RowEnumerator<matrix_iterator>>{ begin(), end() };
// }
//
// auto enumerate_cols(std::size_t row = 0)
//{
//	auto start = col_iterator{ std::begin(m_data), std::end(m_data), n_cols() };
//	for (std::size_t i = 0; i < row; ++i) { start.next_row(); };
//	auto end = col_iterator{ std::end(m_data), std::end(m_data), n_cols() };
//	return IterableWrapper<Enumerator<col_iterator>>{ start, end };
// }