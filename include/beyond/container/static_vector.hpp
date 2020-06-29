#pragma once

#ifndef BEYOND_CORE_CONTAINER_STATIC_VECTOR_HPP
#define BEYOND_CORE_CONTAINER_STATIC_VECTOR_HPP

#include <algorithm>
#include <iterator>
#include <type_traits>

#include <fmt/format.h>

#include "../types/optional.hpp"
#include "../utils/assert.hpp"
#include "../utils/bit_cast.hpp"
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
    std::uninitialized_value_construct_n(reinterpret_cast<T*>(data_), size_);
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
    std::uninitialized_fill_n(reinterpret_cast<T*>(data_), size_, v);
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
    std::uninitialized_copy(first, last, reinterpret_cast<T*>(data_));
  }

  constexpr StaticVector(std::initializer_list<value_type> il)
      : size_{static_cast<size_type>(il.size())}
  {
    BEYOND_ASSERT(size_ <= capacity());
    std::uninitialized_copy(std::begin(il), std::end(il),
                            reinterpret_cast<T*>(data_));
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
                              reinterpret_cast<T*>(data_));
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
                                reinterpret_cast<T*>(data_));
      size_ = rhs.size_;
    }
    return *this;
  }

  /**
   * @brief Gets the capacity of the `StaticVector`
   *
   * Complexity: O(1)
   */
  [[nodiscard]] constexpr auto capacity() const noexcept -> size_type
  {
    return N;
  }

  /**
   * @brief Gets the size of the `StaticVector`
   *
   * Complexity: O(1)
   */
  [[nodiscard]] constexpr auto size() const noexcept -> size_type
  {
    return size_;
  }

  /**
   * @brief Returns if the `StaticVector` is empty or not
   *
   * Complexity: O(1)
   */
  [[nodiscard]] constexpr auto empty() const noexcept -> size_type
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
  auto push_back(const value_type& value) -> reference
  {
    return emplace_back(value);
  }

  /// @overload
  template <typename... Args> auto push_back(value_type&& value) -> reference
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
  template <typename... Args> auto emplace_back(Args&&... args) -> reference
  {
    if (size() >= capacity()) {
      beyond::panic("Try to add to a to a full vector");
    }

    new (reinterpret_cast<T*>(data_) + size_) T(std::forward<Args>(args)...);
    ++size_;
    return reinterpret_cast<T*>(data_)[size_ - 1];
  }

  /**
   * @brief Removes the last element of the container
   *
   * @warning Will `panic()` if `size() == 0`, the result is undefined
   *
   * Complexity: O(1)
   */
  auto pop_back() -> void
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
    std::destroy_n(reinterpret_cast<T*>(data_), size_);
    size_ = 0;
  }

  /**
   * @brief Access an object at index `pos` with bounds checking
   * @warning If `pos > size()`, the result is undefined
   *
   * Complexity: O(1)
   */
  [[nodiscard]] constexpr auto operator[](size_type pos) const noexcept
      -> const_reference
  {
    if (pos >= size()) {
      beyond::panic("Accessing static_vector out-of-range");
    }

    return reinterpret_cast<const T*>(data_)[pos];
  }

  /// @overload
  [[nodiscard]] constexpr auto operator[](size_type pos) noexcept -> reference
  {
    if (pos >= size()) {
      beyond::panic("Accessing static_vector out-of-range");
    }

    return reinterpret_cast<T*>(data_)[pos];
  }

  /**
   * @brief access the first element
   * @warning Calling `front` on an empty container is undefined.
   *
   * Complexity: O(1)
   */
  [[nodiscard]] constexpr auto front() noexcept -> reference
  {
    return reinterpret_cast<T*>(data_)[0];
  }

  /// @overload
  [[nodiscard]] constexpr auto front() const noexcept -> const_reference
  {
    return reinterpret_cast<const T*>(data_)[0];
  }

  /**
   * @brief access the last element
   * @warning Calling `back` on an empty container is undefined.
   *
   * Complexity: O(1)
   */
  [[nodiscard]] constexpr auto back() noexcept -> reference
  {
    return reinterpret_cast<T*>(data_)[size_ - 1];
  }

  /// @overload
  [[nodiscard]] constexpr auto back() const noexcept -> const_reference
  {
    return reinterpret_cast<const T*>(data_)[size_ - 1];
  }

  /**
   * @brief Gets the underlying raw data pointer of the `StaticVector`
   *
   * Complexity: O(1)
   */
  [[nodiscard]] constexpr auto data() noexcept -> pointer
  {
    return reinterpret_cast<T*>(data_);
  }

  /// @overload
  [[nodiscard]] constexpr auto data() const noexcept -> const_pointer
  {
    return reinterpret_cast<const T*>(data_);
  }

  /**
   * @brief Access an object at index `pos` without bounds checking
   * @warning If `pos > size()`, the result is undefined
   *
   * Complexity: O(1)
   */
  [[nodiscard]] constexpr auto unsafe_at(size_type pos) const noexcept
      -> const_reference
  {
    return reinterpret_cast<const T*>(data_)[pos];
  }

  /// @overload
  [[nodiscard]] constexpr auto unsafe_at(size_type pos) noexcept -> reference
  {
    return reinterpret_cast<T*>(data_)[pos];
  }

  // TODO(lesley): erase, insert, resize, asign

  template <bool is_const = false> class Itr {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = std::conditional_t<is_const, const T&, T&>;
    using pointer = std::conditional_t<is_const, const T*, T*>;

    explicit constexpr Itr(pointer data = nullptr) : data_{data} {}

    [[nodiscard]] constexpr auto operator*() const noexcept -> reference
    {
      return *data_;
    }

    [[nodiscard]] constexpr auto operator->() const noexcept -> pointer
    {
      return data_;
    }

    constexpr auto operator+=(difference_type n) noexcept -> Itr&
    {
      data_ += n;
      return *this;
    }

    constexpr auto operator-=(difference_type n) noexcept -> Itr&
    {
      data_ -= n;
      return *this;
    }

    constexpr auto operator++() noexcept -> Itr&
    {
      ++data_;
      return *this;
    }

    constexpr auto operator--() noexcept -> Itr&
    {
      --data_;
      return *this;
    }

    constexpr auto operator++(int) noexcept -> Itr
    {
      return Itr{data_++};
    }

    constexpr auto operator--(int) noexcept -> Itr
    {
      return Itr{data_--};
    }

    [[nodiscard]] friend constexpr auto operator==(Itr lhs, Itr rhs) noexcept
        -> bool
    {
      return lhs.data_ == rhs.data_;
    }

    [[nodiscard]] friend constexpr auto operator!=(Itr lhs, Itr rhs) noexcept
        -> bool
    {
      return !(lhs == rhs);
    }

    [[nodiscard]] friend constexpr auto operator<(Itr lhs, Itr rhs) noexcept
        -> bool
    {
      return lhs.data_ < rhs.data_;
    }

    [[nodiscard]] friend constexpr auto operator<=(Itr lhs, Itr rhs) noexcept
        -> bool
    {
      return lhs.data_ <= rhs.data_;
    }

    [[nodiscard]] friend constexpr auto operator>(Itr lhs, Itr rhs) noexcept
        -> bool
    {
      return lhs.data_ > rhs.data_;
    }

    [[nodiscard]] friend constexpr auto operator>=(Itr lhs, Itr rhs) noexcept
        -> bool
    {
      return lhs.data_ >= rhs.data_;
    }

    [[nodiscard]] friend constexpr auto operator-(Itr lhs, Itr rhs) noexcept
        -> difference_type
    {
      return lhs.data_ - rhs.data_;
    }

    [[nodiscard]] friend constexpr auto operator+(Itr lhs,
                                                  difference_type rhs) noexcept
        -> Itr
    {
      return Itr{lhs.data_ + rhs};
    }

    [[nodiscard]] friend constexpr auto operator+(difference_type lhs,
                                                  Itr rhs) noexcept -> Itr
    {
      return Itr{lhs + rhs.data_};
    }

  private:
    pointer data_ = nullptr;
  };

  using iterator = Itr<false>;
  using const_iterator = Itr<true>;

  // TODO(lesley): reverse iterators
  [[nodiscard]] constexpr auto begin() noexcept -> iterator
  {
    return iterator{reinterpret_cast<T*>(data_)};
  }

  [[nodiscard]] constexpr auto end() noexcept -> iterator
  {
    return iterator{reinterpret_cast<T*>(data_) + size_};
  }

  [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator
  {
    return const_iterator{reinterpret_cast<const T*>(data_)};
  }

  [[nodiscard]] constexpr auto end() const noexcept -> const_iterator
  {
    return const_iterator{reinterpret_cast<const T*>(data_) + size_};
  }

  [[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator
  {
    return const_iterator{reinterpret_cast<const T*>(data_)};
  }

  [[nodiscard]] constexpr auto cend() const noexcept -> const_iterator
  {
    return const_iterator{reinterpret_cast<const T*>(data_) + size_};
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
