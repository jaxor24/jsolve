#include "matrix.h"
#include <iomanip>

// Free functions - helpers

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{
    os << std::fixed << std::setprecision(4);
    os << "\n";
    os << "[";
    for (const auto& [n_elem, elem] : enumerate(m.m_data))
    {
        os << std::setw(7) << elem;

        if (n_elem + 1 != m.m_data.size())
        {
            if ((n_elem + 1) % m.n_cols() == 0)
            {
                os << "\n";
                os << "  ";
            }
            else
            {
                os << ",";
            }
        }
    }

    os << "]\n";

    return os;
}
