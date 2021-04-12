#ifndef BEYOND_CORE_TYPES_UNIQUE_PTR_HPP
#define BEYOND_CORE_TYPES_UNIQUE_PTR_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

namespace beyond {

/**
 * @brief  A lightweight alternative to std::unique_ptr
 *
 * Difference to std::unique_ptr
 * - Full constexpr support
 * - No custom deleter support. Use `std::unique_ptr` if you want to have custom
 * deleter.
 * - No array support (yet)
 */
template <typename T> class UniquePtr {
  T* ptr_ = nullptr;

public:
  using pointer = T*;
  using reference = T&;
  using element_type = T;

  constexpr UniquePtr() = default;

  /// @brief Construct from nullptr
  constexpr UniquePtr(std::nullptr_t) noexcept {}

  /// @brief Construct from an owning pointer
  explicit constexpr UniquePtr(pointer ptr) : ptr_{ptr} {}

  constexpr ~UniquePtr()
  {
    delete ptr_;
  }
  constexpr UniquePtr(const UniquePtr&) = delete;
  constexpr auto operator=(const UniquePtr&) -> UniquePtr& = delete;
  constexpr UniquePtr(UniquePtr&& rhs) noexcept
      : ptr_{std::exchange(rhs.ptr_, nullptr)}
  {
  }

  constexpr auto operator=(UniquePtr&& rhs) & noexcept -> UniquePtr&
  {
    ptr_ = std::exchange(rhs.ptr_, nullptr);
  }

  /// @brief Conversion constructor. Useful for covariant conversion.
  template <class U>
  constexpr UniquePtr(UniquePtr<U>&& u) noexcept
      requires(std::is_convertible_v<U*, pointer> && !std::is_array_v<U> &&
               !std::is_same_v<T, U>)
      : ptr_{u.release()}
  {
  }

  [[nodiscard]] constexpr auto operator*() const noexcept -> reference
  {
    return *ptr_;
  }

  [[nodiscard]] constexpr auto operator->() const noexcept -> pointer
  {
    return ptr_;
  }

  /// @brief Returns a pointer to the managed object or nullptr if no object is
  /// owned.
  [[nodiscard]] constexpr auto get() const noexcept -> pointer
  {
    return ptr_;
  }

  /// @brief Converts this UniquePtr to a boolean, false means it is a nullptr.
  explicit constexpr operator bool() const noexcept
  {
    return ptr_ != nullptr;
  }

  /// @brief returns a pointer to the managed object and releases the ownership
  [[nodiscard]] constexpr auto release() noexcept -> pointer
  {
    return std::exchange(ptr_, nullptr);
  }

  /// @brief Replaces the managed object
  constexpr void reset(pointer ptr = pointer{}) noexcept
  {
    delete ptr_;
    ptr_ = ptr;
  }

  /// @brief swaps the managed objects
  constexpr void swap(UniquePtr rhs) noexcept
  {
    std::swap(ptr_, rhs.ptr_);
  }

  /// @brief Equality comparison between an UniquePtr and nullptr
  constexpr friend auto operator==(const UniquePtr& lhs, nullptr_t) -> bool
  {
    return lhs.ptr_ == nullptr;
  }

  /// @brief Threeway comparison between an UniquePtr and nullptr
  constexpr friend auto operator<=>(const UniquePtr& lhs, nullptr_t)
      -> std::strong_ordering
  {
    return lhs.ptr_ <=> nullptr;
  }

  /// @brief Equality comparison between two UniquePtr
  constexpr friend auto operator==(const UniquePtr& lhs, const UniquePtr& rhs)
      -> bool
  {
    return lhs.ptr_ == rhs.ptr_;
  }

  /// @brief Threeway comparison between two UniquePtr
  friend auto operator<=>(const UniquePtr& lhs, const UniquePtr& rhs)
      -> std::strong_ordering
  {
    return lhs.ptr_ <=> rhs.ptr_;
  }
};

// Array is not supported yet
template <typename T> class UniquePtr<T[]>;

/// @brief Swap free function for UniquePtr
template <typename T> constexpr auto swap(UniquePtr<T>& lhs, UniquePtr<T>& rhs)
{
  lhs.swap(rhs);
}

/// @brief creates a unique pointer that manages a new object
/// @related UniquePtr
template <class T, class... Args>
constexpr auto make_unique(Args&&... args) -> UniquePtr<T> //
    requires(!std::is_array_v<T>)
{
  return UniquePtr<T>{new T(std::forward<Args>(args)...)};
}

/// @brief Like make_unique, but the underlying object is default constructed
/// @related UniquePtr
template <class T>
constexpr auto make_unique_for_overwrite() -> UniquePtr<T> //
    requires(!std::is_array_v<T>)
{
  return UniquePtr<T>{new T};
}

} // namespace beyond

#endif // BEYOND_CORE_TYPES_UNIQUE_PTR_HPP
