#include <catch2/catch.hpp>

#include "../raii_counter.hpp"
#include <beyond/types/unique_ptr.hpp>

namespace {

struct Base {
  constexpr Base() = default;
  constexpr virtual ~Base() = default;
  constexpr virtual auto value() -> int = 0;
};

struct Derived : Base {
  int v;
  constexpr explicit Derived(int v_) : v{v_} {}

  constexpr auto value() -> int override
  {
    return 42;
  }
};

} // namespace

TEST_CASE("UniquePtr test", "[beyond.core.types.unique_ptr]")
{
  SECTION("Default constructor")
  {
    constexpr beyond::UniquePtr<int> ptr;
    STATIC_REQUIRE(!ptr);
  }

  SECTION("From nullptr")
  {
    constexpr beyond::UniquePtr<int> ptr = nullptr;
    STATIC_REQUIRE(ptr == nullptr);
  }

  SECTION("From pointer")
  {
    constexpr auto f = []() {
      beyond::UniquePtr<int> ptr{new int(42)};
      return *ptr;
    };
    STATIC_REQUIRE(f() == 42);
  }

  SECTION("From make_unique")
  {
    constexpr auto f = []() {
      auto ptr = beyond::make_unique<int>(42);
      return *ptr;
    };
    STATIC_REQUIRE(f() == 42);
  }

  SECTION("From make_unique_for_overwrite")
  {
    constexpr auto f = []() {
      auto ptr = beyond::make_unique_for_overwrite<int>();
      *ptr = 42;
      return *ptr;
    };
    STATIC_REQUIRE(f() == 42);
  }

  SECTION("release")
  {
    constexpr auto f = []() {
      auto ptr = beyond::make_unique<int>(42);
      int* raw_ptr = ptr.release();
      int value = *raw_ptr;
      delete raw_ptr;
      return value;
    };
    STATIC_REQUIRE(f() == 42);
  }

  SECTION("reset")
  {
    constexpr auto f = []() {
      auto ptr = beyond::make_unique<int>(42);
      int* raw_ptr = new int(55);
      ptr.reset(raw_ptr);
      raw_ptr = nullptr;
      return *ptr;
    };
    STATIC_REQUIRE(f() == 55);
  }

  SECTION("Covariant conversion")
  {
    beyond::UniquePtr<Base> ptr = beyond::make_unique<Derived>(42);
    REQUIRE(ptr->value() == 42);
  }

  SECTION("Equality comparison")
  {
    constexpr auto f1 = []() {
      auto ptr1 = beyond::make_unique<int>(42);
      return ptr1 == ptr1;
    };
    STATIC_REQUIRE(f1());
    REQUIRE(beyond::make_unique<int>(42) != beyond::make_unique<int>(42));
  }

  SECTION("Three way comparison")
  {
    auto ptr1 = beyond::make_unique<int>(42);
    auto ptr2 = beyond::make_unique<int>(42);
    REQUIRE((ptr1 <=> ptr2) == (ptr1.get() <=> ptr2.get()));
  }
}