#pragma once

#ifndef BEYOND_CORE_CONTAINER_ARRAY_VIEW_HPP
#define BEYOND_CORE_CONTAINER_ARRAY_VIEW_HPP

#include <cstddef>
#include <type_traits>

#include "../types/optional.hpp"

/**
 * @defgroup container Container
 * @brief contains STL-style containers.
 * @ingroup core
 */

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup container
 * @{
 */

/**
 * @brief ArrayView is a view to a contiguous storage
 *
 * Anything invalidate the underlying pointer will invalidate the array
 * view.
 */
template <typename T> class ArrayView {
  T* data_ = nullptr;
  std::size_t size_ = 0;

public:
  using size_type = std::size_t;
  using value_type = T;

  /// @brief Default constructor create an ArrayView that points to nothing
  constexpr ArrayView() noexcept = default;

  /// @brief Constructs an ArrayView from a pointer and a size
  constexpr ArrayView(T* data, std::size_t size) noexcept
      : data_(data), size_(size)
  {
  }

  /*
   * @brief Constructs an ArrayView from a fixed-sized c array
   * @tparam U A type such that `U*` is_nothrow_convertible `T*`
   */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
  template <class U, std::size_t size>
#else
  template <
      class U, std::size_t size,
      class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
#endif
  constexpr ArrayView(U (&data)[size]) noexcept : data_(data), size_(size)
  {
  }

  /**
   * @brief Constructs an `ArrayView<T>` from a `ArrayView<U>`
   *
   * @tparam U A type such that `U*` is_nothrow_convertible `T*`
   * @note T and U must have the same size
   */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
  template <class U>
#else
  template <class U, class = typename std::enable_if<
                         std::is_nothrow_convertible<U*, T*>::value>::type>
#endif
  constexpr /*implicit*/ ArrayView(ArrayView<U> view) noexcept
      : data_{view.data()}, size_{view.size()}
  {
    static_assert(sizeof(T) == sizeof(U));
  }

  /// @brief Returns true if the ArrayView is empty
  [[nodiscard]] constexpr auto empty() const noexcept -> bool
  {
    return data_ == nullptr;
  }

  /// @brief Returns the underlying data of array view
  [[nodiscard]] constexpr auto data() const noexcept -> value_type*
  {
    return data_;
  }

  /// @brief Returns the size of array view
  [[nodiscard]] constexpr auto size() const noexcept -> size_type
  {
    return size_;
  }

  /// @brief Implicit conversion to the underlying pointer
  constexpr /*implicit*/ operator T*() const noexcept
  {
    return data_;
  }

  /**
   * @name begin
   * @brief Pointer to first element
   */
  /// @{
  [[nodiscard]] constexpr auto begin() noexcept -> T*
  {
    return data_;
  }
  [[nodiscard]] constexpr auto begin() const noexcept -> const T*
  {
    return data_;
  }
  [[nodiscard]] constexpr auto cbegin() const noexcept -> const T*
  {
    return data_;
  }
  /// @}

  /**
   * @name end
   * @brief Pointer to one element after last element
   */
  /// @{
  [[nodiscard]] constexpr auto end() noexcept -> T*
  {
    return data_ + size_;
  }
  [[nodiscard]] constexpr auto end() const noexcept -> const T*
  {
    return data_ + size_;
  }
  [[nodiscard]] constexpr auto cend() const noexcept -> const T*
  {
    return data_ + size_;
  }
  /// @}

  /**
   * @name operator[]
   * @brief Returns reference to the element at index, `panic()` if out of index
   */
  /// @{
  [[nodiscard]] constexpr auto operator[](std::size_t index) noexcept -> T&
  {
    if (index >= size_) beyond::panic("Out of index accessing of ArrayView");
    return data_[index];
  }

  [[nodiscard]] constexpr auto operator[](std::size_t index) const noexcept
      -> const T&
  {
    if (index >= size_) beyond::panic("Out of index accessing of ArrayView");
    return data_[index];
  }
  /// @}

  /**
   * @name at_opt
   * @brief Returns a reference to the element at index, nullopt if out of index
   */
  /// @{
  [[nodiscard]] constexpr auto at_opt(std::size_t index) noexcept
      -> optional<T&>
  {
    if (index >= size_) return beyond::nullopt;
    return data_[index];
  }

  [[nodiscard]] constexpr auto at_opt(std::size_t index) const noexcept
      -> optional<const T&>
  {
    if (index >= size_) return beyond::nullopt;
    return data_[index];
  }
  /// @}
};

/** @}@} */

} // namespace beyond

#endif // BEYOND_CORE_CONTAINER_ARRAY_VIEW_HPP
