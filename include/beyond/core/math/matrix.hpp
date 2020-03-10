#pragma once

#ifndef BEYOND_CORE_MATH_MATRIX_HPP
#define BEYOND_CORE_MATH_MATRIX_HPP

/**
 * @file matrix.hpp
 * @brief Provides Matrix classes
 * @ingroup math
 */

#include <cstdlib>

#include "beyond/core/math/math_fwd.hpp"
#include "beyond/core/math/vector.hpp"
#include "beyond/core/utils/assert.hpp"

namespace beyond {
/**
 * @addtogroup core
 * @{
 * @addtogroup math
 * @{
 */

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4701)
#endif

template <typename Matrix> struct MatrixTrait;

template <typename T> struct MatrixTrait<Matrix2<T>> {
  static constexpr std::size_t dimension = 2;
  using ValueType = T;
};

template <typename T> struct MatrixTrait<Matrix3<T>> {
  static constexpr std::size_t dimension = 3;
  using ValueType = T;
};

template <typename T> struct MatrixTrait<Matrix4<T>> {
  static constexpr std::size_t dimension = 4;
  using ValueType = T;
};

template <typename Derived> struct MatrixBase {
  using Trait = MatrixTrait<Derived>;
  using ValueType = typename Trait::ValueType;

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
      noexcept -> ValueType
  {
    BEYOND_ASSERT(i <= dimension() && j < dimension());
    return underlying().data[flattern(i, j)];
  }

  /// @overload
  [[nodiscard]] constexpr auto operator()(std::size_t i, std::size_t j) noexcept
      -> ValueType&
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

  constexpr auto operator*=(ValueType scalar) noexcept -> Derived
  {
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      underlying().data[i] *= scalar;
    }
    return this->underlying();
  }

  constexpr auto operator/=(ValueType scalar) noexcept -> Derived
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
    Derived result(uninitialized_tag);
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      result.data[i] = lhs.underlying().data[i] + rhs.underlying().data[i];
    }
    return result;
  };

  [[nodiscard]] friend constexpr auto operator-(const MatrixBase& lhs,
                                                const MatrixBase& rhs) noexcept
      -> Derived
  {
    Derived result(uninitialized_tag);
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      result.data[i] = lhs.underlying().data[i] - rhs.underlying().data[i];
    }
    return result;
  };

  [[nodiscard]] friend constexpr auto operator*(const MatrixBase& lhs,
                                                const MatrixBase& rhs) noexcept
      -> Derived
  {
    Derived result(uninitialized_tag);
    for (std::size_t i = 0; i < dimension(); i++) {
      for (std::size_t j = 0; j < dimension(); j++) {
        ValueType n = 0;
        for (std::size_t k = 0; k < dimension(); k++) {
          n += lhs(i, k) * rhs(k, j);
        }
        result(i, j) = n;
      }
    }
    return result;
  }

  [[nodiscard]] friend constexpr auto operator*(const MatrixBase& mat,
                                                ValueType s) noexcept -> Derived
  {
    Derived result(uninitialized_tag);
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      result.data[i] = mat.underlying().data[i] * s;
    }
    return result;
  };

  [[nodiscard]] friend constexpr auto operator*(ValueType s,
                                                const MatrixBase& mat) noexcept
      -> Derived
  {
    return mat * s;
  };

  [[nodiscard]] friend constexpr auto operator/(const MatrixBase& mat,
                                                ValueType s) noexcept -> Derived
  {
    Derived result(uninitialized_tag);
    const auto s_inv = 1 / s;
    for (std::size_t i = 0; i < MatrixBase::size(); ++i) {
      result.data[i] = mat.underlying().data[i] * s_inv;
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

  [[nodiscard]] static constexpr auto zero() noexcept -> Derived
  {
    return Derived();
  }

protected:
  constexpr MatrixBase() noexcept = default;

  [[nodiscard]] static constexpr auto flattern(std::size_t i,
                                               std::size_t j) noexcept
      -> std::size_t
  {
    return j * Trait::dimension + i;
  }
};

template <typename Derived>
[[nodiscard]] constexpr auto transpose(const MatrixBase<Derived>& m) noexcept
    -> Derived
{
  Derived r(uninitialized_tag);
  for (std::size_t i = 0; i < m.dimension(); ++i) {
    for (std::size_t j = 0; j < m.dimension(); ++j) {
      r(i, j) = m(j, i);
    }
  }
  return r;
}

template <typename T> struct Matrix2 : MatrixBase<Matrix2<T>> {
  using BaseType = MatrixBase<Matrix2>;
  using ValueType = T;

  ValueType data[BaseType::size()];

  /// @brief Create a Matrix2 with all its elements zero-initialized
  constexpr Matrix2() noexcept : data{} {}

  /// @brief Create a Matrix2 with all its elements uninitialized
  explicit Matrix2(UninitializedTag) noexcept {}

  constexpr Matrix2(const ValueType v00, const ValueType v01,
                    const ValueType v10, const ValueType v11) noexcept
      : data{v00, v10, v01, v11}
  {
  }

  [[nodiscard]] friend constexpr auto operator*(const Matrix2& m,
                                                const Vector2<ValueType> v)
      -> Vector2<ValueType>
  {
    return Vector2<ValueType>(m.data[0] * v.x + m.data[2] * v.y,
                              m.data[1] * v.x + m.data[3] * v.y);
  }

  [[nodiscard]] static constexpr auto identity() noexcept -> Matrix2
  {
    return Matrix2(1, 0, 0, 1);
  }

  [[nodiscard]] friend constexpr auto determinant(const Matrix2& m) noexcept
      -> ValueType
  {
    return m.data[0] * m.data[3] - m.data[1] * m.data[2];
  }

  [[nodiscard]] friend constexpr auto inverse(const Matrix2& m) noexcept
      -> Matrix2
  {
    return Matrix2(m(1, 1), -m(0, 1), -m(1, 0), m(0, 0)) / determinant(m);
  }
};

template <typename T> struct Matrix3 : MatrixBase<Matrix3<T>> {
  using BaseType = MatrixBase<Matrix3>;
  using ValueType = T;

  ValueType data[BaseType::size()];

  /// @brief Create a Matrix3 with all its elements zero-initialized
  constexpr Matrix3() noexcept : data{} {}

  /// @brief Create a Matrix3 with all its elements uninitialized
  explicit Matrix3(UninitializedTag) noexcept {}

  constexpr Matrix3(const ValueType v00, const ValueType v01,
                    const ValueType v02, const ValueType v10,
                    const ValueType v11, const ValueType v12,
                    const ValueType v20, const ValueType v21,
                    const ValueType v22) noexcept
      : data{v00, v10, v20, v01, v11, v21, v02, v12, v22}
  {
  }

  [[nodiscard]] friend constexpr auto operator*(const Matrix3& m,
                                                const Vector3<ValueType> v)
      -> Vector3<ValueType>
  {
    return Vector3<ValueType>(
        m.data[0] * v.x + m.data[3] * v.y + m.data[6] * v.z,
        m.data[1] * v.x + m.data[4] * v.y + m.data[7] * v.z,
        m.data[2] * v.x + m.data[5] * v.y + m.data[8] * v.z);
  }

  [[nodiscard]] static constexpr auto identity() noexcept -> Matrix3
  {
    return Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);
  }

  [[nodiscard]] friend constexpr auto determinant(const Matrix3& m) noexcept
      -> ValueType
  {
    return m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) -
           m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
           m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
  }

  [[nodiscard]] friend constexpr auto inverse(const Matrix3& m) noexcept
      -> Matrix3
  {
    return Matrix3(m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1),
                   m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2),
                   m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1),
                   m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2),
                   m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0),
                   m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2),
                   m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0),
                   m(0, 1) * m(2, 0) - m(0, 0) * m(2, 1),
                   m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) /
           determinant(m);
  }
};

template <typename T> struct Matrix4 : MatrixBase<Matrix4<T>> {
  using BaseType = MatrixBase<Matrix4>;
  using ValueType = T;

  ValueType data[BaseType::size()];

  /// @brief Create a Matrix4 with all its elements zero-initialized
  constexpr Matrix4() noexcept : data{} {}

  /// @brief Create a Matrix4 with all its elements uninitialized
  explicit Matrix4(UninitializedTag) noexcept {}

  constexpr Matrix4(const ValueType v00, const ValueType v01,
                    const ValueType v02, const ValueType v03,
                    const ValueType v10, const ValueType v11,
                    const ValueType v12, const ValueType v13,
                    const ValueType v20, const ValueType v21,
                    const ValueType v22, const ValueType v23,
                    const ValueType v30, const ValueType v31,
                    const ValueType v32, const ValueType v33) noexcept
      : data{v00, v10, v20, v30, v01, v11, v21, v31,
             v02, v12, v22, v32, v03, v13, v23, v33}
  {
  }

  [[nodiscard]] friend constexpr auto operator*(const Matrix4& m,
                                                const Vector4<ValueType> v)
      -> Vector4<ValueType>
  {
    return Vector4<ValueType>(
        m.data[0] * v.x + m.data[4] * v.y + m.data[8] * v.z + m.data[12] * v.w,
        m.data[1] * v.x + m.data[5] * v.y + m.data[9] * v.z + m.data[13] * v.w,
        m.data[2] * v.x + m.data[6] * v.y + m.data[10] * v.z + m.data[14] * v.w,
        m.data[3] * v.x + m.data[7] * v.y + m.data[11] * v.z +
            m.data[15] * v.w);
  }

  [[nodiscard]] static constexpr auto identity() noexcept -> Matrix4
  {
    return Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
  }

  [[nodiscard]] friend constexpr auto determinant(const Matrix4& m) noexcept
      -> ValueType
  {
    return m(0, 0) * m(1, 1) * m(2, 2) * m(3, 3) +
           m(0, 0) * m(2, 1) * m(3, 2) * m(1, 3) +
           m(0, 0) * m(3, 1) * m(1, 2) * m(2, 3) +
           m(1, 0) * m(0, 1) * m(3, 2) * m(2, 3) +
           m(1, 0) * m(2, 1) * m(0, 2) * m(3, 3) +
           m(1, 0) * m(3, 1) * m(2, 2) * m(0, 3) +
           m(2, 0) * m(0, 1) * m(1, 2) * m(3, 3) +
           m(2, 0) * m(1, 1) * m(3, 2) * m(0, 3) +
           m(2, 0) * m(3, 1) * m(0, 2) * m(1, 3) +
           m(3, 0) * m(0, 1) * m(2, 2) * m(1, 3) +
           m(3, 0) * m(1, 1) * m(0, 2) * m(2, 3) +
           m(3, 0) * m(2, 1) * m(1, 2) * m(0, 3) -
           m(0, 0) * m(1, 1) * m(3, 2) * m(2, 3) -
           m(0, 0) * m(2, 1) * m(1, 2) * m(3, 3) -
           m(0, 0) * m(3, 1) * m(2, 2) * m(1, 3) -
           m(1, 0) * m(0, 1) * m(2, 2) * m(3, 3) -
           m(1, 0) * m(2, 1) * m(3, 2) * m(0, 3) -
           m(1, 0) * m(3, 1) * m(0, 2) * m(2, 3) -
           m(2, 0) * m(0, 1) * m(3, 2) * m(1, 3) -
           m(2, 0) * m(1, 1) * m(0, 2) * m(3, 3) -
           m(2, 0) * m(3, 1) * m(1, 2) * m(0, 3) -
           m(3, 0) * m(0, 1) * m(1, 2) * m(2, 3) -
           m(3, 0) * m(1, 1) * m(2, 2) * m(0, 3) -
           m(3, 0) * m(2, 1) * m(0, 2) * m(1, 3);
  }

  [[nodiscard]] friend constexpr auto inverse(const Matrix4& m) noexcept
      -> Matrix4
  {
    Matrix4 out;

    out(0, 0) = m(2, 1) * m(3, 2) * m(1, 3) - m(3, 1) * m(2, 2) * m(1, 3) +
                m(3, 1) * m(1, 2) * m(2, 3) - m(1, 1) * m(3, 2) * m(2, 3) -
                m(2, 1) * m(1, 2) * m(3, 3) + m(1, 1) * m(2, 2) * m(3, 3);

    out(1, 0) = m(3, 0) * m(2, 2) * m(1, 3) - m(2, 0) * m(3, 2) * m(1, 3) -
                m(3, 0) * m(1, 2) * m(2, 3) + m(1, 0) * m(3, 2) * m(2, 3) +
                m(2, 0) * m(1, 2) * m(3, 3) - m(1, 0) * m(2, 2) * m(3, 3);

    out(2, 0) = m(2, 0) * m(3, 1) * m(1, 3) - m(3, 0) * m(2, 1) * m(1, 3) +
                m(3, 0) * m(1, 1) * m(2, 3) - m(1, 0) * m(3, 1) * m(2, 3) -
                m(2, 0) * m(1, 1) * m(3, 3) + m(1, 0) * m(2, 1) * m(3, 3);

    out(3, 0) = m(3, 0) * m(2, 1) * m(1, 2) - m(2, 0) * m(3, 1) * m(1, 2) -
                m(3, 0) * m(1, 1) * m(2, 2) + m(1, 0) * m(3, 1) * m(2, 2) +
                m(2, 0) * m(1, 1) * m(3, 2) - m(1, 0) * m(2, 1) * m(3, 2);

    out(0, 1) = m(3, 1) * m(2, 2) * m(0, 3) - m(2, 1) * m(3, 2) * m(0, 3) -
                m(3, 1) * m(0, 2) * m(2, 3) + m(0, 1) * m(3, 2) * m(2, 3) +
                m(2, 1) * m(0, 2) * m(3, 3) - m(0, 1) * m(2, 2) * m(3, 3);

    out(1, 1) = m(2, 0) * m(3, 2) * m(0, 3) - m(3, 0) * m(2, 2) * m(0, 3) +
                m(3, 0) * m(0, 2) * m(2, 3) - m(0, 0) * m(3, 2) * m(2, 3) -
                m(2, 0) * m(0, 2) * m(3, 3) + m(0, 0) * m(2, 2) * m(3, 3);

    out(2, 1) = m(3, 0) * m(2, 1) * m(0, 3) - m(2, 0) * m(3, 1) * m(0, 3) -
                m(3, 0) * m(0, 1) * m(2, 3) + m(0, 0) * m(3, 1) * m(2, 3) +
                m(2, 0) * m(0, 1) * m(3, 3) - m(0, 0) * m(2, 1) * m(3, 3);

    out(3, 1) = m(2, 0) * m(3, 1) * m(0, 2) - m(3, 0) * m(2, 1) * m(0, 2) +
                m(3, 0) * m(0, 1) * m(2, 2) - m(0, 0) * m(3, 1) * m(2, 2) -
                m(2, 0) * m(0, 1) * m(3, 2) + m(0, 0) * m(2, 1) * m(3, 2);

    out(0, 2) = m(1, 1) * m(3, 2) * m(0, 3) - m(3, 1) * m(1, 2) * m(0, 3) +
                m(3, 1) * m(0, 2) * m(1, 3) - m(0, 1) * m(3, 2) * m(1, 3) -
                m(1, 1) * m(0, 2) * m(3, 3) + m(0, 1) * m(1, 2) * m(3, 3);

    out(1, 2) = m(3, 0) * m(1, 2) * m(0, 3) - m(1, 0) * m(3, 2) * m(0, 3) -
                m(3, 0) * m(0, 2) * m(1, 3) + m(0, 0) * m(3, 2) * m(1, 3) +
                m(1, 0) * m(0, 2) * m(3, 3) - m(0, 0) * m(1, 2) * m(3, 3);

    out(2, 2) = m(1, 0) * m(3, 1) * m(0, 3) - m(3, 0) * m(1, 1) * m(0, 3) +
                m(3, 0) * m(0, 1) * m(1, 3) - m(0, 0) * m(3, 1) * m(1, 3) -
                m(1, 0) * m(0, 1) * m(3, 3) + m(0, 0) * m(1, 1) * m(3, 3);

    out(3, 2) = m(3, 0) * m(1, 1) * m(0, 2) - m(1, 0) * m(3, 1) * m(0, 2) -
                m(3, 0) * m(0, 1) * m(1, 2) + m(0, 0) * m(3, 1) * m(1, 2) +
                m(1, 0) * m(0, 1) * m(3, 2) - m(0, 0) * m(1, 1) * m(3, 2);

    out(0, 3) = m(2, 1) * m(1, 2) * m(0, 3) - m(1, 1) * m(2, 2) * m(0, 3) -
                m(2, 1) * m(0, 2) * m(1, 3) + m(0, 1) * m(2, 2) * m(1, 3) +
                m(1, 1) * m(0, 2) * m(2, 3) - m(0, 1) * m(1, 2) * m(2, 3);

    out(1, 3) = m(1, 0) * m(2, 2) * m(0, 3) - m(2, 0) * m(1, 2) * m(0, 3) +
                m(2, 0) * m(0, 2) * m(1, 3) - m(0, 0) * m(2, 2) * m(1, 3) -
                m(1, 0) * m(0, 2) * m(2, 3) + m(0, 0) * m(1, 2) * m(2, 3);

    out(2, 3) = m(2, 0) * m(1, 1) * m(0, 3) - m(1, 0) * m(2, 1) * m(0, 3) -
                m(2, 0) * m(0, 1) * m(1, 3) + m(0, 0) * m(2, 1) * m(1, 3) +
                m(1, 0) * m(0, 1) * m(2, 3) - m(0, 0) * m(1, 1) * m(2, 3);

    out(3, 3) = m(1, 0) * m(2, 1) * m(0, 2) - m(2, 0) * m(1, 1) * m(0, 2) +
                m(2, 0) * m(0, 1) * m(1, 2) - m(0, 0) * m(2, 1) * m(1, 2) -
                m(1, 0) * m(0, 1) * m(2, 2) + m(0, 0) * m(1, 1) * m(2, 2);

    return out / determinant(m);
  }
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/** @}
 *  @} */
} // namespace beyond

#endif // BEYOND_CORE_MATH_MATRIX_HPP