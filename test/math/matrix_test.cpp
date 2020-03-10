#include <catch2/catch.hpp>

#include "beyond/core/math/matrix.hpp"
#include "beyond/core/math/serial.hpp"

#include "../serial_test_util.hpp"

template <typename Mat>
auto matrix_approx_match(const Mat& lhs, const Mat& rhs,
                         std::string_view msg = "")
{
  for (std::size_t i = 0; i < lhs.dimension(); ++i) {
    for (std::size_t j = 0; j < lhs.dimension(); ++j) {
      INFO(fmt::format("{}: {} != {}, at ({}, {})", msg, lhs, rhs, i, j));
      REQUIRE(lhs(i, j) == Approx(rhs(i, j)).epsilon(0.01));
    }
  }
}

 TEMPLATE_TEST_CASE("Default constructed matrices are zero matrices",
                   "[beyond.core.math.mat]", beyond::Mat2, beyond::Mat3,
                   beyond::Mat4)
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
                   beyond::Mat2, beyond::Mat3, beyond::Mat4)
{
  const auto I = TestType::identity();
  const auto dim = TestType::dimension();
  for (std::size_t i = 0; i < dim; ++i) {
    for (std::size_t j = 0; j < dim; ++j) {
      REQUIRE(I(i, j) == (i == j ? 1 : 0));
    }
  }
}

TEST_CASE("Matrices accessors", "[beyond.core.math.mat]")
{
  GIVEN("the following 2x2 matrix M")
  {
    beyond::Mat2 M{
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

    THEN("Can get the string representation the matrix")
    {
      const auto expected = R"(mat2(
1, 2,
5.5, 6.5,
))";
      REQUIRE_TO_STRING_EQ(M, expected);
    }
  }

  GIVEN("the following 3x3 matrix M")
  {
    beyond::Mat3 M{
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

    THEN("Can get the string representation the matrix")
    {
      const auto expected = R"(mat3(
1, 2, 3,
5.5, 6.5, 7.5,
9, 10, 11,
))";
      REQUIRE_TO_STRING_EQ(M, expected);
    }
  }

  GIVEN("the following 4x4 matrix M")
  {
    beyond::Mat4 M{
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

    THEN("Can get the string representation the matrix")
    {
      const auto expected = R"(mat4(
1, 2, 3, 4,
5.5, 6.5, 7.5, 8.5,
9, 10, 11, 12,
13.5, 14.5, 15.5, 16.5,
))";
      REQUIRE_TO_STRING_EQ(M, expected);
    }
  }
}

SCENARIO("Operations on 4x4 matrices", "[beyond.core.math.mat]")
{
  GIVEN("The following 4x4 matrix")
  {
    const beyond::Mat4 M1{
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
    beyond::Mat4 A{
        // clang-format off
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16
        // clang-format on
    };

    constexpr float s = 2;

    constexpr beyond::Mat4 A_times_s{
        // clang-format off
        2,  4,  6,  8,
        10, 12, 14, 16,
        18, 20, 22, 24,
        26, 28, 30, 32
        // clang-format on
    };

    constexpr beyond::Mat4 A_div_s{
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
    beyond::Mat4 A{
        // clang-format off
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
        // clang-format on
    };
    beyond::Mat4 B{
        // clang-format off
        -2, 1,  2,  3,
        3,  2,  1,  -1,
        4,  3,  6,   5,
        1,  2,  7,   8
        // clang-format on
    };

    constexpr beyond::Mat4 Sum{
        // clang-format off
        -1, 3, 5, 7,
        8, 8, 8, 7,
        13, 11, 13, 11,
        6, 6, 10, 10,
        // clang-format on
    };

    constexpr beyond::Mat4 Diff{
        // clang-format off
        3, 1, 1, 1,
        2, 4, 6, 9,
        5, 5, 1, 1,
        4, 2, -4, -6,
        // clang-format on
    };

    constexpr beyond::Mat4 AB{
        // clang-format off
        20, 22, 50,  48,
        44, 54, 114, 108,
        40, 58, 110, 102,
        16, 26, 46,  42
        // clang-format on
    };

    constexpr beyond::Mat4 BA{
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

  GIVEN("Matrix A and a vector v")
  {
    const beyond::Mat4 A{
        // clang-format off
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
        // clang-format on
    };
    const beyond::Vec4 v{1, 2, 3, 4};

    THEN("A*v gives the correct result")
    {
      REQUIRE(A * v == beyond::Vec4{30, 70, 70, 30});
    }
  }
}

TEST_CASE("Matrix Transpose", "[beyond.core.math.mat]")
{
  GIVEN("2x2 Matrix A and its transpose AT")
  {
    const beyond::Mat2 A{
        // clang-format off
        1, 2,
        5, 6,
        // clang-format on
    };

    const beyond::Mat2 AT{
        // clang-format off
        1, 5,
        2, 6,
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

  GIVEN("3x3 Matrix A and its transpose AT")
  {
    const beyond::Mat3 A{
        // clang-format off
        1, 2, 3,
        5, 6, 7,
        9, 8, 7,
        // clang-format on
    };

    const beyond::Mat3 AT{
        // clang-format off
        1, 5, 9,
        2, 6, 8,
        3, 7, 7,
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

  GIVEN("4x4 Matrix A and its transpose AT")
  {
    const beyond::Mat4 A{
        // clang-format off
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
        // clang-format on
    };

    const beyond::Mat4 AT{
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
}

TEST_CASE("Matrix determinant", "[beyond.core.math.mat]")
{
  SECTION("2x2 matrix")
  {
    const beyond::Mat2 A(
        // clang-format off
        1, 5,
        -3, 2
        // clang-format on
    );
    REQUIRE(determinant(A) == 17);
  }

  SECTION("3x3 matrix")
  {
    const beyond::Mat3 A(
        // clang-format off
         1,  2,  6,
        -5,  8, -4,
         2,  6,  4
        // clang-format on
    );
    REQUIRE(determinant(A) == -196);
  }

  SECTION("4x4 matrix")
  {
    const beyond::Mat4 A(
        // clang-format off
        -2, -8,  3,  5,
        -3,  1,  7,  3,
         1,  2, -9,  6,
        -6,  7,  7, -9
        // clang-format on
    );
    REQUIRE(determinant(A) == -4071);
  }
}

TEST_CASE("Matrix inverse", "[beyond.core.math.mat]")
{
  SECTION("2x2 matrix")
  {
    const beyond::Mat2 A(
        // clang-format off
        4,  7,
        2,  6
        // clang-format on
    );

    const beyond::Mat2 Ainv(
        // clang-format off
         0.6, -0.7,
        -0.2,  0.4
        // clang-format on
    );
    matrix_approx_match(inverse(A), Ainv, "2x2 matrix inverse");
    matrix_approx_match(A, inverse(Ainv), "2x2 matrix inverse");
  }

  SECTION("3x3 matrix")
  {
    const beyond::Mat3 A(
        // clang-format off
        3,  0,  2,
        2,  0, -2,
        0,  1,  1
        // clang-format on
    );

    const beyond::Mat3 Ainv(
        // clang-format off
         0.2,   0.2,   0,
        -0.2,   0.3,   1,
         0.2,  -0.3,   0
        // clang-format on
    );
    matrix_approx_match(inverse(A), Ainv, "3x3 matrix inverse");
    matrix_approx_match(A, inverse(Ainv), "3x3 matrix inverse");
  }

  SECTION("4x4 matrix")
  {
    const beyond::Mat4 A(
        // clang-format off
        -5,  2,  6, -8,
         1, -5,  1,  8,
         7,  7, -6, -7,
         1, -3,  7,  4
        // clang-format on
    );

    const beyond::Mat4 Ainv(
        // clang-format off
         0.218045,  0.451128,   0.240602, -0.0451128,
        -0.808271,  -1.45677,  -0.443609,  0.520677,
        -0.0789474, -0.223684, -0.0526316, 0.197368,
        -0.522556,  -0.81391,  -0.300752,  0.306391
        // clang-format on
    );

    matrix_approx_match(inverse(A), Ainv, "4x4 matrix inverse");
    matrix_approx_match(A, inverse(Ainv), "4x4 matrix inverse");
  }
}