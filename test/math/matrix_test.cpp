#include <cstdlib>
#include <ostream>

#include "beyond/core/utils/assert.hpp"

namespace beyond {
/**
 * @addtogroup core
 * @{
 * @addtogroup math
 * @{
 */

template <typename T, int dim, typename Derived> struct MatrixBase {
  constexpr auto underlying() noexcept -> Derived&
  {
    return static_cast<Derived&>(*this);
  }

  constexpr auto underlying() const noexcept -> Derived const&
  {
    return static_cast<const Derived&>(*this);
  }

  [[nodiscard]] static constexpr auto dimension() noexcept -> int
  {
    return dim;
  }

  [[nodiscard]] static constexpr auto size() noexcept -> int
  {
    return size_;
  }

  /**
   * @brief Gets the element of the matrix at i-th row and j-th column
   *
   * @warning If the i and j are out-of-index, the result is undefined
   */
  [[nodiscard]] constexpr auto operator()(int i, int j) const noexcept -> T
  {
    BEYOND_ASSERT(i >= 0 && j >= 0 && i <= dimension() && j < dimension());
    return underlying().data[flattern(i, j)];
  }

  /// @overload
  [[nodiscard]] constexpr auto operator()(int i, int j) noexcept -> T&
  {
    BEYOND_ASSERT(i >= 0 && j >= 0 && i <= dimension() && j < dimension());
    return underlying().data[this->flattern(i, j)];
  }

  friend constexpr auto operator==(const MatrixBase& lhs,
                                   const MatrixBase& rhs) noexcept -> bool
  {
    for (int i = 0; i < MatrixBase::size(); ++i) {
      if (lhs.underlying().data[i] != rhs.underlying().data[i]) {
        return false;
      }
    }
    return true;
  }

  friend constexpr auto operator!=(const MatrixBase& lhs,
                                   const MatrixBase& rhs) noexcept -> bool
  {
    return !(lhs == rhs);
  }

  friend constexpr auto operator*(const MatrixBase& lhs,
                                  const MatrixBase& rhs) noexcept -> Derived
  {
    Derived result;
    for (int i = 0; i < dimension(); i++) {
      for (int j = 0; j < dimension(); j++) {
        T n{};
        for (int k = 0; k < dimension(); k++) {
          n += lhs(i, k) * rhs(k, j);
        }
        result(i, j) = n;
      }
    }
    return result;
  }

protected:
  constexpr MatrixBase() noexcept = default;

  static constexpr auto flattern(int i, int j) noexcept -> int
  {
    return j * dim + i;
  }

private:
  static constexpr int size_ = dim * dim;
};

template <typename T, int dim, typename Derived>
auto operator<<(std::ostream& os, const MatrixBase<T, dim, Derived>& m)
    -> std::ostream&
{
  os << "mat" << m.dimension() << "(\n";
  for (int i = 0; i < m.dimension(); i++) {
    for (int j = 0; j < m.dimension(); j++) {
      os << m.underlying().data[i * m.dimension() + j] << ", ";
    }
    os << '\n';
  }
  os << "\n)";
  return os;
}

template <typename T, int dim, typename Derived>
constexpr auto transpose(const MatrixBase<T, dim, Derived>& m) noexcept
    -> Derived
{
  Derived r;
  for (int i = 0; i < m.dimension(); ++i) {
    for (int j = 0; j < m.dimension(); ++j) {
      r(i, j) = m(j, i);
    }
  }
  return r;
}

template <typename T> struct Mat4 : MatrixBase<T, 4, Mat4<T>> {
  using BaseType = MatrixBase<T, 4, float>;

  T data[BaseType::size()];

  constexpr Mat4() noexcept
      : data{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}
  {
  }

  constexpr Mat4(const T v00, const T v01, const T v02, const T v03,
                 const T v10, const T v11, const T v12, const T v13,
                 const T v20, const T v21, const T v22, const T v23,
                 const T v30, const T v31, const T v32, const T v33) noexcept
      : data{v00, v10, v20, v30, v01, v11, v21, v31,
             v02, v12, v22, v32, v03, v13, v23, v33}
  {
  }
};

using Mat4f = Mat4<float>;

/** @}
 *  @} */
} // namespace beyond

#include <catch2/catch.hpp>

SCENARIO("Constructing and inspecting a 4x4 matrix", "[beyond.core.math.mat]")
{
  GIVEN("A default constructed 4x4 matrix I")
  {
    const beyond::Mat4f I;
    THEN("It is an identity matrix")
    {
      for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
          REQUIRE(I(i, j) == Approx(i == j ? 1 : 0));
        }
      }
    }
  }

  GIVEN("the following 4x4 matrix M")
  {
    const beyond::Mat4f M{
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
    const beyond::Mat4f M1{
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

  GIVEN("Two matrices A and B")
  {
    const beyond::Mat4f A{
        // clang-format off
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
        // clang-format on
    };
    const beyond::Mat4f B{
        // clang-format off
        -2, 1,  2,  3,
        3,  2,  1,  -1,
        4,  3,  6,   5,
        1,  2,  7,   8
        // clang-format on
    };

    THEN("A*B generates correct result")
    {
      const beyond::Mat4f AB{
          // clang-format off
          20, 22, 50,  48,
          44, 54, 114, 108,
          40, 58, 110, 102,
          16, 26, 46,  42
          // clang-format on
      };
      REQUIRE(A * B == AB);
    }

    THEN("B*A generates correct result")
    {
      const beyond::Mat4f BA{
          // clang-format off
          36, 30, 24, 18,
          17, 22, 27, 32,
          98, 94, 90, 86,
          114, 102, 90, 78,
          // clang-format on
      };

      REQUIRE(B * A == BA);
    }
  }

  GIVEN("Two matrices A")
  {
    const beyond::Mat4f A{
        // clang-format off
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
        // clang-format on
    };

    const beyond::Mat4f AT{
        // clang-format off
        1, 5, 9, 5,
        2, 6, 8, 4,
        3, 7, 7, 3,
        4, 8, 6, 2,
        // clang-format on
    };

    THEN("Get its transpose")
    {
      REQUIRE(AT == transpose(A));
    }

    THEN("The transpose of its transpose is the original matrix")
    {
      REQUIRE(A == transpose(AT));
    }
  }
}