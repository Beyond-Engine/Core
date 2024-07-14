#include <catch2/catch_test_macros.hpp>

#include <beyond/utils/hash.hpp>

#include <string>

template <typename T> void single_hash_combine(std::size_t& seed, const T& v)
{
  seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

TEST_CASE("hash combine")
{
  const std::size_t value1 = 1;
  const float value2 = 42.f;
  const std::string value3 = "hello world";

  const auto hash = []<typename T>(const T& v) { return std::hash<T>{}(v); };

  auto excepted = hash(value1);
  single_hash_combine(excepted, value2);
  single_hash_combine(excepted, value3);

  REQUIRE(beyond::hash_combine(value1, value2, value3) == excepted);
}