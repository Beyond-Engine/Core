#include <catch2/catch.hpp>

#include <beyond/container/array_view.hpp>

using beyond::ArrayView;

TEST_CASE("ArrayView Default constructor")
{
  constexpr ArrayView<int> cv;
  STATIC_REQUIRE(cv == nullptr);
  STATIC_REQUIRE(cv.empty());
  STATIC_REQUIRE(cv.size() == 0);

  ArrayView<int> v;
  CHECK(v == nullptr);
  CHECK(v.empty());
  CHECK(v.size() == 0);
}

TEST_CASE("ArrayView construct from a pointer and a size")
{
  static constexpr size_t size = 42;
  static constexpr int ca[size]{};

  constexpr ArrayView<const int> cv{ca, size};
  STATIC_REQUIRE(cv != nullptr);
  STATIC_REQUIRE(!cv.empty());
  STATIC_REQUIRE(cv.size() == size);
  STATIC_REQUIRE(cv.data() == ca);

  int a[size]{};
  ArrayView<int> v{a, size};
  CHECK(v != nullptr);
  CHECK(!v.empty());
  CHECK(v.size() == size);
  CHECK(v.data() == a);
}

TEST_CASE("ArrayView construct const from none-const")
{
  static constexpr size_t size = 42;

  int a[size]{};
  ArrayView<int> v1{a, size};
  ArrayView<const int> v2 = v1;
  CHECK(v2 != nullptr);
  CHECK(!v2.empty());
  CHECK(v2.size() == v1.size());
  CHECK(v2.data() == v1.data());
}

TEST_CASE("ArrayView construct from c array")
{
  static constexpr size_t size = 42;
  static constexpr int ca[size]{};

  constexpr ArrayView<const int> cv{ca};
  STATIC_REQUIRE(cv != nullptr);
  STATIC_REQUIRE(!cv.empty());
  STATIC_REQUIRE(cv.size() == size);
  STATIC_REQUIRE(cv.data() == ca);

  int a[size]{};
  ArrayView<int> v{a};
  CHECK(v != nullptr);
  CHECK(!v.empty());
  CHECK(v.size() == size);
  CHECK(v.data() == a);
}

TEST_CASE("ArrayView random access")
{
  static constexpr int ca[]{1, 2, 3, 4};
  static constexpr ArrayView<const int> cv{ca};
  STATIC_REQUIRE(cv[2]);
  STATIC_REQUIRE(cv[2].value() == ca[2]);
  STATIC_REQUIRE(!cv[5]);

  int a[]{1, 2, 3, 4};
  ArrayView<int> v1{a};
  CHECK(v1[2]);
  CHECK(v1[2].value() == ca[2]);
  CHECK(!v1[5]);

  ArrayView<int> v2{a};
  CHECK(v2[2]);
  CHECK(v2[2].value() == ca[2]);
  CHECK(!v2[5]);

  ArrayView<const int> v3{ca};
  CHECK(v3[2]);
  CHECK(v3[2].value() == ca[2]);
  CHECK(!v3[5]);
}

TEST_CASE("ArrayView iterator")
{
  static constexpr int ca[]{1, 2, 3, 4};
  static constexpr ArrayView<const int> cv{ca};

  STATIC_REQUIRE(cv.begin() == cv.data());
  STATIC_REQUIRE(cv.begin() == cv.cbegin());
  STATIC_REQUIRE(cv.end() == cv.cend());
  STATIC_REQUIRE(*cv.begin() == 1);
  STATIC_REQUIRE(*(cv.end() - 1) == 4);

  int a[]{1, 2, 3, 4};
  ArrayView<int> v1{a};
  CHECK(v1.begin() == v1.data());
  CHECK(v1.begin() == v1.cbegin());
  CHECK(v1.end() == v1.cend());
  CHECK(*v1.begin() == 1);
  CHECK(*(v1.end() - 1) == 4);

  ArrayView<int> v2{a};
  CHECK(v2.begin() == v2.data());
  CHECK(v2.begin() == v2.cbegin());
  CHECK(v2.end() == v2.cend());
  CHECK(*v2.begin() == 1);
  CHECK(*(v2.end() - 1) == 4);

  ArrayView<const int> v3{ca};
  CHECK(v3.begin() == v3.data());
  CHECK(v3.begin() == v3.cbegin());
  CHECK(v3.end() == v3.cend());
  CHECK(*v3.begin() == 1);
  CHECK(*(v3.end() - 1) == 4);
}