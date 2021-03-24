#ifndef BEYOND_CORE_CONTAINER_ARRAY_HPP
#define BEYOND_CORE_CONTAINER_ARRAY_HPP

#include <beyond/utils/assert.hpp>
#include <beyond/utils/force_inline.hpp>

#include <iterator>
#include <type_traits>

namespace beyond {

template <typename T, unsigned int N> struct Array {
  static_assert(N > 0, "Cannot create an array with zero elements");

  T elems_[N];

  using value_type = T;
  using size_type = decltype(N);
  using difference_type = std::make_signed_t<size_type>;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // =====================================================
  // Elements access
  // =====================================================
  /**
   * @brief access specified element with bound checking
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
  operator[](size_type index) noexcept -> reference
  {
    BEYOND_ENSURE(index < N);
    return elems_[index];
  };

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
  operator[](size_type index) const noexcept -> const_reference
  {
    BEYOND_ENSURE(index < N);
    return elems_[index];
  };

  /// @brief Returns a reference to the first element in the array.
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto front() noexcept -> reference
  {
    return elems_[0];
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto front() const noexcept
      -> const_reference
  {
    return elems_[0];
  }

  /// @brief Returns a reference to the last element in the array.
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto back() noexcept -> reference
  {
    return elems_[N - 1];
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto back() const noexcept
      -> const_reference
  {
    return elems_[N - 1];
  }

  /// @brief direct access to the underlying array
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto data() noexcept -> pointer
  {
    return elems_;
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto data() const noexcept
      -> const_pointer
  {
    return elems_;
  }

  // =====================================================
  // Iterators
  // =====================================================
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto begin() noexcept -> iterator
  {
    return elems_;
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto begin() const noexcept
      -> const_iterator
  {
    return elems_;
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto end() noexcept -> iterator
  {
    return elems_ + N;
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto end() const noexcept
      -> const_iterator
  {
    return elems_ + N;
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto rbegin() noexcept
      -> reverse_iterator
  {
    return reverse_iterator{end()};
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto rbegin() const noexcept
      -> const_reverse_iterator
  {
    return const_reverse_iterator{end()};
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto rend() noexcept
      -> reverse_iterator
  {
    return reverse_iterator{begin()};
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto rend() const noexcept
      -> const_reverse_iterator
  {
    return const_reverse_iterator{begin()};
  }

  // =====================================================
  // Capacity
  // =====================================================
  /// @brief Checks if the array is empty. Always false.
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto empty() const noexcept
      -> bool
  {
    return false;
  }

  /// @brief Returns the size of the array.
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto size() const noexcept
      -> size_type
  {
    return N;
  }

  /// @brief Returns the capacity of the array. Always equals to size()
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto max_size() const noexcept
      -> size_type
  {
    return N;
  }

  // =====================================================
  // Operations
  // =====================================================
  /**
   * @brief Assigns the given value v to all elements in array
   */
  constexpr void fill(const T& v) noexcept(std::is_nothrow_copy_assignable_v<T>)
  {
    for (size_type i = 0; i < N; ++i) {
      elems_[i] = v;
    }
  }

  friend constexpr void
  swap(Array& a, Array& b) noexcept(std::is_nothrow_copy_assignable_v<T>)
  {
    for (size_type i = 0; i < N; ++i) {
      using std::swap;
      swap(a.elems_[i], b.elems_[i]);
    }
  }

  // TODO: remove this once no longer support GCC10
#if !defined(__GNUC__) || __GNUC__ > 10
  /**
   * @brief Checks if the two arrays are equal
   */
  [[nodiscard]] friend constexpr auto operator==(Array lhs, Array rhs) noexcept
      -> bool = default;

  [[nodiscard]] friend constexpr auto operator<=>(Array lhs,
                                                  Array rhs) noexcept = default;
#endif
};

/**
 * @brief Extracts the Ith element element from the array.
 */
template <unsigned int I, class T, unsigned int N>
[[nodiscard]] constexpr auto get(Array<T, N>& a) noexcept -> T&
{
  static_assert(I < N);
  return a.elems_[I];
}

template <unsigned int I, class T, unsigned int N>
[[nodiscard]] constexpr auto get(const Array<T, N>& a) noexcept -> const T&
{
  static_assert(I < N);
  return a.elems_[I];
}

template <unsigned int I, class T, unsigned int N>
[[nodiscard]] constexpr auto get(Array<T, N>&& a) noexcept -> T&&
{
  static_assert(I < N);
  return std::move(a.elems_[I]);
}

template <unsigned int I, class T, unsigned int N>
[[nodiscard]] constexpr auto get(const Array<T, N>&& a) noexcept -> const T&&
{
  static_assert(I < N);
  return std::move(a.elems_[I]);
}

} // namespace beyond

namespace std {
template <typename T, unsigned int N> struct tuple_size<beyond::Array<T, N>> {
  static constexpr size_t value = N;
};

template <std::size_t I, class T, unsigned int N>
struct tuple_element<I, beyond::Array<T, N>> {
  using type = T;
};

} // namespace std

#endif // BEYOND_CORE_CONTAINER_ARRAY_HPP
