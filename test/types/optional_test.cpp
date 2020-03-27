#include "beyond/types/optional.hpp"
#include <catch2/catch.hpp>

#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

TEST_CASE("optional Assignment value", "[beyond.core.types.optional]")
{
  beyond::optional<int> o1 = 42;
  beyond::optional<int> o2 = 12;
  beyond::optional<int> o3;

  o1 = o1;
  REQUIRE(*o1 == 42);

  o1 = o2;
  REQUIRE(*o1 == 12);

  o1 = o3;
  REQUIRE(!o1);

  o1 = 42;
  REQUIRE(*o1 == 42);

  o1 = beyond::nullopt;
  REQUIRE(!o1);

  o1 = std::move(o2);
  REQUIRE(*o1 == 12);

  beyond::optional<short> o4 = 42;

  o1 = o4;
  REQUIRE(*o1 == 42);

  o1 = std::move(o4);
  REQUIRE(*o1 == 42);
}

TEST_CASE("optional Assignment reference", "[beyond.core.types.optional]")
{
  auto i = 42;
  auto j = 12;

  beyond::optional<int&> o1 = i;
  beyond::optional<int&> o2 = j;
  beyond::optional<int&> o3;

  o1 = o1;
  REQUIRE(*o1 == 42);
  REQUIRE(&*o1 == &i);

  o1 = o2;
  REQUIRE(*o1 == 12);

  o1 = o3;
  REQUIRE(!o1);

  auto k = 42;
  o1 = k;
  REQUIRE(*o1 == 42);
  REQUIRE(*o1 == i);
  REQUIRE(*o1 == k);
  REQUIRE(&*o1 != &i);
  REQUIRE(&*o1 == &k);

  k = 12;
  REQUIRE(*o1 == 12);

  o1 = beyond::nullopt;
  REQUIRE(!o1);

  o1 = std::move(o2);
  REQUIRE(*o1 == 12);
}

TEST_CASE("optional Triviality", "[beyond.core.types.optional]")
{
  REQUIRE(std::is_trivially_copy_constructible<beyond::optional<int>>::value);
  REQUIRE(std::is_trivially_copy_assignable<beyond::optional<int>>::value);
  REQUIRE(std::is_trivially_move_constructible<beyond::optional<int>>::value);
  REQUIRE(std::is_trivially_move_assignable<beyond::optional<int>>::value);
  REQUIRE(std::is_trivially_destructible<beyond::optional<int>>::value);

  {
    struct T {
      T(const T&) = default;
      T(T&&) = default;
      T& operator=(const T&) = default;
      T& operator=(T&&) = default;
      ~T() = default;
    };
    REQUIRE(std::is_trivially_copy_constructible<beyond::optional<T>>::value);
    REQUIRE(std::is_trivially_copy_assignable<beyond::optional<T>>::value);
    REQUIRE(std::is_trivially_move_constructible<beyond::optional<T>>::value);
    REQUIRE(std::is_trivially_move_assignable<beyond::optional<T>>::value);
    REQUIRE(std::is_trivially_destructible<beyond::optional<T>>::value);
  }

  {
    struct T {
      T(const T&) {}
      T(T&&){};
      T& operator=(const T&)
      {
        return *this;
      }
      T& operator=(T&&)
      {
        return *this;
      };
      ~T() {}
    };
    REQUIRE(!std::is_trivially_copy_constructible<beyond::optional<T>>::value);
    REQUIRE(!std::is_trivially_copy_assignable<beyond::optional<T>>::value);
    REQUIRE(!std::is_trivially_move_constructible<beyond::optional<T>>::value);
    REQUIRE(!std::is_trivially_move_assignable<beyond::optional<T>>::value);
    REQUIRE(!std::is_trivially_destructible<beyond::optional<T>>::value);
  }
}

TEST_CASE("optional Deletion", "[beyond.core.types.optional]")
{
  REQUIRE(std::is_copy_constructible<beyond::optional<int>>::value);
  REQUIRE(std::is_copy_assignable<beyond::optional<int>>::value);
  REQUIRE(std::is_move_constructible<beyond::optional<int>>::value);
  REQUIRE(std::is_move_assignable<beyond::optional<int>>::value);
  REQUIRE(std::is_destructible<beyond::optional<int>>::value);

  {
    struct T {
      T(const T&) = default;
      T(T&&) = default;
      T& operator=(const T&) = default;
      T& operator=(T&&) = default;
      ~T() = default;
    };
    REQUIRE(std::is_copy_constructible<beyond::optional<T>>::value);
    REQUIRE(std::is_copy_assignable<beyond::optional<T>>::value);
    REQUIRE(std::is_move_constructible<beyond::optional<T>>::value);
    REQUIRE(std::is_move_assignable<beyond::optional<T>>::value);
    REQUIRE(std::is_destructible<beyond::optional<T>>::value);
  }

  {
    struct T {
      T(const T&) = delete;
      T(T&&) = delete;
      T& operator=(const T&) = delete;
      T& operator=(T&&) = delete;
    };
    REQUIRE(!std::is_copy_constructible<beyond::optional<T>>::value);
    REQUIRE(!std::is_copy_assignable<beyond::optional<T>>::value);
    REQUIRE(!std::is_move_constructible<beyond::optional<T>>::value);
    REQUIRE(!std::is_move_assignable<beyond::optional<T>>::value);
  }

  {
    struct T {
      T(const T&) = delete;
      T(T&&) = default;
      T& operator=(const T&) = delete;
      T& operator=(T&&) = default;
    };
    REQUIRE(!std::is_copy_constructible<beyond::optional<T>>::value);
    REQUIRE(!std::is_copy_assignable<beyond::optional<T>>::value);
    REQUIRE(std::is_move_constructible<beyond::optional<T>>::value);
    REQUIRE(std::is_move_assignable<beyond::optional<T>>::value);
  }

  {
    struct T {
      T(const T&) = default;
      T(T&&) = delete;
      T& operator=(const T&) = default;
      T& operator=(T&&) = delete;
    };
    REQUIRE(std::is_copy_constructible<beyond::optional<T>>::value);
    REQUIRE(std::is_copy_assignable<beyond::optional<T>>::value);
  }
}

TEST_CASE("optional Constexpr", "[beyond.core.types.optional]")
{
  SECTION("empty construct")
  {
    constexpr beyond::optional<int> o2{};
    constexpr beyond::optional<int> o3 = {};
    constexpr beyond::optional<int> o4 = beyond::nullopt;
    constexpr beyond::optional<int> o5 = {beyond::nullopt};
    constexpr beyond::optional<int> o6(beyond::nullopt);

    STATIC_REQUIRE(!o2);
    STATIC_REQUIRE(!o3);
    STATIC_REQUIRE(!o4);
    STATIC_REQUIRE(!o5);
    STATIC_REQUIRE(!o6);
  }

  SECTION("value construct")
  {
    constexpr beyond::optional<int> o1 = 42;
    constexpr beyond::optional<int> o2{42};
    constexpr beyond::optional<int> o3(42);
    constexpr beyond::optional<int> o4 = {42};
    constexpr int i = 42;
    constexpr beyond::optional<int> o5 = std::move(i);
    constexpr beyond::optional<int> o6{std::move(i)};
    constexpr beyond::optional<int> o7(std::move(i));
    constexpr beyond::optional<int> o8 = {std::move(i)};

    STATIC_REQUIRE(*o1 == 42);
    STATIC_REQUIRE(*o2 == 42);
    STATIC_REQUIRE(*o3 == 42);
    STATIC_REQUIRE(*o4 == 42);
    STATIC_REQUIRE(*o5 == 42);
    STATIC_REQUIRE(*o6 == 42);
    STATIC_REQUIRE(*o7 == 42);
    STATIC_REQUIRE(*o8 == 42);
  }
}

namespace {

struct foo {
  foo() = default;
  foo(foo&) = delete;
  foo(foo&&){};
};

} // anonymous namespace

TEST_CASE("optional Constructors", "[beyond.core.types.optional]")
{
  beyond::optional<int> o1;
  REQUIRE(!o1);

  beyond::optional<int> o2 = beyond::nullopt;
  REQUIRE(!o2);

  beyond::optional<int> o3 = 42;
  REQUIRE(*o3 == 42);

  beyond::optional<int> o4 = o3;
  REQUIRE(*o4 == 42);

  beyond::optional<int> o5 = o1;
  REQUIRE(!o5);

  beyond::optional<int> o6 = std::move(o3);
  REQUIRE(*o6 == 42);

  beyond::optional<short> o7 = 42;
  REQUIRE(*o7 == 42);

  beyond::optional<int> o8 = o7;
  REQUIRE(*o8 == 42);

  beyond::optional<int> o9 = std::move(o7);
  REQUIRE(*o9 == 42);

  {
    beyond::optional<int&> o;
    REQUIRE(!o);

    beyond::optional<int&> oo = o;
    REQUIRE(!oo);
  }

  {
    auto i = 42;
    beyond::optional<int&> o = i;
    REQUIRE(o);
    REQUIRE(*o == 42);

    beyond::optional<int&> oo = o;
    REQUIRE(oo);
    REQUIRE(*oo == 42);
  }

  std::vector<foo> v;
  v.emplace_back();
  beyond::optional<std::vector<foo>> ov = std::move(v);
  REQUIRE(ov->size() == 1);
}

TEST_CASE("optional Emplace", "[beyond.core.types.optional]")
{
  beyond::optional<std::pair<std::pair<int, int>, std::pair<double, double>>> i;
  i.emplace(std::piecewise_construct, std::make_tuple(0, 2),
            std::make_tuple(3, 4));
  REQUIRE(i->first.first == 0);
  REQUIRE(i->first.second == 2);
  REQUIRE(i->second.first == 3);
  REQUIRE(i->second.second == 4);
}

namespace {
constexpr int get_int(int)
{
  return 42;
}
constexpr beyond::optional<int> get_opt_int(int)
{
  return 42;
}

} // anonymous namespace

TEST_CASE("optional Monadic operations", "[beyond.core.types.optional]")
{
  SECTION("map")
  { // lhs is empty
    beyond::optional<int> o1;
    auto o1r = o1.map([](int i) { return i + 2; });
    STATIC_REQUIRE((std::is_same<decltype(o1r), beyond::optional<int>>::value));
    REQUIRE(!o1r);

    // lhs has value
    beyond::optional<int> o2 = 40;
    auto o2r = o2.map([](int i) { return i + 2; });
    STATIC_REQUIRE((std::is_same<decltype(o2r), beyond::optional<int>>::value));
    REQUIRE(o2r.value() == 42);

    struct rval_call_map {
      double operator()(int) &&
      {
        return 42.0;
      };
    };

    // ensure that function object is forwarded
    beyond::optional<int> o3 = 42;
    auto o3r = o3.map(rval_call_map{});
    STATIC_REQUIRE(
        (std::is_same<decltype(o3r), beyond::optional<double>>::value));
    REQUIRE(o3r.value() == 42);

    // ensure that lhs is forwarded
    beyond::optional<int> o4 = 40;
    auto o4r = std::move(o4).map([](int&& i) { return i + 2; });
    STATIC_REQUIRE((std::is_same<decltype(o4r), beyond::optional<int>>::value));
    REQUIRE(o4r.value() == 42);

    // ensure that lhs is const-propagated
    const beyond::optional<int> o5 = 40;
    auto o5r = o5.map([](const int& i) { return i + 2; });
    STATIC_REQUIRE((std::is_same<decltype(o5r), beyond::optional<int>>::value));
    REQUIRE(o5r.value() == 42);

    // test void return
    beyond::optional<int> o7 = 40;
    auto f7 = [](const int&) { return; };
    auto o7r = o7.map(f7);
    STATIC_REQUIRE(
        (std::is_same<decltype(o7r),
                      beyond::optional<beyond::monostate_t>>::value));
    REQUIRE(o7r.has_value());

    // test each overload in turn
    beyond::optional<int> o8 = 42;
    auto o8r = o8.map([](int) { return 42; });
    REQUIRE(*o8r == 42);

    beyond::optional<int> o9 = 42;
    auto o9r = o9.map([](int) { return; });
    REQUIRE(o9r);

    beyond::optional<int> o12 = 42;
    auto o12r = std::move(o12).map([](int) { return 42; });
    REQUIRE(*o12r == 42);

    beyond::optional<int> o13 = 42;
    auto o13r = std::move(o13).map([](int) { return; });
    REQUIRE(o13r);

    const beyond::optional<int> o16 = 42;
    auto o16r = o16.map([](int) { return 42; });
    REQUIRE(*o16r == 42);

    const beyond::optional<int> o17 = 42;
    auto o17r = o17.map([](int) { return; });
    REQUIRE(o17r);

    const beyond::optional<int> o20 = 42;
    auto o20r = std::move(o20).map([](int) { return 42; });
    REQUIRE(*o20r == 42);

    const beyond::optional<int> o21 = 42;
    auto o21r = std::move(o21).map([](int) { return; });
    REQUIRE(o21r);

    beyond::optional<int> o24 = beyond::nullopt;
    auto o24r = o24.map([](int) { return 42; });
    REQUIRE(!o24r);

    beyond::optional<int> o25 = beyond::nullopt;
    auto o25r = o25.map([](int) { return; });
    REQUIRE(!o25r);

    beyond::optional<int> o28 = beyond::nullopt;
    auto o28r = std::move(o28).map([](int) { return 42; });
    REQUIRE(!o28r);

    beyond::optional<int> o29 = beyond::nullopt;
    auto o29r = std::move(o29).map([](int) { return; });
    REQUIRE(!o29r);

    const beyond::optional<int> o32 = beyond::nullopt;
    auto o32r = o32.map([](int) { return 42; });
    REQUIRE(!o32r);

    const beyond::optional<int> o33 = beyond::nullopt;
    auto o33r = o33.map([](int) { return; });
    REQUIRE(!o33r);

    const beyond::optional<int> o36 = beyond::nullopt;
    auto o36r = std::move(o36).map([](int) { return 42; });
    REQUIRE(!o36r);

    const beyond::optional<int> o37 = beyond::nullopt;
    auto o37r = std::move(o37).map([](int) { return; });
    REQUIRE(!o37r);

    // callable which returns a reference
    beyond::optional<int> o38 = 42;
    auto o38r = o38.map([](int& i) -> const int& { return i; });
    REQUIRE(o38r);
    REQUIRE(*o38r == 42);

    int i = 42;
    beyond::optional<int&> o39 = i;
    o39.map([](int& x) { x = 12; });
    REQUIRE(i == 12);
  }

  SECTION("map constexpr")
  {
    /*
     * TODO: enable when C++20
    // test each overload in turn
    constexpr beyond::optional<int> o16 = 42;
    constexpr auto o16r = o16.map(get_int);
    STATIC_REQUIRE(*o16r == 42);

    constexpr beyond::optional<int> o20 = 42;
    constexpr auto o20r = std::move(o20).map(get_int);
    STATIC_REQUIRE(*o20r == 42);

    constexpr beyond::optional<int> o32 = beyond::nullopt;
    constexpr auto o32r = o32.map(get_int);
    STATIC_REQUIRE(!o32r);
    constexpr beyond::optional<int> o36 = beyond::nullopt;
    constexpr auto o36r = std::move(o36).map(get_int);
    STATIC_REQUIRE(!o36r);
    */
  }

  SECTION("transform")
  { // lhs is empty
    beyond::optional<int> o1;
    auto o1r = o1.transform([](int i) { return i + 2; });
    STATIC_REQUIRE((std::is_same<decltype(o1r), beyond::optional<int>>::value));
    REQUIRE(!o1r);

    // lhs has value
    beyond::optional<int> o2 = 40;
    auto o2r = o2.transform([](int i) { return i + 2; });
    STATIC_REQUIRE((std::is_same<decltype(o2r), beyond::optional<int>>::value));
    REQUIRE(o2r.value() == 42);

    struct rval_call_transform {
      double operator()(int) &&
      {
        return 42.0;
      };
    };

    // ensure that function object is forwarded
    beyond::optional<int> o3 = 42;
    auto o3r = o3.transform(rval_call_transform{});
    STATIC_REQUIRE(
        (std::is_same<decltype(o3r), beyond::optional<double>>::value));
    REQUIRE(o3r.value() == 42);

    // ensure that lhs is forwarded
    beyond::optional<int> o4 = 40;
    auto o4r = std::move(o4).transform([](int&& i) { return i + 2; });
    STATIC_REQUIRE((std::is_same<decltype(o4r), beyond::optional<int>>::value));
    REQUIRE(o4r.value() == 42);

    // ensure that lhs is const-propagated
    const beyond::optional<int> o5 = 40;
    auto o5r = o5.transform([](const int& i) { return i + 2; });
    STATIC_REQUIRE((std::is_same<decltype(o5r), beyond::optional<int>>::value));
    REQUIRE(o5r.value() == 42);

    // test void return
    beyond::optional<int> o7 = 40;
    auto f7 = [](const int&) { return; };
    auto o7r = o7.transform(f7);
    STATIC_REQUIRE(
        (std::is_same<decltype(o7r),
                      beyond::optional<beyond::monostate_t>>::value));
    REQUIRE(o7r.has_value());

    // test each overload in turn
    beyond::optional<int> o8 = 42;
    auto o8r = o8.transform([](int) { return 42; });
    REQUIRE(*o8r == 42);

    beyond::optional<int> o9 = 42;
    auto o9r = o9.transform([](int) { return; });
    REQUIRE(o9r);

    beyond::optional<int> o12 = 42;
    auto o12r = std::move(o12).transform([](int) { return 42; });
    REQUIRE(*o12r == 42);

    beyond::optional<int> o13 = 42;
    auto o13r = std::move(o13).transform([](int) { return; });
    REQUIRE(o13r);

    const beyond::optional<int> o16 = 42;
    auto o16r = o16.transform([](int) { return 42; });
    REQUIRE(*o16r == 42);

    const beyond::optional<int> o17 = 42;
    auto o17r = o17.transform([](int) { return; });
    REQUIRE(o17r);

    const beyond::optional<int> o20 = 42;
    auto o20r = std::move(o20).transform([](int) { return 42; });
    REQUIRE(*o20r == 42);

    const beyond::optional<int> o21 = 42;
    auto o21r = std::move(o21).transform([](int) { return; });
    REQUIRE(o21r);

    beyond::optional<int> o24 = beyond::nullopt;
    auto o24r = o24.transform([](int) { return 42; });
    REQUIRE(!o24r);

    beyond::optional<int> o25 = beyond::nullopt;
    auto o25r = o25.transform([](int) { return; });
    REQUIRE(!o25r);

    beyond::optional<int> o28 = beyond::nullopt;
    auto o28r = std::move(o28).transform([](int) { return 42; });
    REQUIRE(!o28r);

    beyond::optional<int> o29 = beyond::nullopt;
    auto o29r = std::move(o29).transform([](int) { return; });
    REQUIRE(!o29r);

    const beyond::optional<int> o32 = beyond::nullopt;
    auto o32r = o32.transform([](int) { return 42; });
    REQUIRE(!o32r);

    const beyond::optional<int> o33 = beyond::nullopt;
    auto o33r = o33.transform([](int) { return; });
    REQUIRE(!o33r);

    const beyond::optional<int> o36 = beyond::nullopt;
    auto o36r = std::move(o36).transform([](int) { return 42; });
    REQUIRE(!o36r);

    const beyond::optional<int> o37 = beyond::nullopt;
    auto o37r = std::move(o37).transform([](int) { return; });
    REQUIRE(!o37r);

    // callable which returns a reference
    beyond::optional<int> o38 = 42;
    auto o38r = o38.transform([](int& i) -> const int& { return i; });
    REQUIRE(o38r);
    REQUIRE(*o38r == 42);

    int i = 42;
    beyond::optional<int&> o39 = i;
    o39.transform([](int& x) { x = 12; });
    REQUIRE(i == 12);
  }

  SECTION("transform constexpr")
  {
    /*
     * TODO: enable when C++20
    // test each overload in turn
    constexpr beyond::optional<int> o16 = 42;
    constexpr auto o16r = o16.transform(get_int);
    STATIC_REQUIRE(*o16r == 42);

    constexpr beyond::optional<int> o20 = 42;
    constexpr auto o20r = std::move(o20).transform(get_int);
    STATIC_REQUIRE(*o20r == 42);

    constexpr beyond::optional<int> o32 = beyond::nullopt;
    constexpr auto o32r = o32.transform(get_int);
    STATIC_REQUIRE(!o32r);
    constexpr beyond::optional<int> o36 = beyond::nullopt;
    constexpr auto o36r = std::move(o36).transform(get_int);
    STATIC_REQUIRE(!o36r);
    */
  }

  SECTION("and_then")
  {

    // lhs is empty
    beyond::optional<int> o1;
    auto o1r = o1.and_then([](int) { return beyond::optional<float>{42}; });
    STATIC_REQUIRE(
        (std::is_same<decltype(o1r), beyond::optional<float>>::value));
    REQUIRE(!o1r);

    // lhs has value
    beyond::optional<int> o2 = 12;
    auto o2r = o2.and_then([](int) { return beyond::optional<float>{42}; });
    STATIC_REQUIRE(
        (std::is_same<decltype(o2r), beyond::optional<float>>::value));
    REQUIRE(o2r.value() == 42.f);

    // lhs is empty, rhs returns empty
    beyond::optional<int> o3;
    auto o3r = o3.and_then([](int) { return beyond::optional<float>{}; });
    STATIC_REQUIRE(
        (std::is_same<decltype(o3r), beyond::optional<float>>::value));
    REQUIRE(!o3r);

    // rhs returns empty
    beyond::optional<int> o4 = 12;
    auto o4r = o4.and_then([](int) { return beyond::optional<float>{}; });
    STATIC_REQUIRE(
        (std::is_same<decltype(o4r), beyond::optional<float>>::value));
    REQUIRE(!o4r);

    struct rval_call_and_then {
      beyond::optional<double> operator()(int) &&
      {
        return beyond::optional<double>(42.0);
      };
    };

    // ensure that function object is forwarded
    beyond::optional<int> o5 = 42;
    auto o5r = o5.and_then(rval_call_and_then{});
    STATIC_REQUIRE(
        (std::is_same<decltype(o5r), beyond::optional<double>>::value));
    REQUIRE(o5r.value() == 42);

    // ensure that lhs is forwarded
    beyond::optional<int> o6 = 42;
    auto o6r = std::move(o6).and_then(
        [](int&& i) { return beyond::optional<double>(i); });
    STATIC_REQUIRE(
        (std::is_same<decltype(o6r), beyond::optional<double>>::value));
    REQUIRE(o6r.value() == 42);

    // ensure that function object is const-propagated
    const beyond::optional<int> o7 = 42;
    auto o7r =
        o7.and_then([](const int& i) { return beyond::optional<double>(i); });
    STATIC_REQUIRE(
        (std::is_same<decltype(o7r), beyond::optional<double>>::value));
    REQUIRE(o7r.value() == 42);

    // test each overload in turn
    beyond::optional<int> o8 = 42;
    auto o8r = o8.and_then([](int) { return beyond::make_optional(42); });
    REQUIRE(*o8r == 42);

    beyond::optional<int> o9 = 42;
    auto o9r =
        std::move(o9).and_then([](int) { return beyond::make_optional(42); });
    REQUIRE(*o9r == 42);

    const beyond::optional<int> o10 = 42;
    auto o10r = o10.and_then([](int) { return beyond::make_optional(42); });
    REQUIRE(*o10r == 42);

    const beyond::optional<int> o11 = 42;
    auto o11r =
        std::move(o11).and_then([](int) { return beyond::make_optional(42); });
    REQUIRE(*o11r == 42);

    beyond::optional<int> o16 = beyond::nullopt;
    auto o16r = o16.and_then([](int) { return beyond::make_optional(42); });
    REQUIRE(!o16r);

    beyond::optional<int> o17 = beyond::nullopt;
    auto o17r =
        std::move(o17).and_then([](int) { return beyond::make_optional(42); });
    REQUIRE(!o17r);

    const beyond::optional<int> o18 = beyond::nullopt;
    auto o18r = o18.and_then([](int) { return beyond::make_optional(42); });
    REQUIRE(!o18r);

    const beyond::optional<int> o19 = beyond::nullopt;
    auto o19r =
        std::move(o19).and_then([](int) { return beyond::make_optional(42); });
    REQUIRE(!o19r);

    int i = 3;
    beyond::optional<int&> o20{i};
    std::move(o20).and_then([](int& r) { return beyond::optional<int&>{++r}; });
    REQUIRE(o20);
    REQUIRE(i == 4);
  }

  SECTION("constexpr and_then")
  {
    /*
     * TODO: enable when C++20
    constexpr beyond::optional<int> o10 = 42;
    constexpr auto o10r = o10.and_then(get_opt_int);
    REQUIRE(*o10r == 42);

    constexpr beyond::optional<int> o11 = 42;
    constexpr auto o11r = std::move(o11).and_then(get_opt_int);
    REQUIRE(*o11r == 42);

    constexpr beyond::optional<int> o18 = beyond::nullopt;
    constexpr auto o18r = o18.and_then(get_opt_int);
    REQUIRE(!o18r);

    constexpr beyond::optional<int> o19 = beyond::nullopt;
    constexpr auto o19r = std::move(o19).and_then(get_opt_int);
    REQUIRE(!o19r);
    */
  }

  SECTION("or else")
  {
    beyond::optional<int> o1 = 42;
    REQUIRE(*(o1.or_else([] { return beyond::make_optional(13); })) == 42);

    beyond::optional<int> o2;
    REQUIRE(*(o2.or_else([] { return beyond::make_optional(13); })) == 13);
  }

  SECTION("disjunction")
  {
    beyond::optional<int> o1 = 42;
    beyond::optional<int> o2 = 12;
    beyond::optional<int> o3;

    REQUIRE(*o1.disjunction(o2) == 42);
    REQUIRE(*o1.disjunction(o3) == 42);
    REQUIRE(*o2.disjunction(o1) == 12);
    REQUIRE(*o2.disjunction(o3) == 12);
    REQUIRE(*o3.disjunction(o1) == 42);
    REQUIRE(*o3.disjunction(o2) == 12);
  }

  SECTION("conjunction")
  {
    beyond::optional<int> o1 = 42;
    REQUIRE(*o1.conjunction(42.0) == 42.0);
    REQUIRE(*o1.conjunction(std::string{"hello"}) == std::string{"hello"});

    beyond::optional<int> o2;
    REQUIRE(!o2.conjunction(42.0));
    REQUIRE(!o2.conjunction(std::string{"hello"}));
  }

  SECTION("map_or")
  {
    beyond::optional<int> o1 = 21;
    REQUIRE((o1.map_or([](int x) { return x * 2; }, 13)) == 42);

    beyond::optional<int> o2;
    REQUIRE((o2.map_or([](int x) { return x * 2; }, 13)) == 13);
  }

  SECTION("map_or_else")
  {
    beyond::optional<int> o1 = 21;
    REQUIRE((o1.map_or_else([](int x) { return x * 2; }, [] { return 13; })) ==
            42);

    beyond::optional<int> o2;
    REQUIRE((o2.map_or_else([](int x) { return x * 2; }, [] { return 13; })) ==
            13);
  }

  SECTION("take")
  {
    beyond::optional<int> o1 = 42;
    REQUIRE(*o1.take() == 42);
    REQUIRE(!o1);

    beyond::optional<int> o2;
    REQUIRE(!o2.take());
    REQUIRE(!o2);
  }

  struct foo {
    void non_const() {}
  };

  SECTION("Issue #1")
  {
    beyond::optional<foo> f = foo{};
    auto l = [](auto&& x) { x.non_const(); };
    f.map(l);
  }

  struct overloaded {
    beyond::optional<int> operator()(foo&)
    {
      return 0;
    }
    beyond::optional<std::string> operator()(const foo&)
    {
      return "";
    }
  };

  SECTION("Issue #2")
  {
    beyond::optional<foo> f = foo{};
    auto x = f.and_then(overloaded{});
  }
};

TEST_CASE("optional Hashing", "[beyond.core.types.optional]")
{
  const auto hash = std::hash<beyond::optional<int>>{};
  beyond::optional<int> op1;
  REQUIRE(hash(op1) == 0);

  const int value = 42;
  beyond::optional<int> op2{value};
  REQUIRE(hash(op2) == std::hash<int>{}(value));
}

namespace {

struct takes_init_and_variadic {
  std::vector<int> v;
  std::tuple<int, int> t;
  template <class... Args>
  takes_init_and_variadic(std::initializer_list<int> l, Args&&... args)
      : v(l), t(std::forward<Args>(args)...)
  {
  }
};

} // anonymous namespace

TEST_CASE("optional In place", "[beyond.core.types.optional]")
{
  beyond::optional<int> o1{beyond::in_place};
  beyond::optional<int> o2(beyond::in_place);
  REQUIRE(o1);
  REQUIRE(o1 == 0);
  REQUIRE(o2);
  REQUIRE(o2 == 0);

  beyond::optional<int> o3(beyond::in_place, 42);
  REQUIRE(o3 == 42);

  beyond::optional<std::tuple<int, int>> o4(beyond::in_place, 0, 1);
  REQUIRE(o4);
  REQUIRE(std::get<0>(*o4) == 0);
  REQUIRE(std::get<1>(*o4) == 1);

  beyond::optional<std::vector<int>> o5(beyond::in_place, {0, 1});
  REQUIRE(o5);
  REQUIRE((*o5)[0] == 0);
  REQUIRE((*o5)[1] == 1);

  beyond::optional<takes_init_and_variadic> o6(beyond::in_place, {0, 1}, 2, 3);
  REQUIRE(o6->v[0] == 0);
  REQUIRE(o6->v[1] == 1);
  REQUIRE(std::get<0>(o6->t) == 2);
  REQUIRE(std::get<1>(o6->t) == 3);
}

namespace {

struct foo2 {
  int& v()
  {
    return i;
  }
  int i = 0;
};

int& x(int& i)
{
  i = 42;
  return i;
}

} // anonymous namespace

TEST_CASE(
    " optional `.map` with a callable returning a reference should return an "
    "optional reference",
    "[beyond.core.types.optional]")
{
  beyond::optional<foo2> f = foo2{};
  auto v = f.map(&foo2::v).map(x);
  static_assert(std::is_same<decltype(v), beyond::optional<int&>>::value,
                "Must return a reference");
  REQUIRE(f->i == 42);
  REQUIRE(*v == 42);
  REQUIRE((&f->i) == (&*v));
}

TEST_CASE("optional Self assignment", "[beyond.core.types.optional]")
{
  struct fail_on_copy_self {
    int value;
    fail_on_copy_self(int v) : value(v) {}
    fail_on_copy_self(const fail_on_copy_self& other) : value(other.value)
    {
      REQUIRE(&other != this);
    }
  };

  beyond::optional<fail_on_copy_self> o = fail_on_copy_self(42);

  o = o;
  REQUIRE(o->value == 42);
}

TEST_CASE("optional .emplace()", "[beyond.core.types.optional]")
{
  int i = 0;
  int j = 0;
  beyond::optional<int&> a = i;
  a.emplace(j);
  *a = 42;
  REQUIRE(j == 42);
  REQUIRE(*a == 42);
  REQUIRE(a.has_value());
}

TEST_CASE("optional make_optional", "[beyond.core.types.optional]")
{
  auto o1 = beyond::make_optional(42);
  auto o2 = beyond::optional<int>(42);

  constexpr bool is_same =
      std::is_same<decltype(o1), beyond::optional<int>>::value;
  REQUIRE(is_same);
  REQUIRE(o1 == o2);

  auto o3 = beyond::make_optional<std::tuple<int, int, int, int>>(0, 1, 2, 3);
  REQUIRE(std::get<0>(*o3) == 0);
  REQUIRE(std::get<1>(*o3) == 1);
  REQUIRE(std::get<2>(*o3) == 2);
  REQUIRE(std::get<3>(*o3) == 3);

  auto o4 = beyond::make_optional<std::vector<int>>({0, 1, 2, 3});
  REQUIRE(o4.value()[0] == 0);
  REQUIRE(o4.value()[1] == 1);
  REQUIRE(o4.value()[2] == 2);
  REQUIRE(o4.value()[3] == 3);

  auto o5 = beyond::make_optional<takes_init_and_variadic>({0, 1}, 2, 3);
  REQUIRE(o5->v[0] == 0);
  REQUIRE(o5->v[1] == 1);
  REQUIRE(std::get<0>(o5->t) == 2);
  REQUIRE(std::get<1>(o5->t) == 3);

  auto i = 42;
  auto o6 = beyond::make_optional<int&>(i);
  REQUIRE((std::is_same<decltype(o6), beyond::optional<int&>>::value));
  REQUIRE(o6);
  REQUIRE(*o6 == 42);
}

TEST_CASE("optional Noexcept", "[beyond.core.types.optional]")
{
  beyond::optional<int> o1{4};
  beyond::optional<int> o2{42};

  SECTION("comparison with nullopt")
  {
    REQUIRE(noexcept(o1 == beyond::nullopt));
    REQUIRE(noexcept(beyond::nullopt == o1));
    REQUIRE(noexcept(o1 != beyond::nullopt));
    REQUIRE(noexcept(beyond::nullopt != o1));
    REQUIRE(noexcept(o1 < beyond::nullopt));
    REQUIRE(noexcept(beyond::nullopt < o1));
    REQUIRE(noexcept(o1 <= beyond::nullopt));
    REQUIRE(noexcept(beyond::nullopt <= o1));
    REQUIRE(noexcept(o1 > beyond::nullopt));
    REQUIRE(noexcept(beyond::nullopt > o1));
    REQUIRE(noexcept(o1 >= beyond::nullopt));
    REQUIRE(noexcept(beyond::nullopt >= o1));
  }

  SECTION("swap")
  {
    REQUIRE(noexcept(swap(o1, o2)) == noexcept(o1.swap(o2)));

    struct nothrow_swappable {
      nothrow_swappable& swap(const nothrow_swappable&) noexcept
      {
        return *this;
      }
    };

    beyond::optional<nothrow_swappable> ont;

    REQUIRE(noexcept(ont.swap(ont)));
  }

  SECTION("constructors")
  {
    REQUIRE(noexcept(beyond::optional<int>{}));
    REQUIRE(noexcept(beyond::optional<int>{beyond::nullopt}));

    struct nothrow_move {
      nothrow_move(nothrow_move&&) noexcept = default;
    };

    using nothrow_opt = beyond::optional<nothrow_move>;

    REQUIRE(std::is_nothrow_move_constructible<nothrow_opt>::value);
  }

  SECTION("assignment")
  {
    REQUIRE(noexcept(o1 = beyond::nullopt));

    struct nothrow_move_assign {
      nothrow_move_assign() = default;
      nothrow_move_assign(nothrow_move_assign&&) noexcept = default;
      nothrow_move_assign& operator=(const nothrow_move_assign&) = default;
    };
    using nothrow_opt = beyond::optional<nothrow_move_assign>;

    REQUIRE(
        noexcept(std::declval<nothrow_opt>() = std::declval<nothrow_opt>()));
  }

  SECTION("observers")
  {
    REQUIRE(noexcept(static_cast<bool>(o1)));
    REQUIRE(noexcept(o1.has_value()));
  }

  SECTION("modifiers")
  {
    REQUIRE(noexcept(o1.reset()));
  }
}

TEST_CASE("optional nullopt_t", "[beyond.core.types.optional]")
{
  beyond::optional<int> o1 = beyond::nullopt;
  beyond::optional<int> o2{beyond::nullopt};
  beyond::optional<int> o3(beyond::nullopt);
  beyond::optional<int> o4 = {beyond::nullopt};

  REQUIRE(!o1);
  REQUIRE(!o2);
  REQUIRE(!o3);
  REQUIRE(!o4);

  REQUIRE(!std::is_default_constructible<beyond::nullopt_t>::value);
}

TEST_CASE("optional Observers", "[beyond.core.types.optional]")
{
  struct move_detector {
    move_detector() = default;
    move_detector(move_detector&& rhs)
    {
      rhs.been_moved = true;
    }
    bool been_moved = false;
  };

  beyond::optional<int> o1 = 42;
  beyond::optional<int> o2;
  const beyond::optional<int> o3 = 42;

  REQUIRE(*o1 == 42);
  REQUIRE(*o1 == o1.value());
  REQUIRE(o2.value_or(42) == 42);
  REQUIRE(o3.value() == 42);
  auto success = std::is_same<decltype(o1.value()), int&>::value;
  REQUIRE(success);
  success = std::is_same<decltype(o3.value()), const int&>::value;
  REQUIRE(success);
  success = std::is_same<decltype(std::move(o1).value()), int&&>::value;
  REQUIRE(success);

  success = std::is_same<decltype(std::move(o3).value()), const int&&>::value;
  REQUIRE(success);

  beyond::optional<move_detector> o4{beyond::in_place};
  move_detector o5 = std::move(o4).value();
  REQUIRE(o4->been_moved);
  REQUIRE(!o5.been_moved);
}

TEST_CASE("optional Relational ops", "[beyond.core.types.optional]")
{
  beyond::optional<int> o1{4};
  beyond::optional<int> o2{42};
  beyond::optional<int> o3{};

  SECTION("self simple")
  {
    REQUIRE(!(o1 == o2));
    REQUIRE(o1 == o1);
    REQUIRE(o1 != o2);
    REQUIRE(!(o1 != o1));
    REQUIRE(o1 < o2);
    REQUIRE(!(o1 < o1));
    REQUIRE(!(o1 > o2));
    REQUIRE(!(o1 > o1));
    REQUIRE(o1 <= o2);
    REQUIRE(o1 <= o1);
    REQUIRE(!(o1 >= o2));
    REQUIRE(o1 >= o1);
  }

  SECTION("nullopt simple")
  {
    REQUIRE(!(o1 == beyond::nullopt));
    REQUIRE(!(beyond::nullopt == o1));
    REQUIRE(o1 != beyond::nullopt);
    REQUIRE(beyond::nullopt != o1);
    REQUIRE(!(o1 < beyond::nullopt));
    REQUIRE(beyond::nullopt < o1);
    REQUIRE(o1 > beyond::nullopt);
    REQUIRE(!(beyond::nullopt > o1));
    REQUIRE(!(o1 <= beyond::nullopt));
    REQUIRE(beyond::nullopt <= o1);
    REQUIRE(o1 >= beyond::nullopt);
    REQUIRE(!(beyond::nullopt >= o1));

    REQUIRE(o3 == beyond::nullopt);
    REQUIRE(beyond::nullopt == o3);
    REQUIRE(!(o3 != beyond::nullopt));
    REQUIRE(!(beyond::nullopt != o3));
    REQUIRE(!(o3 < beyond::nullopt));
    REQUIRE(!(beyond::nullopt < o3));
    REQUIRE(!(o3 > beyond::nullopt));
    REQUIRE(!(beyond::nullopt > o3));
    REQUIRE(o3 <= beyond::nullopt);
    REQUIRE(beyond::nullopt <= o3);
    REQUIRE(o3 >= beyond::nullopt);
    REQUIRE(beyond::nullopt >= o3);
  }

  SECTION("with T simple")
  {
    REQUIRE(!(o1 == 1));
    REQUIRE(!(1 == o1));
    REQUIRE(o1 != 1);
    REQUIRE(1 != o1);
    REQUIRE(!(o1 < 1));
    REQUIRE(1 < o1);
    REQUIRE(o1 > 1);
    REQUIRE(!(1 > o1));
    REQUIRE(!(o1 <= 1));
    REQUIRE(1 <= o1);
    REQUIRE(o1 >= 1);
    REQUIRE(!(1 >= o1));

    REQUIRE(o1 == 4);
    REQUIRE(4 == o1);
    REQUIRE(!(o1 != 4));
    REQUIRE(!(4 != o1));
    REQUIRE(!(o1 < 4));
    REQUIRE(!(4 < o1));
    REQUIRE(!(o1 > 4));
    REQUIRE(!(4 > o1));
    REQUIRE(o1 <= 4);
    REQUIRE(4 <= o1);
    REQUIRE(o1 >= 4);
    REQUIRE(4 >= o1);
  }

  beyond::optional<std::string> o4{"hello"};
  beyond::optional<std::string> o5{"xyz"};

  SECTION("self complex")
  {
    REQUIRE(!(o4 == o5));
    REQUIRE(o4 == o4);
    REQUIRE(o4 != o5);
    REQUIRE(!(o4 != o4));
    REQUIRE(o4 < o5);
    REQUIRE(!(o4 < o4));
    REQUIRE(!(o4 > o5));
    REQUIRE(!(o4 > o4));
    REQUIRE(o4 <= o5);
    REQUIRE(o4 <= o4);
    REQUIRE(!(o4 >= o5));
    REQUIRE(o4 >= o4);
  }

  SECTION("nullopt complex")
  {
    REQUIRE(!(o4 == beyond::nullopt));
    REQUIRE(!(beyond::nullopt == o4));
    REQUIRE(o4 != beyond::nullopt);
    REQUIRE(beyond::nullopt != o4);
    REQUIRE(!(o4 < beyond::nullopt));
    REQUIRE(beyond::nullopt < o4);
    REQUIRE(o4 > beyond::nullopt);
    REQUIRE(!(beyond::nullopt > o4));
    REQUIRE(!(o4 <= beyond::nullopt));
    REQUIRE(beyond::nullopt <= o4);
    REQUIRE(o4 >= beyond::nullopt);
    REQUIRE(!(beyond::nullopt >= o4));

    REQUIRE(o3 == beyond::nullopt);
    REQUIRE(beyond::nullopt == o3);
    REQUIRE(!(o3 != beyond::nullopt));
    REQUIRE(!(beyond::nullopt != o3));
    REQUIRE(!(o3 < beyond::nullopt));
    REQUIRE(!(beyond::nullopt < o3));
    REQUIRE(!(o3 > beyond::nullopt));
    REQUIRE(!(beyond::nullopt > o3));
    REQUIRE(o3 <= beyond::nullopt);
    REQUIRE(beyond::nullopt <= o3);
    REQUIRE(o3 >= beyond::nullopt);
    REQUIRE(beyond::nullopt >= o3);
  }

  SECTION("with T complex")
  {
    REQUIRE(!(o4 == "a"));
    REQUIRE(!("a" == o4));
    REQUIRE(o4 != "a");
    REQUIRE("a" != o4);
    REQUIRE(!(o4 < "a"));
    REQUIRE("a" < o4);
    REQUIRE(o4 > "a");
    REQUIRE(!("a" > o4));
    REQUIRE(!(o4 <= "a"));
    REQUIRE("a" <= o4);
    REQUIRE(o4 >= "a");
    REQUIRE(!("a" >= o4));

    REQUIRE(o4 == "hello");
    REQUIRE("hello" == o4);
    REQUIRE(!(o4 != "hello"));
    REQUIRE(!("hello" != o4));
    REQUIRE(!(o4 < "hello"));
    REQUIRE(!("hello" < o4));
    REQUIRE(!(o4 > "hello"));
    REQUIRE(!("hello" > o4));
    REQUIRE(o4 <= "hello");
    REQUIRE("hello" <= o4);
    REQUIRE(o4 >= "hello");
    REQUIRE("hello" >= o4);
  }
}

TEST_CASE("optional swap", "[beyond.core.types.optional]")
{
  SECTION("Swap value")
  {
    beyond::optional<int> o1 = 42;
    beyond::optional<int> o2 = 12;
    o1.swap(o2);
    CHECK(o1.value() == 12);
    CHECK(o2.value() == 42);
  }

  SECTION("Swap value with null intialized")
  {
    beyond::optional<int> o1 = 42;
    beyond::optional<int> o2 = beyond::nullopt;
    o1.swap(o2);
    CHECK(!o1.has_value());
    CHECK(o2.value() == 42);
  }

  SECTION("Swap null intialized with value")
  {
    beyond::optional<int> o1 = beyond::nullopt;
    beyond::optional<int> o2 = 42;
    o1.swap(o2);
    CHECK(o1.value() == 42);
    CHECK(!o2.has_value());
  }
}