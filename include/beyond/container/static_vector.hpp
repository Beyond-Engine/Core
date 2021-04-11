#pragma once

#ifndef BEYOND_CORE_CONTAINER_STATIC_VECTOR_HPP
#define BEYOND_CORE_CONTAINER_STATIC_VECTOR_HPP

#include <iterator>
#include <memory>
#include <new>
#include <type_traits>

#include "../utils/assert.hpp"
#include "../utils/bit_cast.hpp"
#include "vector_interface.hpp"

/**
 * @addtogroup core
 * @{
 * @defgroup container Containers
 * @}
 */

namespace beyond {

template <class Vec> struct VecTrait;
template <class T, std::uint32_t N> class StaticVector;

template <class T, std::uint32_t N> struct ContainerTrait<StaticVector<T, N>> {
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = decltype(N);
  using difference_type = std::make_signed_t<size_type>;
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using reverse_const_iterator = std::reverse_iterator<const_iterator>;
};

/**
 * @brief A fixed-capacity dynamic-sized array
 * @ingroup container
 */
template <class T, std::uint32_t N>
class StaticVector : public VectorInterface<StaticVector<T, N>> {
  using Trait = ContainerTrait<StaticVector>;
  using Base = VectorInterface<StaticVector<T, N>>;

public:
  using value_type = typename Trait::value_type;
  using pointer = typename Trait::pointer;
  using const_pointer = typename Trait::const_pointer;
  using reference = typename Trait::reference;
  using const_reference = typename Trait::const_reference;
  using size_type = typename Trait::size_type;
  using difference_type = typename Trait::difference_type;
  using iterator = typename Trait::iterator;
  using const_iterator = typename Trait::const_iterator;
  using reverse_iterator = typename Trait::reverse_iterator;
  using reverse_const_iterator = typename Trait::reverse_const_iterator;

  constexpr StaticVector() noexcept = default;

  /**
   * @brief constructs a StaticVector with n default-inserted elements.
   * @pre `n <= capacity()`
   *
   * Complexity: O(n)
   */
  explicit StaticVector(size_type n) //
      noexcept(std::is_nothrow_constructible_v<value_type>)
      : size_{n}
  {
    BEYOND_ENSURE(n <= capacity());
    std::uninitialized_value_construct_n(data(), size_);
  }

  /**
   * @brief Constructs a `StaticVector` with `n` copies of value
   * @pre `n <= capacity()`
   *
   * Complexity: O(n)
   */
  explicit StaticVector(size_type n, value_type v) //
      noexcept(std::is_nothrow_copy_constructible_v<value_type>)
      : size_{n}
  {
    BEYOND_ENSURE(n <= capacity());
    std::uninitialized_fill_n(data(), size_, v);
  }

  /**
   * @brief Constructs a `StaticVector` equal to the range `[first, last)`
   *
   * Complexity: Initializing distance(first, last) <= capacity() of
   * `value_type`s
   */
  template <std::input_iterator Itr>
  StaticVector(Itr first, Itr last) //
      noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  {
    const auto distance = static_cast<size_type>(std::distance(first, last));
    BEYOND_ENSURE(distance <= capacity());
    size_ = distance;
    std::uninitialized_copy(first, last, data());
  }

  StaticVector(std::initializer_list<value_type> il)
      : size_{static_cast<size_type>(il.size())}
  {
    BEYOND_ENSURE(size_ <= capacity());
    std::uninitialized_copy(std::begin(il), std::end(il), data());
  }

  ~StaticVector() noexcept(std::is_nothrow_destructible_v<value_type>)
  {
    std::destroy_n(data(), size_);
  }

  StaticVector(const StaticVector& rhs) //
      noexcept(std::is_nothrow_copy_constructible_v<value_type>)
      : size_{rhs.size_}
  {
    std::uninitialized_copy_n(std::begin(rhs), rhs.size_, data());
  }

  auto operator=(const StaticVector& rhs) & noexcept(
      std::is_nothrow_copy_constructible_v<value_type>) -> StaticVector&
  {
    if (this != &rhs) {
      std::destroy_n(data(), size_);
      std::uninitialized_copy_n(std::begin(rhs), rhs.size_, data());
      size_ = rhs.size_;
    }
    return *this;
  }

  StaticVector(StaticVector&& rhs) //
      noexcept(std::is_nothrow_move_constructible_v<value_type>)
      : size_{rhs.size_}
  {
    std::uninitialized_move_n(std::begin(rhs), rhs.size_, data());
    rhs.size_ = 0;
  }

  auto operator=(StaticVector&& rhs) &                        //
      noexcept(std::is_nothrow_move_assignable_v<value_type>) //
      -> StaticVector&
  {
    if (this != &rhs) {
      std::destroy_n(data(), size_);
      std::uninitialized_move_n(std::begin(rhs), rhs.size_, data());
      size_ = std::exchange(rhs.size_, 0);
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
    BEYOND_ENSURE(size_ < capacity());

    new (Base::end()) T(std::forward<Args>(args)...);
    ++size_;
    return data()[size_ - 1];
  }

  /**
   * @brief Removes the last element of the container
   *
   * @warning Will `panic()` if `size() == 0`
   *
   * Complexity: O(1)
   */
  BEYOND_FORCE_INLINE auto pop_back() -> void
  {
    BEYOND_ENSURE(size_ != 0);
    data()[size_].~T();
    --size_;
  }

  /**
   * @brief Clear the `StaticVector`
   *
   * Complexity: O(n)
   */
  auto clear() -> void
  {
    std::destroy_n(data(), size_);
    size_ = 0;
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

  // TODO(lesley): erase, insert, resize, asign

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto begin() noexcept -> iterator
  {
    return iterator{data()};
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto begin() const noexcept
      -> const_iterator
  {
    return const_iterator{data()};
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
  friend constexpr void
  swap(StaticVector<T, N>& lhs,
       StaticVector<T, N>& rhs) noexcept(std::is_nothrow_swappable_v<T>)
  {
    lhs.swap(rhs);
  }

private:
  size_type size_ = 0;
  alignas(T) std::byte data_[sizeof(T) * N];
};

/** @}@} */

// TODO(lesley): lexicographically compares

} // namespace beyond

#endif // BEYOND_CORE_CONTAINER_STATIC_VECTOR_HPP
