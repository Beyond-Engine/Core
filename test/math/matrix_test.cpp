#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>

#include "beyond/core/math/matrix.hpp"

 TEMPLATE_TEST_CASE("Default constructed matrices are zero matrices",
                   "[beyond.core.math.mat]", beyond::Matrix2, beyond::Matrix3,
                   beyond::Matrix4)
{
  const auto Z1 = TestType();
  const auto Z2 = TestType::zero();
  const auto dim = TestType::dimension();
  for (std::size_t i = 0; i < dim; ++i) {
    for (std::size_t j = 0; j < dim; ++j) {
      REQUIRE(Z1(i, j) == 0);
      REQUIRE(Z2(i, j) == 0);
    }
  }
}

 TEMPLATE_TEST_CASE("Creating identity matrices", "[beyond.core.math.mat]",
                   beyond::Matrix2, beyond::Matrix3, beyond::Matrix4)
{
  const auto I = TestType::identity();
  const auto dim = TestType::dimension();
  for (std::size_t i = 0; i < dim; ++i) {
    for (std::size_t j = 0; j < dim; ++j) {
      REQUIRE(I(i, j) == (i == j ? 1 : 0));
    }
  }
}

TEST_CASE("Matrices accessor", "[beyond.core.math.mat]")
{
  GIVEN("the following 2x2 matrix M")
  {
    beyond::Matrix2 M{
        // clang-format off
        1,    2,
        5.5,  6.5,
        // clang-format on
    };
    THEN("the matrix can be accessed by the operator ()")
    {
      REQUIRE(M(0, 0) == Approx(1));
      REQUIRE(M(1, 1) == Approx(6.5));
    }

    WHEN("Modify M(0, 0) into 5")
    {
      M(0, 0) = 5;
      THEN("M(0, 0) == 5")
      {
        REQUIRE(M(0, 0) == 5);
      }
    }
  }

  GIVEN("the following 3x3 matrix M")
  {
    beyond::Matrix3 M{
        // clang-format off
        1,    2,    3,
        5.5,  6.5,  7.5,
        9,    10,   11,
        // clang-format on
    };
    THEN("the matrix can be accessed by the operator ()")
    {
      REQUIRE(M(0, 0) == Approx(1));
      REQUIRE(M(1, 0) == Approx(5.5));
      REQUIRE(M(1, 2) == Approx(7.5));
      REQUIRE(M(2, 2) == Approx(11));
    }

    WHEN("Modify M(0, 0) into 5")
    {
      M(0, 0) = 5;
      THEN("M(0, 0) == 5")
      {
        REQUIRE(M(0, 0) == 5);
      }
    }
  }

  GIVEN("the following 4x4 matrix M")
  {
    beyond::Matrix4 M{
        // clang-format off
        1,    2,    3,    4,
        5.5,  6.5,  7.5,  8.5,
        9,    10,   11,   12,
        13.5, 14.5, 15.5, 16.5
        // clang-format on
    };
    THEN("the matrix can be accessed by the operator ()")
    {
      REQUIRE(M(0, 0) == Approx(1));
      REQUIRE(M(0, 3) == Approx(4));
      REQUIRE(M(1, 0) == Approx(5.5));
      REQUIRE(M(1, 2) == Approx(7.5));
      REQUIRE(M(2, 2) == Approx(11));
      REQUIRE(M(3, 2) == Approx(15.5));
    }

    WHEN("Modify M(0, 0) into 5")
    {
      M(0, 0) = 5;
      THEN("M(0, 0) == 5")
      {
        REQUIRE(M(0, 0) == 5);
      }
    }
  }
}

SCENARIO("Operations on 4x4 matrices", "[beyond.core.math.mat]")
{
  GIVEN("The following 4x4 matrix")
  {
    const beyond::Matrix4 M1{
        // clang-format off
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16
        // clang-format on
    };
    AND_GIVEN("A copied matrix")
    {
      const auto M2 = M1;
      THEN("They should be equal to each other")
      {
        REQUIRE(M1 == M2);
      }
    }
    AND_GIVEN("A different matrix")
    {
      auto M2 = M1;
      M2(0, 0) = 5;
      THEN("They should be equal to each other")
      {
        REQUIRE(M1 != M2);
      }
    }
  }

  GIVEN("A matrix and a scalar s")
  {
    beyond::Matrix4 A{
        // clang-format off
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16
        // clang-format on
    };

    constexpr float s = 2;

    constexpr beyond::Matrix4 A_times_s{
        // clang-format off
        2,  4,  6,  8,
        10, 12, 14, 16,
        18, 20, 22, 24,
        26, 28, 30, 32
        // clang-format on
    };

    constexpr beyond::Matrix4 A_div_s{
        // clang-format off
        0.5, 1,  1.5,  2,
        2.5, 3, 3.5, 4,
        4.5, 5, 5.5, 6,
        6.5, 7, 7.5, 8
        // clang-format on
    };

    WHEN("A *= s")
    {
      A *= s;
      THEN("A' is the component-wise multiplication")
      {
        REQUIRE(A == A_times_s);
      }
    }

    WHEN("A /= s")
    {
      A /= s;
      THEN("A' is the component-wise division")
      {
        REQUIRE(A == A_div_s);
      }
    }

    THEN("A * s = s * A = the component-wise multiplication")
    {
      REQUIRE(A * s == A_times_s);
      REQUIRE(s * A == A_times_s);
    }

    THEN("A / s the component-wise division")
    {
      REQUIRE(A / s == A_div_s);
    }
  }

  GIVEN("Two matrices A and B")
  {
    beyond::Matrix4 A{
        // clang-format off
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
        // clang-format on
    };
    beyond::Matrix4 B{
        // clang-format off
        -2, 1,  2,  3,
        3,  2,  1,  -1,
        4,  3,  6,   5,
        1,  2,  7,   8
        // clang-format on
    };

    constexpr beyond::Matrix4 Sum{
        // clang-format off
        -1, 3, 5, 7,
        8, 8, 8, 7,
        13, 11, 13, 11,
        6, 6, 10, 10,
        // clang-format on
    };

    constexpr beyond::Matrix4 Diff{
        // clang-format off
        3, 1, 1, 1,
        2, 4, 6, 9,
        5, 5, 1, 1,
        4, 2, -4, -6,
        // clang-format on
    };

    constexpr beyond::Matrix4 AB{
        // clang-format off
        20, 22, 50,  48,
        44, 54, 114, 108,
        40, 58, 110, 102,
        16, 26, 46,  42
        // clang-format on
    };

    constexpr beyond::Matrix4 BA{
        // clang-format off
        36, 30, 24, 18,
        17, 22, 27, 32,
        98, 94, 90, 86,
        114, 102, 90, 78,
        // clang-format on
    };

    WHEN("A += B")
    {
      A += B;
      THEN("A equal to the component-wise sum")
      {
        REQUIRE(A == Sum);
      }
    }

    WHEN("A -= B")
    {
      A -= B;
      THEN("A equal to the component-wise subtraction")
      {
        REQUIRE(A == Diff);
      }
    }

    THEN("A + B equal to the component-wise sum")
    {
      REQUIRE(A + B == Sum);
      REQUIRE(B + A == Sum);
    }

    THEN("A - B equal to the component-wise sum")
    {
      REQUIRE(A - B == Diff);
    }

    WHEN("A *= B")
    {
      A *= B;
      THEN("A' = A * B")
      {
        REQUIRE(A == AB);
      }
    }

    THEN("A*B generates correct result")
    {
      REQUIRE(A * B == AB);
    }

    WHEN("B *= A")
    {
      B *= A;
      THEN("B' = B * A")
      {
        REQUIRE(B == BA);
      }
    }

    THEN("B*A generates correct result")
    {
      REQUIRE(B * A == BA);
    }
  }

  GIVEN("Matrix A and its transpose AT")
  {
    const beyond::Matrix4 A{
        // clang-format off
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
        // clang-format on
    };

    const beyond::Matrix4 AT{
        // clang-format off
        1, 5, 9, 5,
        2, 6, 8, 4,
        3, 7, 7, 3,
        4, 8, 6, 2,
        // clang-format on
    };

    THEN("AT = transpose(A)")
    {
      REQUIRE(AT == transpose(A));
    }

    THEN("A = transpose(AT)")
    {
      REQUIRE(A == transpose(AT));
    }
  }

  GIVEN("Matrix A and a vector v")
  {
    const beyond::Matrix4 A{
        // clang-format off
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
        // clang-format on
    };
    const beyond::Vector4f v{1, 2, 3, 4};

    THEN("A*v gives the correct result")
    {
      REQUIRE(A * v == beyond::Vector4f{30, 70, 70, 30});
    }
  }
}