#include <catch2/catch_test_macros.hpp>

#include "beyond/algorithm/sort_by_key.hpp"

#include <array>
#include <string_view>

static_assert(
    std::random_access_iterator<beyond::detail::SortByKeyIterator<int*, int*>>);

TEST_CASE("Sort by key", "[beyond.core.algorithm.sort_by_key]")
{
  struct Result {
    std::array<int, 9> keys;
    std::array<std::string_view, 9> mapped;
  };

  constexpr auto generate_result = []() {
    Result result = {.keys = {9, 2, 7, 8, 5, 4, 3, 6, 1},
                     .mapped = {"9", "2", "7", "8", "5", "4", "3", "6", "1"}};
    beyond::sort_by_key(result.keys.begin(), result.keys.end(),
                        result.mapped.begin());
    return result;
  };

  SECTION("constexpr sort")
  {
    constexpr auto result = generate_result();

    constexpr int keys_expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr std::string_view mapped_expected[] = {"1", "2", "3", "4", "5",
                                                    "6", "7", "8", "9"};
    STATIC_REQUIRE(std::ranges::equal(result.keys, keys_expected));
    STATIC_REQUIRE(std::ranges::equal(result.mapped, mapped_expected));
  }

  SECTION("runtime sort")
  {
    auto result = generate_result();

    int keys_expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::string_view mapped_expected[] = {"1", "2", "3", "4", "5",
                                          "6", "7", "8", "9"};
    REQUIRE(std::ranges::equal(result.keys, keys_expected));
    REQUIRE(std::ranges::equal(result.mapped, mapped_expected));
  }
}