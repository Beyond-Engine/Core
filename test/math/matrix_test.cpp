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

template <typename T> struct Mat4 {
private:
  static constexpr int n_ = 4;
  static constexpr int size_ = 16;

  static constexpr auto flattern(int i, int j) -> int
  {
    return j * n_ + i;
  }

public:
  T data[size_];

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

  /**
   * @brief Gets the element of the matrix at i-th row and j-th column
   *
   * @warning If the i and j are out-of-index, the result is undefined
   */
  [[nodiscard]] constexpr auto operator()(int i, int j) const -> T
  {
    BEYOND_ASSERT(i >= 0 && j >= 0 && i <= n_ && j < n_);
    return data[flattern(i, j)];
  }

  /// @overload
  [[nodiscard]] constexpr auto operator()(int i, int j) -> T&
  {
    BEYOND_ASSERT(i >= 0 && j >= 0 && i <= n_ && j < n_);
    return data[flattern(i, j)];
  }

  [[nodiscard]] constexpr auto size() const -> int
  {
    return size_;
  }

  friend constexpr auto operator==(const Mat4& lhs, const Mat4& rhs) -> bool
  {
    for (int i = 0; i < size_; ++i) {
      if (lhs.data[i] != rhs.data[i]) {
        return false;
      }
    }
    return true;
  }

  friend constexpr auto operator!=(const Mat4& lhs, const Mat4& rhs) -> bool
  {
    return !(lhs == rhs);
  }

  friend constexpr auto operator*(const Mat4& lhs, const Mat4& rhs) -> Mat4
  {
    Mat4 result;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        T n{};
        for (int k = 0; k < 4; k++) {
          n += lhs(i, k) * rhs(k, j);
        }
        result(i, j) = n;
      }
    }
    return result;
  }

  friend auto operator<<(std::ostream& os, const Mat4& m) -> std::ostream&
  {
    os << "mat4(\n";
    for (int i = 0; i < n_; i++) {
      for (int j = 0; j < n_; j++) {
        os << m.data[flattern(i, j)] << ", ";
      }
      os << '\n';
    }
    os << "\n)";
    return os;
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
        -2,  1,  2,  3,
        3,  2,  1,  -1,
        4,  3, 6, 5,
        1, 2, 7, 8
        // clang-format on
    };

    THEN("AxB generates another matrix")
    {
      const beyond::Mat4f AB{
          // clang-format off
          20, 22, 50,  48,
          44, 54, 114, 108,
          40, 58, 110, 102,
          16, 26, 46,  42
          // clang-format on
      };

      const beyond::Mat4f BA{
          // clang-format off
          36, 30, 24, 18,
          17, 22, 27, 32,
          98, 94, 90, 86,
          114, 102, 90, 78,
          // clang-format on
      };

      REQUIRE(A * B == AB);
      REQUIRE(B * A == BA);
    }
  }
}