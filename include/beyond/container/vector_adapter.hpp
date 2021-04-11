#ifndef BEYOND_CORE_CONTAINER_VECTOR_ADAPTER_HPP
#define BEYOND_CORE_CONTAINER_VECTOR_ADAPTER_HPP

#include "../utils/force_inline.hpp"
#include "../utils/panic.hpp"

namespace beyond {

/**
 * @ingroup container
 * @tparam Vec
 */
template <class Vec> struct ContainerTrait;

/**
 * @brief Helper class for defining vector-like containers, use CRTP
 * @ingroup container
 * The Derived type need to provide
 * - an implementation of ContainerTrait
 * - data(), begin(), size()
 */
template <class Derived> struct VectorInterface {
  using Trait = ContainerTrait<Derived>;
  using size_type = typename Trait::size_type;
  using reference = typename Trait::reference;
  using const_reference = typename Trait::const_reference;
  using iterator = typename Trait::iterator;
  using const_iterator = typename Trait::const_iterator;
  using reverse_iterator = typename Trait::reverse_iterator;
  using reverse_const_iterator = typename Trait::reverse_const_iterator;

  /**
   * @brief Returns if the vector is empty or not
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto empty() const noexcept
      -> size_type
  {
    return underlying().size() == 0;
  }

  /**
   * @brief access the first element
   * @warning Will `panic` if call `front` on an empty container
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto front() noexcept -> reference
  {
    BEYOND_ENSURE(underlying().size() != 0);
    return underlying().data()[0];
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto front() const noexcept
      -> const_reference
  {
    BEYOND_ENSURE(underlying().size() != 0);
    return underlying().data()[0];
  }

  /**
   * @brief access the last element
   * @warning Will `panic` if call `back` on an empty container
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto back() noexcept -> reference
  {
    BEYOND_ENSURE(underlying().size() != 0);
    return underlying().data()[underlying().size() - 1];
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto back() const noexcept
      -> const_reference
  {
    BEYOND_ENSURE(underlying().size() != 0);
    return underlying().data()[underlying().size() - 1];
  }

  /**
   * @brief Access an object at index `pos` with bounds checking
   * @warning Will `panic` if `pos >= size()`
   *
   * Complexity: O(1)
   */
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
  operator[](size_type pos) const noexcept -> const_reference
  {
    BEYOND_ENSURE(pos < underlying().size());
    return underlying().data()[pos];
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
  operator[](size_type pos) noexcept -> reference
  {
    BEYOND_ENSURE(pos < underlying().size());
    return underlying().data()[pos];
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
    BEYOND_ASSERT(pos < underlying().size());
    return underlying().data()[pos];
  }

  /// @overload
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
  unsafe_at(size_type pos) noexcept -> reference
  {
    BEYOND_ASSERT(pos < underlying().size());
    return underlying().data()[pos];
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto end() noexcept -> iterator
  {
    return underlying().begin() + underlying().size();
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto end() const noexcept
      -> const_iterator
  {
    return underlying().begin() + underlying().size();
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto rbegin() noexcept
      -> reverse_iterator
  {
    return std::make_reverse_iterator(end());
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto rbegin() const noexcept
      -> reverse_const_iterator
  {
    return std::make_reverse_iterator(end());
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto rend() noexcept
      -> reverse_iterator
  {
    return std::make_reverse_iterator(underlying().begin());
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto rend() const noexcept
      -> reverse_const_iterator
  {
    return std::make_reverse_iterator(underlying().begin());
  }

  // TODO(lesley): reverse iterators

private:
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto underlying() noexcept
      -> Derived&
  {
    return static_cast<Derived&>(*this);
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto underlying() const noexcept
      -> const Derived&
  {
    return static_cast<const Derived&>(*this);
  }
};

} // namespace beyond

#endif // BEYOND_CORE_CONTAINER_VECTOR_ADAPTER_HPP
