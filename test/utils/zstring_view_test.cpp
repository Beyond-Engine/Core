#include <catch2/catch_test_macros.hpp>

#include <beyond/utils/zstring_view.hpp>

TEST_CASE("default constructor for ZStringView",
          "[beyond.core.util.zstring_view]")
{
  constexpr beyond::ZStringView sv;
  REQUIRE(sv.empty());
  REQUIRE(strcmp(sv.data(), "") == 0);
  REQUIRE(strcmp(sv.c_str(), "") == 0);
  REQUIRE(sv.size() == 0);
}

TEST_CASE("ZStringView from a string literal",
          "[beyond.core.util.zstring_view]")
{
  constexpr beyond::ZStringView sv{"Hello world"};
  REQUIRE(not sv.empty());
  REQUIRE(strcmp(sv.c_str(), "Hello world") == 0);
  REQUIRE(sv.size() == 11);
}

TEST_CASE("ZStringView from a c literal", "[beyond.core.util.zstring_view]")
{
  constexpr const char* c_string = "Hello world";
  constexpr beyond::ZStringView sv{c_string};
  REQUIRE(strcmp(sv.c_str(), c_string) == 0);
  REQUIRE(sv.size() == strlen(c_string));

  beyond::ZStringView runtime_sv{c_string};
  REQUIRE(strcmp(runtime_sv.c_str(), c_string) == 0);
  REQUIRE(runtime_sv.size() == strlen(c_string));
}

TEST_CASE("Implicitly construct ZStringView from std::string",
          "[beyond.core.util.zstring_view]")
{
  std::string str = "Hello";
  beyond::ZStringView sv = str;
  REQUIRE(sv == str);
}

TEST_CASE("Implicitly convert ZStringView to std::string_view",
          "[beyond.core.util.zstring_view]")
{
  beyond::ZStringView sv = "Hello";
  std::string_view std_sv = sv;
  REQUIRE(std::string{std_sv} == sv);
}