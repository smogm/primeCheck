#include <basePrime.hpp>

std::ostream& operator<<(std::ostream& os, const BasePrime& foo)
{
    os << foo.className;
    return os;
}
