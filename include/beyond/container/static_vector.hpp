#pragma once

#ifndef BEYOND_CORE_CONTAINER_STATIC_VECTOR_HPP
#define BEYOND_CORE_CONTAINER_STATIC_VECTOR_HPP

#include <iterator>
#include <memory>
#include <new>
#include <type_traits>

#include "../utils/assert.hpp"
#include "../utils/bit_cast.hpp"
#include "../utils/force_inline.hpp"
#include "../utils/panic.hpp"

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup container
 * @{
 */

/**
 * @brief A fixed-capacity dynamic-sized array
 */
template <class T, std::uint32_t N> class StaticVector final {
public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::uint32_t;
  using difference_type = std::make_signed_t<size_type>;

  StaticVector() noexcept = default;

  /**
   * @brief constructs a StaticVector with n default-inserted elements.
   * @pre `n <= capacity()`
   *
   * Complexity: O(n)
   */
  constexpr explicit StaticVector(size_type n) noexcept(
      std::is_nothrow_constructible_v<value_type>)
      : size_{n}
  {
    BEYOND_ASSERT(n <= capacity());
    std::uninitialized_value_construct_n(
        std::launder(reinterpret_cast<T*>(data_)), size_);
  }

  /**
   * @brief Constructs a `StaticVector` with `n` copies of value
   * @pre `n <= capacity()`
   *
   * Complexity: O(n)
   */
  constexpr explicit StaticVector(size_type n, value_type v) noexcept(
      std::is_nothrow_copy_constructible_v<value_type>)
      : size_{n}
  {
    BEYOND_ASSERT(n <= capacity());
    std::uninitialized_fill_n(std::launder(reinterpret_cast<T*>(data_)), size_,
                              v);
  }

  /**
   * @brief Constructs a `StaticVector` equal to the range `[first, last)`
   *
   * Complexity: Initializing distance(first, last) <= capacity() of
   * `value_type`s
   */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
  template <class InputIterator>
#else
  template <class InputIterator,
            typename = decltype(*std::declval<InputIterator&>(), void(),
                                ++std::declval<InputIterator&>(), void())>
#endif
  constexpr StaticVector(InputIterator first, InputIterator last) noexcept(
      std::is_nothrow_copy_constructible_v<value_type>)
  {
    const auto distance = static_cast<size_type>(std::distance(first, last));
    BEYOND_ASSERT(distance <= capacity());
    size_ = distance;
    std::uninitialized_copy(first, last,
                            std::launder(reinterpret_cast<T*>(data_)));
  }

  constexpr StaticVector(std::initializer_list<value_type> il)
      : size_{static_cast<size_type>(il.size())}
  {
    BEYOND_ASSERT(size_ <= capacity());
    std::uninitialized_copy(std::begin(il), std::end(il),
                            std::launder(reinterpret_cast<T*>(data_)));
  }

  ~StaticVector() noexcept(std::is_nothrow_destructible_v<value_type>)
  {
    std::destroy_n(reinterpret_cast<T*>(data_), size_);
  }

  StaticVector(const StaticVector& rhs) noexcept(
      std::is_nothrow_copy_constructible_v<value_type>)
      : size_{rhs.size_}
  {
    std::uninitialized_copy_n(std::begin(rhs), rhs.size_,
                              std::launder(reinterpret_cast<T*>(data_)));
  }

  auto operator=(const StaticVector& rhs) & noexcept(
      std::is_nothrow_copy_constructible_v<value_type>) -> StaticVector&
  {
    if (this != &rhs) {
      std::destroy_n(reinterpret_cast<T*>(data_), size_);
      std::uninitialized_copy_n(std::begin(rhs), rhs.size_,
                                reinterpret_cast<T*>(data_));
      size_ = rhs.size_;
    }
    return *this;
  }

  StaticVector(StaticVector&& rhs) noexcept(
      std::is_nothrow_move_constructible_v<value_type>)
      : size_{rhs.size_}
  {
    std::uninitialized_move_n(std::begin(rhs), rhs.size_,
                              reinterpret_cast<T*>(data_));
  }

  auto operator=(StaticVector&& rhs) & noexcept(
      std::is_nothrow_move_assignable_v<value_type>) -> StaticVector&
  {
    if (this != &rhs) {
      std::destroy_n(reinterpret_cast<T*>(data_), size_);
      std::uninitialized_move_n(std::begin(rhs), rhs.size_,
                                std::launder(reinterpret_cast<T*>(data_)));
      size_ = rhs.size_;
    }
    return *this;
  }

  /**
   * @brief Gets the capacity of the `StaticVector`
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto capacity() const noexcept
      -> size_type
  {
    return N;
  }

  /**
   * @brief Gets the size of the `StaticVector`
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto size() const noexcept
      -> size_type
  {
    return size_;
  }

  /**
   * @brief Returns if the `StaticVector` is empty or not
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto empty() const noexcept
      -> size_type
  {
    return size_ == 0;
  }

  /**
   * @brief Pushes an object into the end of the StaticVector
   *
   * @warning Will `panic()` if `size() >= capacity()`
   * @return A reference to the created object
   *
   * Complexity: O(1)
   */
  template <typename... Args>
  BEYOND_FORCE_INLINE auto push_back(const value_type& value) -> reference
  {
    return emplace_back(value);
  }

  /// @overload
  template <typename... Args>
  BEYOND_FORCE_INLINE auto push_back(value_type&& value) -> reference
  {
    return emplace_back(std::move(value));
  }

  /**
   * @brief Inplace constructs an object into the end of the StaticVector
   *
   * @warning Will `panic()` if `size() >= capacity()`
   * @return A reference to the created object
   *
   * Complexity: O(1)
   */
  template <typename... Args>
  BEYOND_FORCE_INLINE auto emplace_back(Args&&... args) -> reference
  {
    if (size() >= capacity()) {
      beyond::panic("Try to add to a to a full vector");
    }

    new (end()) T(std::forward<Args>(args)...);
    ++size_;
    return data()[size_ - 1];
  }

  /**
   * @brief Removes the last element of the container
   *
   * @warning Will `panic()` if `size() == 0`, the result is undefined
   *
   * Complexity: O(1)
   */
  BEYOND_FORCE_INLINE auto pop_back() -> void
  {
    BEYOND_ASSERT(size_ != 0);
    --size_;
  }

  /**
   * @brief Clear the `StaticVector`
   *
   * Complexity: O(n)
   */
  auto clear() -> void
  {
    std::destroy_n(std::launder(reinterpret_cast<T*>(data_)), size_);
    size_ = 0;
  }

  /**
   * @brief Access an object at index `pos` with bounds checking
   * @warning If `pos > size()`, the result is undefined
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
  operator[](size_type pos) const noexcept -> const_reference
  {
    if (pos >= size()) {
      beyond::panic("Accessing static_vector out-of-range");
    }

    return data()[pos];
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
  operator[](size_type pos) noexcept -> reference
  {
    if (pos >= size()) {
      beyond::panic("Accessing static_vector out-of-range");
    }

    return data()[pos];
  }

  /**
   * @brief access the first element
   * @warning Calling `front` on an empty container is undefined.
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto front() noexcept -> reference
  {
    BEYOND_ASSERT(size_ != 0);
    return data()[0];
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto front() const noexcept
      -> const_reference
  {
    BEYOND_ASSERT(size_ != 0);
    return data()[0];
  }

  /**
   * @brief access the last element
   * @warning Calling `back` on an empty container is undefined.
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto back() noexcept -> reference
  {
    BEYOND_ASSERT(size_ != 0);
    return data()[size_ - 1];
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto back() const noexcept
      -> const_reference
  {
    BEYOND_ASSERT(size_ != 0);
    return data()[size_ - 1];
  }

  /**
   * @brief Gets the underlying raw data pointer of the `StaticVector`
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto data() noexcept -> pointer
  {
    return std::launder(reinterpret_cast<T*>(data_));
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto data() const noexcept
      -> const_pointer
  {
    return std::launder(reinterpret_cast<const T*>(data_));
  }

  /**
   * @brief Access an object at index `pos` without bounds checking
   * @warning If `pos > size()`, the result is undefined
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
  unsafe_at(size_type pos) const noexcept -> const_reference
  {
    return std::launder(reinterpret_cast<const T*>(data_))[pos];
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
  unsafe_at(size_type pos) noexcept -> reference
  {
    return std::launder(reinterpret_cast<T*>(data_))[pos];
  }

  // TODO(lesley): erase, insert, resize, asign

  using iterator = T*;
  using const_iterator = const T*;

  // TODO(lesley): reverse iterators
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto begin() noexcept -> iterator
  {
    return iterator{reinterpret_cast<T*>(data_)};
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto end() noexcept -> iterator
  {
    return begin() + size_;
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto begin() const noexcept
      -> const_iterator
  {
    return const_iterator{reinterpret_cast<const T*>(data_)};
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto end() const noexcept
      -> const_iterator
  {
    return begin() + size_;
  }

  /**
   * @brief Swap two `StaticVector`s
   *
   * Complexity: Linear on the size() of both vector
   */
  constexpr auto
  swap(StaticVector& other) noexcept(std::is_nothrow_swappable_v<value_type>)
      -> void
  {
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
  }

  /**
   * @brief Swaps two `StaticVector`
   */
  friend constexpr auto
  swap(StaticVector<T, N>& lhs,
       StaticVector<T, N>& rhs) noexcept(std::is_nothrow_swappable_v<T>) -> void
  {
    lhs.swap(rhs);
  }

private:
  size_type size_ = 0;
  alignas(T) std::byte data_[sizeof(T) * N];
};

/** @}@} */

// TODO(lesley): lexicographically compares
// Free functions TODO(lesley): erase, erase_if

} // namespace beyond

#endif // BEYOND_CORE_CONTAINER_STATIC_VECTOR_HPP
