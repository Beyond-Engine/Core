#include <cstdlib>
#include <ostream>

#include "beyond/core/math/vector.hpp"
#include "beyond/core/utils/assert.hpp"

namespace beyond {
/**
 * @addtogroup core
 * @{
 * @addtogroup math
 * @{
 */

struct Matrix2;
struct Matrix3;
struct Matrix4;

template <typename Matrix>
struct MatrixTrait;

template <>
struct MatrixTrait<Matrix2> {
  static constexpr std::size_t dimension = 2;
};

template <>
struct MatrixTrait<Matrix3> {
  static constexpr std::size_t dimension = 3;
};

template <>
struct MatrixTrait<Matrix4> {
  static constexpr std::size_t dimension = 4;
};

template <typename Derived> struct MatrixBase {
  using Trait = MatrixTrait<typename Derived>;

  [[nodiscard]] constexpr auto underlying() noexcept -> Derived&
  {
    return static_cast<Derived&>(*this);
  }

  [[nodiscard]] constexpr auto underlying() const noexcept -> Derived const&
  {
    return static_cast<const Derived&>(*this);
  }

  [[nodiscard]] static constexpr auto dimension() noexcept -> std::size_t
  {
    return Trait::dimension;
  }

  [[nodiscard]] static constexpr auto size() noexcept -> std::size_t
  {
    return Trait::dimension * Trait::dimension;
  }

  /**
   * @brief Gets the element of the matrix at i-th row and j-th column
   *
   * @warning If the i and j are out-of-index, the result is undefined
   */
  [[nodiscard]] constexpr auto operator()(std::size_t i, std::size_t j) const
      noexcept -> float
  {
    BEYOND_ASSERT(i <= dimension() && j < dimension());
    return underlying().data[flattern(i, j)];
  }

  /// @overload
  [[nodiscard]] constexpr auto operator()(std::size_t i, std::size_t j) noexcept
      -> float&
  {
    BEYOND_ASSERT(i <= dimension() && j < dimension());
    return underlying().data[this->flattern(i, j)];
  }

  constexpr auto operator+=(const MatrixBase& other) -> Derived&
  {
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      underlying().data[i] += other.underlying().data[i];
    }

    return underlying();
  }

  constexpr auto operator-=(const MatrixBase& other) -> Derived&
  {
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      underlying().data[i] -= other.underlying().data[i];
    }

    return underlying();
  }

  constexpr auto operator*=(const MatrixBase& other) noexcept -> Derived
  {
    this->underlying() = this->underlying() * other;
    return this->underlying();
  }

  constexpr auto operator*=(float scalar) noexcept -> Derived
  {
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      underlying().data[i] *= scalar;
    }
    return this->underlying();
  }

  constexpr auto operator/=(float scalar) noexcept -> Derived
  {
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      underlying().data[i] /= scalar;
    }
    return this->underlying();
  }

  [[nodiscard]] friend constexpr auto operator+(const MatrixBase& lhs,
                                                const MatrixBase& rhs) noexcept
      -> Derived
  {
    Derived result;
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      result.data[i] = lhs.underlying().data[i] + rhs.underlying().data[i];
    }
    return result;
  };

  [[nodiscard]] friend constexpr auto operator-(const MatrixBase& lhs,
                                                const MatrixBase& rhs) noexcept
      -> Derived
  {
    Derived result;
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      result.data[i] = lhs.underlying().data[i] - rhs.underlying().data[i];
    }
    return result;
  };

  [[nodiscard]] friend constexpr auto operator*(const MatrixBase& lhs,
                                                const MatrixBase& rhs) noexcept
      -> Derived
  {
    Derived result;
    for (std::size_t i = 0; i < dimension(); i++) {
      for (std::size_t j = 0; j < dimension(); j++) {
        float n = 0;
        for (std::size_t k = 0; k < dimension(); k++) {
          n += lhs(i, k) * rhs(k, j);
        }
        result(i, j) = n;
      }
    }
    return result;
  }

  [[nodiscard]] friend constexpr auto operator*(const MatrixBase& mat,
                                                float s) noexcept -> Derived
  {
    Derived result;
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      result.data[i] = mat.underlying().data[i] * s;
    }
    return result;
  };

  [[nodiscard]] friend constexpr auto operator*(float s,
                                                const MatrixBase& mat) noexcept
      -> Derived
  {
    return mat * s;
  };

  [[nodiscard]] friend constexpr auto operator/(const MatrixBase& mat,
                                                float s) noexcept -> Derived
  {
    Derived result;
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      result.data[i] = mat.underlying().data[i] / s;
    }
    return result;
  };

  [[nodiscard]] friend constexpr auto operator==(const MatrixBase& lhs,
                                                 const MatrixBase& rhs) noexcept
      -> bool
  {
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      if (lhs.underlying().data[i] != rhs.underlying().data[i]) {
        return false;
      }
    }
    return true;
  }

  [[nodiscard]] friend constexpr auto operator!=(const MatrixBase& lhs,
                                                 const MatrixBase& rhs) noexcept
      -> bool
  {
    return !(lhs == rhs);
  }

protected:
  constexpr MatrixBase() noexcept = default;

  static constexpr auto flattern(std::size_t i, std::size_t j) noexcept
      -> std::size_t
  {
    return j * Trait::dimension + i;
  }
};

template <typename Derived>
auto operator<<(std::ostream& os, const MatrixBase<Derived>& m)
    -> std::ostream&
{
  os << "mat" << m.dimension() << "(\n";
  for (std::size_t i = 0; i < m.dimension(); i++) {
    for (std::size_t j = 0; j < m.dimension(); j++) {
      os << m.underlying().data[j * m.dimension() + i] << ", ";
    }
    os << '\n';
  }
  os << "\n)";
  return os;
}

template <typename Derived>
constexpr auto transpose(const MatrixBase<Derived>& m) noexcept
    -> Derived
{
  Derived r;
  for (std::size_t i = 0; i < m.dimension(); ++i) {
    for (std::size_t j = 0; j < m.dimension(); ++j) {
      r(i, j) = m(j, i);
    }
  }
  return r;
}

struct Matrix4 : MatrixBase<Matrix4> {
  using BaseType = MatrixBase<Matrix4>;

  float data[BaseType::size()];

  constexpr Matrix4() noexcept
      : data{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}
  {
  }

  constexpr Matrix4(const float v00, const float v01, const float v02, const float v03,
                 const float v10, const float v11, const float v12, const float v13,
                 const float v20, const float v21, const float v22, const float v23,
                 const float v30, const float v31, const float v32, const float v33) noexcept
      : data{v00, v10, v20, v30, v01, v11, v21, v31,
             v02, v12, v22, v32, v03, v13, v23, v33}
  {
  }

  friend constexpr auto operator*(const Matrix4& m, const Vector4<float> v)
      -> Vector4<float>
  {
    return Vector4<float>(
        m.data[0] * v.x + m.data[4] * v.y + m.data[8] * v.z + m.data[12] * v.w,
        m.data[1] * v.x + m.data[5] * v.y + m.data[9] * v.z + m.data[13] * v.w,
        m.data[2] * v.x + m.data[6] * v.y + m.data[10] * v.z + m.data[14] * v.w,
        m.data[3] * v.x + m.data[7] * v.y + m.data[11] * v.z +
            m.data[15] * v.w);
  }
};

/** @}
 *  @} */
} // namespace beyond

#include <catch2/catch.hpp>

SCENARIO("Operations on 4x4 matrices", "[beyond.core.math.mat]")
{
  GIVEN("A default constructed 4x4 matrix I")
  {
    const beyond::Matrix4 I;
    THEN("It is an identity matrix")
    {
      for (std::size_t i = 0; i < 4; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
          REQUIRE(I(i, j) == Approx(i == j ? 1 : 0));
        }
      }
    }
  }

  GIVEN("the following 4x4 matrix M")
  {
    const beyond::Matrix4 M{
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
  }

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
        0.5,  1,  1.5,  2,
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