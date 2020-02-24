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
  static constexpr int row_count = 4;
  static constexpr int col_count = 4;

public:
  T data[16];

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
   */
  [[nodiscard]] constexpr auto operator()(int i, int j) const -> T
  {
    BEYOND_ASSERT(i >= 0 && j >= 0 && i <= row_count && j < col_count);
    return data[j * row_count + i];
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
}