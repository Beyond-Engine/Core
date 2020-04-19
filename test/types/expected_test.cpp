#include "beyond/types/expected.hpp"
#include <catch2/catch.hpp>

#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

TEST_CASE("expected Triviality", "[beyond.core.types.expected]")
{
  STATIC_REQUIRE(
      std::is_trivially_copy_constructible<beyond::expected<int, int>>::value);
  STATIC_REQUIRE(
      std::is_trivially_copy_assignable<beyond::expected<int, int>>::value);
  STATIC_REQUIRE(
      std::is_trivially_move_constructible<beyond::expected<int, int>>::value);
  STATIC_REQUIRE(
      std::is_trivially_move_assignable<beyond::expected<int, int>>::value);
  STATIC_REQUIRE(
      std::is_trivially_destructible<beyond::expected<int, int>>::value);

  STATIC_REQUIRE(
      std::is_trivially_copy_constructible<beyond::expected<void, int>>::value);
  STATIC_REQUIRE(
      std::is_trivially_move_constructible<beyond::expected<void, int>>::value);
  STATIC_REQUIRE(
      std::is_trivially_destructible<beyond::expected<void, int>>::value);

  {
    struct T {
      T(const T&) = default;
      T(T&&) = default;
      T& operator=(const T&) = default;
      T& operator=(T&&) = default;
      ~T() = default;
    };
    STATIC_REQUIRE(
        std::is_trivially_copy_constructible<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(
        std::is_trivially_copy_assignable<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(
        std::is_trivially_move_constructible<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(
        std::is_trivially_move_assignable<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(
        std::is_trivially_destructible<beyond::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T&) {}
      T(T&&){};
      T& operator=(const T&) {}
      T& operator=(T&&){};
      ~T() {}
    };
    STATIC_REQUIRE(
        !std::is_trivially_copy_constructible<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(
        !std::is_trivially_copy_assignable<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(
        !std::is_trivially_move_constructible<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(
        !std::is_trivially_move_assignable<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(
        !std::is_trivially_destructible<beyond::expected<T, int>>::value);
  }
}

TEST_CASE("expected Deletion", "[beyond.core.types.expected]")
{
  STATIC_REQUIRE(std::is_copy_constructible<beyond::expected<int, int>>::value);
  STATIC_REQUIRE(std::is_copy_assignable<beyond::expected<int, int>>::value);
  STATIC_REQUIRE(std::is_move_constructible<beyond::expected<int, int>>::value);
  STATIC_REQUIRE(std::is_move_assignable<beyond::expected<int, int>>::value);
  STATIC_REQUIRE(std::is_destructible<beyond::expected<int, int>>::value);

  {
    struct T {
      T() = default;
    };
    STATIC_REQUIRE(
        std::is_default_constructible<beyond::expected<T, int>>::value);
  }

  {
    struct T {
      T(int) {}
    };
    STATIC_REQUIRE(
        !std::is_default_constructible<beyond::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T&) = default;
      T(T&&) = default;
      T& operator=(const T&) = default;
      T& operator=(T&&) = default;
      ~T() = default;
    };
    STATIC_REQUIRE(std::is_copy_constructible<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(std::is_copy_assignable<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(std::is_move_constructible<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(std::is_move_assignable<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(std::is_destructible<beyond::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T&) = delete;
      T(T&&) = delete;
      T& operator=(const T&) = delete;
      T& operator=(T&&) = delete;
    };
    STATIC_REQUIRE(
        !std::is_copy_constructible<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(!std::is_copy_assignable<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(
        !std::is_move_constructible<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(!std::is_move_assignable<beyond::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T&) = delete;
      T(T&&) = default;
      T& operator=(const T&) = delete;
      T& operator=(T&&) = default;
    };
    STATIC_REQUIRE(
        !std::is_copy_constructible<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(!std::is_copy_assignable<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(std::is_move_constructible<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(std::is_move_assignable<beyond::expected<T, int>>::value);
  }

  {
    struct T {
      T(const T&) = default;
      T(T&&) = delete;
      T& operator=(const T&) = default;
      T& operator=(T&&) = delete;
    };
    STATIC_REQUIRE(std::is_copy_constructible<beyond::expected<T, int>>::value);
    STATIC_REQUIRE(std::is_copy_assignable<beyond::expected<T, int>>::value);
  }

  {
    beyond::expected<int, int> e;
    STATIC_REQUIRE(std::is_default_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_move_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    STATIC_REQUIRE(std::is_move_assignable<decltype(e)>::value);
    STATIC_REQUIRE(std::is_trivially_copy_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_trivially_copy_assignable<decltype(e)>::value);
    STATIC_REQUIRE(std::is_trivially_move_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    beyond::expected<int, std::string> e;
    STATIC_REQUIRE(std::is_default_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_move_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    STATIC_REQUIRE(std::is_move_assignable<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_copy_constructible<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_copy_assignable<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    beyond::expected<std::string, int> e;
    STATIC_REQUIRE(std::is_default_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_move_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    STATIC_REQUIRE(std::is_move_assignable<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_copy_constructible<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_copy_assignable<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    beyond::expected<std::string, std::string> e;
    STATIC_REQUIRE(std::is_default_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_move_constructible<decltype(e)>::value);
    STATIC_REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    STATIC_REQUIRE(std::is_move_assignable<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_copy_constructible<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_copy_assignable<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    STATIC_REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
  }
}

TEST_CASE("expected Simple assignment", "[beyond.core.types.expected]")
{
  beyond::expected<int, int> e1 = 42;
  beyond::expected<int, int> e2 = 17;
  beyond::expected<int, int> e3 = 21;
  beyond::expected<int, int> e4 = beyond::make_unexpected(42);
  beyond::expected<int, int> e5 = beyond::make_unexpected(17);
  beyond::expected<int, int> e6 = beyond::make_unexpected(21);

  e1 = e2;
  REQUIRE(e1);
  REQUIRE(*e1 == 17);
  REQUIRE(e2);
  REQUIRE(*e2 == 17);

  e1 = std::move(e2);
  REQUIRE(e1);
  REQUIRE(*e1 == 17);
  REQUIRE(e2);
  REQUIRE(*e2 == 17);

  e1 = 42;
  REQUIRE(e1);
  REQUIRE(*e1 == 42);

  auto unex = beyond::make_unexpected(12);
  e1 = unex;
  REQUIRE(!e1);
  REQUIRE(e1.error() == 12);

  e1 = beyond::make_unexpected(42);
  REQUIRE(!e1);
  REQUIRE(e1.error() == 42);

  e1 = e3;
  REQUIRE(e1);
  REQUIRE(*e1 == 21);

  e4 = e5;
  REQUIRE(!e4);
  REQUIRE(e4.error() == 17);

  e4 = std::move(e6);
  REQUIRE(!e4);
  REQUIRE(e4.error() == 21);

  e4 = e1;
  REQUIRE(e4);
  REQUIRE(*e4 == 21);
}

TEST_CASE("expected Assignment deletion", "[beyond.core.types.expected]")
{
  struct has_all {
    has_all() = default;
    has_all(const has_all&) = default;
    has_all(has_all&&) noexcept = default;
    has_all& operator=(const has_all&) = default;
  };

  beyond::expected<has_all, has_all> e1 = {};
  beyond::expected<has_all, has_all> e2 = {};
  e1 = e2;

  struct except_move {
    except_move() = default;
    except_move(const except_move&) = default;
    except_move(except_move&&) noexcept(false){};
    except_move& operator=(const except_move&) = default;
  };
  beyond::expected<except_move, except_move> e3 = {};
  beyond::expected<except_move, except_move> e4 = {};
  // e3 = e4; should not compile
}

struct takes_init_and_variadic {
  std::vector<int> v;
  std::tuple<int, int> t;
  template <class... Args>
  takes_init_and_variadic(std::initializer_list<int> l, Args&&... args)
      : v(l), t(std::forward<Args>(args)...)
  {
  }
};

TEST_CASE("expected Constructors", "[beyond.core.types.expected]")
{
  {
    beyond::expected<int, int> e;
    REQUIRE(e);
    REQUIRE(e == 0);
  }

  {
    beyond::expected<int, int> e = beyond::make_unexpected(0);
    REQUIRE(!e);
    REQUIRE(e.error() == 0);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 0);
    REQUIRE(!e);
    REQUIRE(e.error() == 0);
  }

  {
    beyond::expected<int, int> e(beyond::in_place, 42);
    REQUIRE(e);
    REQUIRE(e == 42);
  }

  {
    beyond::expected<std::vector<int>, int> e(beyond::in_place, {0, 1});
    REQUIRE(e);
    REQUIRE((*e)[0] == 0);
    REQUIRE((*e)[1] == 1);
  }

  {
    beyond::expected<std::tuple<int, int>, int> e(beyond::in_place, 0, 1);
    REQUIRE(e);
    REQUIRE(std::get<0>(*e) == 0);
    REQUIRE(std::get<1>(*e) == 1);
  }

  {
    beyond::expected<takes_init_and_variadic, int> e(beyond::in_place, {0, 1},
                                                     2, 3);
    REQUIRE(e);
    REQUIRE(e->v[0] == 0);
    REQUIRE(e->v[1] == 1);
    REQUIRE(std::get<0>(e->t) == 2);
    REQUIRE(std::get<1>(e->t) == 3);
  }

  {
    beyond::expected<int, int> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(std::is_trivially_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_trivially_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_trivially_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_trivially_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    beyond::expected<int, std::string> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    beyond::expected<std::string, int> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    beyond::expected<std::string, std::string> e;
    REQUIRE(std::is_default_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_constructible<decltype(e)>::value);
    REQUIRE(std::is_move_constructible<decltype(e)>::value);
    REQUIRE(std::is_copy_assignable<decltype(e)>::value);
    REQUIRE(std::is_move_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_copy_assignable<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_constructible<decltype(e)>::value);
    REQUIRE(!std::is_trivially_move_assignable<decltype(e)>::value);
  }

  {
    beyond::expected<void, int> e;
    REQUIRE(e);
  }

  {
    beyond::expected<void, int> e(beyond::unexpect, 42);
    REQUIRE(!e);
    REQUIRE(e.error() == 42);
  }
}

TEST_CASE("expected Emplace", "[beyond.core.types.expected]")
{
  {
    beyond::expected<std::unique_ptr<int>, int> e;
    e.emplace(new int{42});
    REQUIRE(e);
    REQUIRE(**e == 42);
  }

  {
    beyond::expected<std::vector<int>, int> e;
    e.emplace({0, 1});
    REQUIRE(e);
    REQUIRE((*e)[0] == 0);
    REQUIRE((*e)[1] == 1);
  }

  {
    beyond::expected<std::tuple<int, int>, int> e;
    e.emplace(2, 3);
    REQUIRE(e);
    REQUIRE(std::get<0>(*e) == 2);
    REQUIRE(std::get<1>(*e) == 3);
  }

  {
    beyond::expected<takes_init_and_variadic, int> e =
        beyond::make_unexpected(0);
    e.emplace({0, 1}, 2, 3);
    REQUIRE(e);
    REQUIRE(e->v[0] == 0);
    REQUIRE(e->v[1] == 1);
    REQUIRE(std::get<0>(e->t) == 2);
    REQUIRE(std::get<1>(e->t) == 3);
  }
}

TEST_CASE("expected map()", "[beyond.core.types.expected]")
{
  auto mul2 = [](int a) { return a * 2; };
  auto ret_void = [](int a) {};

  {
    beyond::expected<int, int> e = 21;
    auto ret = e.map(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = e.map(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = std::move(e).map(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = std::move(e).map(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.map(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.map(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).map(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).map(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = e.map(ret_void);
    REQUIRE(ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), beyond::expected<void, int>>::value));
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = e.map(ret_void);
    REQUIRE(ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), beyond::expected<void, int>>::value));
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = std::move(e).map(ret_void);
    REQUIRE(ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), beyond::expected<void, int>>::value));
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = std::move(e).map(ret_void);
    REQUIRE(ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), beyond::expected<void, int>>::value));
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.map(ret_void);
    REQUIRE(!ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), beyond::expected<void, int>>::value));
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.map(ret_void);
    REQUIRE(!ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), beyond::expected<void, int>>::value));
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).map(ret_void);
    REQUIRE(!ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), beyond::expected<void, int>>::value));
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).map(ret_void);
    REQUIRE(!ret);
    STATIC_REQUIRE(
        (std::is_same<decltype(ret), beyond::expected<void, int>>::value));
  }

  // mapping functions which return references
  {
    beyond::expected<int, int> e(42);
    auto ret = e.map([](int& i) -> int& { return i; });
    REQUIRE(ret);
    REQUIRE(ret == 42);
  }

  {
    int i = 0;
    beyond::expected<void, int> a;
    a.map([&i] { i = 42; });
    REQUIRE(i == 42);

    auto x = a.map([] { return 42; });
    REQUIRE(*x == 42);
  }

  {
    struct non_copyable {
      non_copyable(non_copyable&&) = default;
      non_copyable(non_copyable const&) = delete;
      non_copyable() = default;
    };

    beyond::expected<non_copyable, int>{}.map([](non_copyable) {});
  }
}

TEST_CASE("expected map_error()", "[beyond.core.types.expected]")
{
  auto mul2 = [](int a) { return a * 2; };
  auto ret_void = [](int a) {};

  {
    beyond::expected<int, int> e = 21;
    auto ret = e.map_error(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = e.map_error(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = std::move(e).map_error(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = std::move(e).map_error(mul2);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.map_error(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 42);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.map_error(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 42);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).map_error(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 42);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).map_error(mul2);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 42);
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = e.map_error(ret_void);
    REQUIRE(ret);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = e.map_error(ret_void);
    REQUIRE(ret);
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = std::move(e).map_error(ret_void);
    REQUIRE(ret);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = std::move(e).map_error(ret_void);
    REQUIRE(ret);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.map_error(ret_void);
    REQUIRE(!ret);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.map_error(ret_void);
    REQUIRE(!ret);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).map_error(ret_void);
    REQUIRE(!ret);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).map_error(ret_void);
    REQUIRE(!ret);
  }

  {
    struct S {
      int x;
    };

    struct F {
      int x;
    };

    auto res = beyond::expected<S, F>{beyond::unexpect, F{}};

    res.map_error([](F f) {

    });
  }

  {
    beyond::expected<void, int> res{beyond::unexpect, 0};
    REQUIRE(!res);
    res = res.map_error([](int i) { return 42; });
    REQUIRE(res.error() == 42);
  }
}

TEST_CASE("expected and_then()", "[beyond.core.types.expected]")
{
  auto succeed = [](int a) { return beyond::expected<int, int>(21 * 2); };
  auto fail = [](int a) {
    return beyond::expected<int, int>(beyond::unexpect, 17);
  };

  {
    beyond::expected<int, int> e = 21;
    auto ret = e.and_then(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = e.and_then(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = std::move(e).and_then(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = std::move(e).and_then(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = std::move(e).and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = std::move(e).and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.and_then(succeed);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.and_then(succeed);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).and_then(succeed);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).and_then(succeed);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).and_then(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }
}

TEST_CASE("expected or_else()", "[beyond.core.types.expected]")
{
  using eptr = std::unique_ptr<int>;
  auto succeed = [](int a) { return beyond::expected<int, int>(21 * 2); };
  auto succeedptr = [](eptr e) { return beyond::expected<int, eptr>(21 * 2); };
  auto fail = [](int a) {
    return beyond::expected<int, int>(beyond::unexpect, 17);
  };
  auto efail = [](eptr e) {
    *e = 17;
    return beyond::expected<int, eptr>(beyond::unexpect, std::move(e));
  };
  auto failptr = [](eptr e) {
    return beyond::expected<int, eptr>(beyond::unexpect, std::move(e));
  };
  auto failvoid = [](int) {};
  auto failvoidptr = [](const eptr&) { /* don't consume */ };
  auto consumeptr = [](eptr) {};
  auto make_u_int = [](int n) { return std::unique_ptr<int>(new int(n)); };

  {
    beyond::expected<int, int> e = 21;
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = std::move(e).or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    beyond::expected<int, eptr> e = 21;
    auto ret = std::move(e).or_else(succeedptr);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = std::move(e).or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = e.or_else(fail);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = e.or_else(fail);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    beyond::expected<int, int> e = 21;
    auto ret = std::move(e).or_else(fail);
    REQUIRE(ret);
    REQUIRE(ret == 21);
  }

  {
    beyond::expected<int, eptr> e = 21;
    auto ret = std::move(e).or_else(efail);
    REQUIRE(ret);
    REQUIRE(ret == 21);
  }

  {
    const beyond::expected<int, int> e = 21;
    auto ret = std::move(e).or_else(fail);
    REQUIRE(ret);
    REQUIRE(*ret == 21);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    beyond::expected<int, eptr> e(beyond::unexpect, make_u_int(21));
    auto ret = std::move(e).or_else(succeedptr);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).or_else(succeed);
    REQUIRE(ret);
    REQUIRE(*ret == 42);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.or_else(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.or_else(failvoid);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.or_else(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = e.or_else(failvoid);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).or_else(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).or_else(failvoid);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }

  {
    beyond::expected<int, eptr> e(beyond::unexpect, make_u_int(21));
    auto ret = std::move(e).or_else(failvoidptr);
    REQUIRE(!ret);
    REQUIRE(*ret.error() == 21);
  }

  {
    beyond::expected<int, eptr> e(beyond::unexpect, make_u_int(21));
    auto ret = std::move(e).or_else(consumeptr);
    REQUIRE(!ret);
    REQUIRE(ret.error() == nullptr);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).or_else(fail);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 17);
  }

  {
    const beyond::expected<int, int> e(beyond::unexpect, 21);
    auto ret = std::move(e).or_else(failvoid);
    REQUIRE(!ret);
    REQUIRE(ret.error() == 21);
  }
}

struct move_detector {
  move_detector() = default;
  move_detector(move_detector&& rhs)
  {
    rhs.been_moved = true;
  }
  bool been_moved = false;
};

TEST_CASE("expected Observers", "[beyond.core.types.expected]")
{
  beyond::expected<int, int> o1 = 42;
  beyond::expected<int, int> o2{beyond::unexpect, 0};
  const beyond::expected<int, int> o3 = 42;

  REQUIRE(*o1 == 42);
  REQUIRE(*o1 == o1.value());
  REQUIRE(o2.value_or(42) == 42);
  REQUIRE(o2.error() == 0);
  REQUIRE(o3.value() == 42);
  auto success = std::is_same<decltype(o1.value()), int&>::value;
  REQUIRE(success);
  success = std::is_same<decltype(o3.value()), const int&>::value;
  REQUIRE(success);
  success = std::is_same<decltype(std::move(o1).value()), int&&>::value;
  REQUIRE(success);

#ifndef TL_EXPECTED_NO_CONSTRR
  success = std::is_same<decltype(std::move(o3).value()), const int&&>::value;
  REQUIRE(success);
#endif

  beyond::expected<move_detector, int> o4{beyond::in_place};
  move_detector o5 = std::move(o4).value();
  REQUIRE(o4->been_moved);
  REQUIRE(!o5.been_moved);
}

struct no_throw {
  no_throw(std::string i) : i(i) {}
  std::string i;
};
struct canthrow_move {
  canthrow_move(std::string i) : i(i) {}
  canthrow_move(canthrow_move const&) = default;
  canthrow_move(canthrow_move&& other) noexcept(false) : i(other.i) {}
  canthrow_move& operator=(canthrow_move&&) = default;
  std::string i;
};

bool should_throw = false;
struct willthrow_move {
  willthrow_move(std::string i) : i(i) {}
  willthrow_move(willthrow_move const&) = default;
  willthrow_move(willthrow_move&& other) : i(other.i)
  {
    if (should_throw) throw 0;
  }
  willthrow_move& operator=(willthrow_move&&) = default;
  std::string i;
};

template <class T1, class T2> void swap_test()
{
  std::string s1 = "abcdefghijklmnopqrstuvwxyz";
  std::string s2 = "zyxwvutsrqponmlkjihgfedcba";

  beyond::expected<T1, T2> a{s1};
  beyond::expected<T1, T2> b{s2};
  swap(a, b);
  REQUIRE(a->i == s2);
  REQUIRE(b->i == s1);

  a = s1;
  b = beyond::unexpected<T2>(s2);
  swap(a, b);
  REQUIRE(a.error().i == s2);
  REQUIRE(b->i == s1);

  a = beyond::unexpected<T2>(s1);
  b = s2;
  swap(a, b);
  REQUIRE(a->i == s2);
  REQUIRE(b.error().i == s1);

  a = beyond::unexpected<T2>(s1);
  b = beyond::unexpected<T2>(s2);
  swap(a, b);
  REQUIRE(a.error().i == s2);
  REQUIRE(b.error().i == s1);

  a = s1;
  b = s2;
  a.swap(b);
  REQUIRE(a->i == s2);
  REQUIRE(b->i == s1);

  a = s1;
  b = beyond::unexpected<T2>(s2);
  a.swap(b);
  REQUIRE(a.error().i == s2);
  REQUIRE(b->i == s1);

  a = beyond::unexpected<T2>(s1);
  b = s2;
  a.swap(b);
  REQUIRE(a->i == s2);
  REQUIRE(b.error().i == s1);

  a = beyond::unexpected<T2>(s1);
  b = beyond::unexpected<T2>(s2);
  a.swap(b);
  REQUIRE(a.error().i == s2);
  REQUIRE(b.error().i == s1);
}

TEST_CASE("expected swap", "[beyond.core.types.expected]")
{

  swap_test<no_throw, no_throw>();
  swap_test<no_throw, canthrow_move>();
  swap_test<canthrow_move, no_throw>();

  std::string s1 = "abcdefghijklmnopqrstuvwxyz";
  std::string s2 = "zyxwvutsrqponmlkjihgfedcbaxxx";
  beyond::expected<no_throw, willthrow_move> a{s1};
  beyond::expected<no_throw, willthrow_move> b{beyond::unexpect, s2};
  should_throw = 1;

#ifdef _MSC_VER
  // this seems to break catch on GCC and Clang
  REQUIRE_THROWS(swap(a, b));
#endif

  REQUIRE(a->i == s1);
  REQUIRE(b.error().i == s2);
}

struct a {
};
struct b : a {
};

auto doit() -> beyond::expected<std::unique_ptr<b>, int>
{
  return beyond::make_unexpected(0);
}

TEST_CASE("expected covariant", "[beyond.core.types.expected]")
{
  beyond::expected<std::unique_ptr<a>, int> msg = doit();
  REQUIRE(!msg.has_value());

  beyond::expected<a, int> exp = beyond::expected<b, int>(beyond::unexpect, 0);
  REQUIRE(!exp.has_value());
}

TEST_CASE("expected moving of nested type", "[beyond.core.types.expected]")
{
  struct foo {
    foo() = default;
    foo(foo&) = delete;
    foo(foo&&){};
  };

  std::vector<foo> v;
  v.emplace_back();
  beyond::expected<std::vector<foo>, int> ov = std::move(v);
  REQUIRE(ov->size() == 1);
}
