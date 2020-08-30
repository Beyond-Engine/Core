#include "beyond/container/static_vector.hpp"

int main()
{
  beyond::StaticVector<int, 5> v{0, 1, 2, 3};
  return v[0];
}
