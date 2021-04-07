/*
 expected.hpp
 An implementation of std::expected with extensions
 Written in 2017 by Simon Brand (simonrbrand@gmail.com, @TartanLlama)
 Forked and modified in 2020 by Lesley Lai
*/

#ifndef BEYOND_CORE_TYPES_EXPECTED_HPP
#define BEYOND_CORE_TYPES_EXPECTED_HPP

#include <functional>
#include <type_traits>
#include <utility>

#include "../utils/panic.hpp"
#include "in_place.hpp"
#include "monostate.hpp"

namespace beyond {

namespace detail {

template <class T> concept NotVoid = !std::is_void_v<T>;

}

/**
 * @addtogroup core
 * @{
 * @addtogroup types
 * @{
 */

template <class T, class E> class expected;

template <detail::NotVoid E> class unexpected {
public:
  unexpected() = delete;
  constexpr explicit unexpected(const E& e) : val_(e) {}

  constexpr explicit unexpected(E&& e) : val_(std::move(e)) {}

  [[nodiscard]] constexpr auto value() const& -> const E&
  {
    return val_;
  }
  [[nodiscard]] constexpr auto value() & -> E&
  {
    return val_;
  }
  [[nodiscard]] constexpr auto value() && -> E&&
  {
    return std::move(val_);
  }
  [[nodiscard]] constexpr auto value() const&& -> const E&&
  {
    return std::move(val_);
  }

  [[nodiscard]] friend constexpr auto
  operator==(const unexpected& lhs, const unexpected& rhs) -> bool = default;

  [[nodiscard]] friend constexpr auto
  operator<=>(const unexpected<E>& lhs, const unexpected<E>& rhs) = default;

private:
  E val_;
};

template <class E>
constexpr unexpected<typename std::decay<E>::type> make_unexpected(E&& e)
{
  return unexpected<typename std::decay<E>::type>(std::forward<E>(e));
}

struct unexpect_t {
  unexpect_t() = default;
};
static constexpr unexpect_t unexpect{};

namespace detail {

// Trait for checking if a type is a beyond::expected
template <class T> struct is_expected_impl : std::false_type {
};
template <class T, class E>
struct is_expected_impl<expected<T, E>> : std::true_type {
};
template <class T> using is_expected = is_expected_impl<std::decay_t<T>>;

template <class T, class E, class U>
concept support_expected_forward_value =
    std::is_constructible_v<T, U&&> &&
    !std::is_same_v<std::decay_t<U>, in_place_t> &&
    !std::is_same_v<expected<T, E>, std::decay_t<U>> &&
    !std::is_same_v<unexpected<E>, std::decay_t<U>>;

template <class T, class E, class U, class G, class UR, class GR>
concept support_expected_from_other =
    std::is_constructible_v<T, UR>&& std::is_constructible_v<E, GR> &&
    !std::is_constructible_v<T, expected<U, G>&> &&
    !std::is_constructible_v<T, expected<U, G>&&> &&
    !std::is_constructible_v<T, const expected<U, G>&> &&
    !std::is_constructible_v<T, const expected<U, G>&&> &&
    !std::is_convertible_v<expected<U, G>&, T> &&
    !std::is_convertible_v<expected<U, G>&&, T> &&
    !std::is_convertible_v<const expected<U, G>&, T> &&
    !std::is_convertible_v<const expected<U, G>&&, T>;

template <class T>
concept support_expected_trivial_copy_assignment =
    std::is_trivially_copy_assignable_v<T>&&
        std::is_trivially_copy_constructible_v<T>&&
            std::is_trivially_destructible_v<T>;

template <class T>
concept support_expected_trivial_move_assignment =
    std::is_trivially_move_assignable_v<T>&&
        std::is_trivially_move_constructible_v<T>&&
            std::is_trivially_destructible_v<T>;

template <class T>
concept support_expected_copy_assignment =
    std::is_copy_constructible_v<T>&& std::is_copy_assignable_v<T>;

template <class T>
concept support_expected_move_assignment =
    std::is_move_constructible_v<T>&& std::is_move_assignable_v<T>;

} // namespace detail

namespace detail {
struct no_init_t {
};
static constexpr no_init_t no_init{};

// Implements the storage of the values, and ensures that the destructor is
// trivial if it can be.
template <class T, class E> struct expected_storage_base {
  union {
    T m_val;
    unexpected<E> m_unexpect;
    char m_no_init;
  };
  bool m_has_val;

  constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
  constexpr expected_storage_base() requires(
      !std::is_default_constructible_v<T>) = delete;

  constexpr expected_storage_base(no_init_t) : m_no_init(), m_has_val(false) {}

  template <class... Args>
  constexpr expected_storage_base(in_place_t, Args&&... args) //
      requires(std::is_constructible_v<T, Args&&...>)
      : m_val(std::forward<Args>(args)...), m_has_val(true)
  {
  }

  template <class U, class... Args>
  constexpr expected_storage_base(
      in_place_t, std::initializer_list<U> il,
      Args&&... args) //
      requires(std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>)
      : m_val(il, std::forward<Args>(args)...), m_has_val(true)
  {
  }
  template <class... Args>
  constexpr explicit expected_storage_base(unexpect_t, Args&&... args) //
      requires(std::is_constructible_v<E, Args&&...>)
      : m_unexpect(std::forward<Args>(args)...), m_has_val(false)
  {
  }

  template <class U, class... Args>
  constexpr explicit expected_storage_base(
      unexpect_t, std::initializer_list<U> il, Args&&... args) //
      requires(std::is_constructible_v<E, std::initializer_list<U>&, Args&&...>)
      : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false)
  {
  }

  constexpr ~expected_storage_base()
  {
    if (m_has_val) {
      if constexpr (!std::is_trivially_destructible_v<T>) m_val.~T();
    } else {
      if constexpr (!std::is_trivially_destructible_v<E>) {
        m_unexpect.~unexpected<E>();
      }
    }
  }

  // If both T and E are trivially destructible, the destructor can be trivial
  constexpr ~expected_storage_base() noexcept
      requires(std::is_trivially_destructible_v<T>&&
                   std::is_trivially_destructible_v<E>) = default;

  template <class... Args> constexpr void construct(Args&&... args) noexcept
  {
    new (std::addressof(this->m_val)) T(std::forward<Args>(args)...);
    this->m_has_val = true;
  }

  template <class Rhs> constexpr void construct_with(Rhs&& rhs) noexcept
  {
    new (std::addressof(this->m_val)) T(*std::forward<Rhs>(rhs));
    this->m_has_val = true;
  }

  template <class... Args>
  constexpr void construct_error(Args&&... args) noexcept
  {
    new (std::addressof(this->m_unexpect))
        unexpected<E>(std::forward<Args>(args)...);
    this->m_has_val = false;
  }

  // These assign overloads ensure that the most efficient assignment
  // implementation is used while maintaining the strong exception guarantee.
  // The problematic case is where rhs has a value, but *this does not.
  //
  // This overload handles the case where we can just copy-construct `T`
  // directly into place without throwing.
  template <class U = T>
  constexpr void assign(const expected_storage_base& rhs) noexcept
      requires(std::is_nothrow_copy_constructible_v<U>)
  {
    if (!this->m_has_val && rhs.m_has_val) {
      geterr().~unexpected<E>();
      construct(rhs.get());
    } else {
      assign_common(rhs);
    }
  }

  // This overload handles the case where we can attempt to create a copy of
  // `T`, then no-throw move it into place if the copy was successful.
  template <class U = T>
  constexpr void assign(const expected_storage_base& rhs) noexcept requires(
      !std::is_nothrow_copy_constructible_v<U> &&
      std::is_nothrow_move_constructible_v<U>)
  {
    if (!this->m_has_val && rhs.m_has_val) {
      T tmp = rhs.get();
      geterr().~unexpected<E>();
      construct(std::move(tmp));
    } else {
      assign_common(rhs);
    }
  }

  // This overload is the worst-case, where we have to move-construct the
  // unexpected value into temporary storage, then try to copy the T into place.
  // If the construction succeeds, then everything is fine, but if it throws,
  // then we move the old unexpected value back into place before rethrowing the
  // exception.
  template <class U = T>
  constexpr void assign(const expected_storage_base& rhs) requires(
      !std::is_nothrow_copy_constructible_v<U> &&
      !std::is_nothrow_move_constructible_v<U>)
  {
    if (!this->m_has_val && rhs.m_has_val) {
      auto tmp = std::move(geterr());
      geterr().~unexpected<E>();
      try {
        construct(rhs.get());
      } catch (...) {
        geterr() = std::move(tmp);
        throw;
      }
    } else {
      assign_common(rhs);
    }
  }

  // These overloads do the same as above, but for rvalues
  template <class U = T>
  constexpr void assign(expected_storage_base&& rhs) noexcept
      requires(std::is_nothrow_move_constructible_v<U>)
  {
    if (!this->m_has_val && rhs.m_has_val) {
      geterr().~unexpected<E>();
      construct(std::move(rhs).get());
    } else {
      assign_common(std::move(rhs));
    }
  }

  template <class U = T>
  constexpr void assign(expected_storage_base&& rhs) //
      requires(!std::is_nothrow_move_constructible_v<U>)
  {
    if (!this->m_has_val && rhs.m_has_val) {
      auto tmp = std::move(geterr());
      geterr().~unexpected<E>();
      try {
        construct(std::move(rhs).get());
      } catch (...) {
        geterr() = std::move(tmp);
        throw;
      }
    } else {
      assign_common(std::move(rhs));
    }
  }

  // The common part of move/copy assigning
  template <class Rhs> constexpr void assign_common(Rhs&& rhs)
  {
    if (this->m_has_val) {
      if (rhs.m_has_val) {
        get() = std::forward<Rhs>(rhs).get();
      } else {
        destroy_val();
        construct_error(std::forward<Rhs>(rhs).geterr());
      }
    } else {
      if (!rhs.m_has_val) { geterr() = std::forward<Rhs>(rhs).geterr(); }
    }
  }

  [[nodiscard]] constexpr auto has_value() const -> bool
  {
    return this->m_has_val;
  }

  [[nodiscard]] constexpr auto get() & -> T&
  {
    return this->m_val;
  }
  [[nodiscard]] constexpr auto get() const& -> const T&
  {
    return this->m_val;
  }
  [[nodiscard]] constexpr auto get() && -> T&&
  {
    return std::move(this->m_val);
  }
  [[nodiscard]] constexpr auto get() const&& -> const T&&
  {
    return std::move(this->m_val);
  }

  [[nodiscard]] constexpr auto geterr() & -> unexpected<E>&
  {
    return this->m_unexpect;
  }
  [[nodiscard]] constexpr auto geterr() const& -> const unexpected<E>&
  {
    return this->m_unexpect;
  }
  [[nodiscard]] constexpr auto geterr() && -> unexpected<E>&&
  {
    return std::move(this->m_unexpect);
  }
  [[nodiscard]] constexpr auto geterr() const&& -> const unexpected<E>&&
  {
    return std::move(this->m_unexpect);
  }

  constexpr void destroy_val()
  {
    get().~T();
  }
};

// `T` is `void`
template <class E> struct expected_storage_base<void, E> {

  struct dummy {
  };
  union {
    unexpected<E> m_unexpect;
    dummy m_val;
  };
  bool m_has_val;

  constexpr expected_storage_base() : m_has_val(true) {}
  constexpr expected_storage_base(no_init_t) : m_val(), m_has_val(false) {}
  constexpr expected_storage_base(in_place_t) : m_has_val(true) {}

  template <class... Args>
  constexpr explicit expected_storage_base(unexpect_t, Args&&... args) //
      requires(std::is_constructible_v<E, Args&&...>)
      : m_unexpect(std::forward<Args>(args)...), m_has_val(false)
  {
  }

  template <class U, class... Args>
  constexpr explicit expected_storage_base(
      unexpect_t, std::initializer_list<U> il, Args&&... args) //
      requires(std::is_constructible_v<E, std::initializer_list<U>&, Args&&...>)
      : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false)
  {
  }

  constexpr ~expected_storage_base()
  {
    if (!m_has_val) { m_unexpect.~unexpected<E>(); }
  }

  constexpr ~expected_storage_base()                //
      requires(std::is_trivially_destructible_v<E>) //
      = default;

  template <class... Args> constexpr void construct() noexcept
  {
    this->m_has_val = true;
  }

  // This function doesn't use its argument, but needs it so that code in
  // levels above this can work independently of whether T is void
  template <class Rhs> constexpr void construct_with(Rhs&&) noexcept
  {
    this->m_has_val = true;
  }

  template <class... Args>
  constexpr void construct_error(Args&&... args) noexcept
  {
    new (std::addressof(this->m_unexpect))
        unexpected<E>(std::forward<Args>(args)...);
    this->m_has_val = false;
  }

  template <class Rhs> constexpr void assign(Rhs&& rhs) noexcept
  {
    if (!this->m_has_val) {
      if (rhs.m_has_val) {
        geterr().~unexpected<E>();
        construct();
      } else {
        geterr() = std::forward<Rhs>(rhs).geterr();
      }
    } else {
      if (!rhs.m_has_val) { construct_error(std::forward<Rhs>(rhs).geterr()); }
    }
  }

  [[nodiscard]] constexpr auto has_value() const -> bool
  {
    return this->m_has_val;
  }

  [[nodiscard]] constexpr auto geterr() & -> unexpected<E>&
  {
    return this->m_unexpect;
  }
  [[nodiscard]] constexpr auto geterr() const& -> const unexpected<E>&
  {
    return this->m_unexpect;
  }
  [[nodiscard]] constexpr auto geterr() && -> unexpected<E>&&
  {
    return std::move(this->m_unexpect);
  }
  [[nodiscard]] constexpr auto geterr() const&& -> const unexpected<E>&&
  {
    return std::move(this->m_unexpect);
  }

  constexpr void destroy_val()
  {
    // no-op
  }
};

} // namespace detail

/// An `expected<T, E>` object is an object that contains the storage for
/// another object and manages the lifetime of this contained object `T`.
/// Alternatively it could contain the storage for another unexpected object
/// `E`. The contained object may not be initialized after the expected object
/// has been initialized, and may not be destroyed before the expected object
/// has been destroyed. The initialization state of the contained object is
/// tracked by the expected object.
template <class T, class E>
class expected : private detail::expected_storage_base<T, E> {
  static_assert(!std::is_reference<T>::value, "T must not be a reference");
  static_assert(
      !std::is_same<T, std::remove_cv<in_place_t>>::value,
      "T must not be in_place_t");
  static_assert(
      !std::is_same<T, std::remove_cv<unexpect_t>>::value,
      "T must not be unexpect_t");
  static_assert(
      !std::is_same<T, std::remove_cv<unexpected<E>>>::value,
      "T must not be unexpected<E>");
  static_assert(!std::is_reference<E>::value, "E must not be a reference");

  constexpr auto get()
  {
    return detail::expected_storage_base<T, E>::get();
  }

  T* valptr()
  {
    return std::addressof(this->m_val);
  }
  const T* valptr() const
  {
    return std::addressof(this->m_val);
  }
  unexpected<E>* errptr()
  {
    return std::addressof(this->m_unexpect);
  }
  const unexpected<E>* errptr() const
  {
    return std::addressof(this->m_unexpect);
  }

  template <class U = T> constexpr U& val() requires(!std::is_void_v<U>)
  {
    return this->m_val;
  }
  constexpr unexpected<E>& err()
  {
    return this->m_unexpect;
  }

  template <class U = T>
  constexpr const U& val() const requires(!std::is_void_v<U>)
  {
    return this->m_val;
  }
  constexpr const unexpected<E>& err() const
  {
    return this->m_unexpect;
  }

  using impl_base = detail::expected_storage_base<T, E>;

public:
  typedef T value_type;
  typedef E error_type;
  typedef unexpected<E> unexpected_type;

  using detail::expected_storage_base<T, E>::expected_storage_base;

  constexpr expected() = default;

  // Copy constructor
  constexpr expected(const expected& rhs)
      : detail::expected_storage_base<T, E>(detail::no_init)
  {
    if (rhs.has_value()) {
      this->construct_with(rhs);
    } else {
      this->construct_error(rhs.geterr());
    }
  }
  constexpr expected(const expected&) //
      requires(
          (std::is_void_v<T> || std::is_trivially_copy_constructible_v<T>) //
          &&std::is_trivially_copy_constructible_v<E>)                     //
      = default;
  constexpr expected(const expected&) //
      requires(
          !(std::is_void_v<T> || std::is_copy_constructible_v<T>) ||
          !std::is_copy_constructible_v<E>) //
      = delete;

  // Move constrcutor
  constexpr expected(expected&& rhs) noexcept : expected<T, E>(detail::no_init)
  {
    static_assert(std::is_nothrow_move_constructible_v<T>);
    static_assert(std::is_nothrow_move_constructible_v<E>);
    if (rhs.has_value()) {
      this->construct_with(std::move(rhs));
    } else {
      this->construct_error(std::move(rhs.geterr()));
    }
  }
  constexpr expected(expected&& rhs) noexcept //
      requires(
          (std::is_void_v<T> || std::is_trivially_move_constructible_v<T>) //
          &&std::is_trivially_move_constructible_v<E>)                     //
      = default;
  constexpr expected(expected&&) noexcept //
      requires(
          !(std::is_void_v<T> || std::is_move_constructible_v<T>) ||
          !std::is_move_constructible_v<E>) = delete;

  // Copy assignment
  constexpr auto operator=(const expected& rhs) -> expected&
  {
    this->assign(rhs);
    return *this;
  }
  constexpr auto operator=(const expected& rhs) -> expected& //
      requires(
          (std::is_void_v<T> ||
           detail::support_expected_trivial_copy_assignment<T>) //
          &&detail::support_expected_trivial_copy_assignment<E>) = default;
  constexpr auto operator=(const expected& rhs) -> expected& //
      requires(
          !(std::is_void_v<T> || detail::support_expected_copy_assignment<T>) ||
          !detail::support_expected_copy_assignment<E>) = delete;

  // Move assignment
  constexpr auto operator=(expected&& rhs) noexcept -> expected&
  {
    static_assert(
        std::is_nothrow_move_constructible_v<T> &&
        std::is_nothrow_move_assignable_v<T>);
    this->assign(std::move(rhs));
    return *this;
  }
  constexpr auto operator=(expected&& rhs) noexcept -> expected& //
      requires(
          (std::is_void_v<T> ||
           detail::support_expected_trivial_move_assignment<T>)  //
          &&detail::support_expected_trivial_move_assignment<E>) //
      = default;
  constexpr auto operator=(expected&& rhs) noexcept -> expected& //
      requires(
          !(std::is_void_v<T> || detail::support_expected_move_assignment<T>) ||
          !detail::support_expected_move_assignment<E>) //
      = delete;

  /**
   * @name and_then
   * @brief  Carries out some operation which returns an expected on the stored
   * object if there is one.
   * @param f A function takes T and return the a expected<T2, E>
   */
  /// @{
  template <class F> constexpr auto and_then(F&& f) &
  {
    return and_then_impl(*this, std::forward<F>(f));
  }

  template <class F> constexpr auto and_then(F&& f) &&
  {
    return and_then_impl(std::move(*this), std::forward<F>(f));
  }

  template <class F> constexpr auto and_then(F&& f) const&
  {
    return and_then_impl(*this, std::forward<F>(f));
  }

  template <class F> constexpr auto and_then(F&& f) const&&
  {
    return and_then_impl(std::move(*this), std::forward<F>(f));
  }
  /// @}

  /**
   * @name map
   * @brief  Carries out some operation on the stored object if there is one.
   * @param f A function takes T and return the a T2
   *
   * Functor map operation.
   */
  /// @{
  template <class F> constexpr auto map(F&& f) &
  {
    return expected_map_impl(*this, std::forward<F>(f));
  }
  template <class F> constexpr auto map(F&& f) &&
  {
    return expected_map_impl(std::move(*this), std::forward<F>(f));
  }
  template <class F> constexpr auto map(F&& f) const&
  {
    return expected_map_impl(*this, std::forward<F>(f));
  }
  template <class F> constexpr auto map(F&& f) const&&
  {
    return expected_map_impl(std::move(*this), std::forward<F>(f));
  }

  template <class F> constexpr auto transform(F&& f) &
  {
    return expected_map_impl(*this, std::forward<F>(f));
  }
  template <class F> constexpr auto transform(F&& f) &&
  {
    return expected_map_impl(std::move(*this), std::forward<F>(f));
  }
  template <class F> constexpr auto transform(F&& f) const&
  {
    return expected_map_impl(*this, std::forward<F>(f));
  }
  template <class F> constexpr auto transform(F&& f) const&&
  {
    return expected_map_impl(std::move(*this), std::forward<F>(f));
  }
  /// @}

  /**
   * @name map_error
   * @brief Carries out some operation which returns another expected on the
   * stored error if there is one.
   * @param f A function takes E and return the an expected<E2>
   */
  /// @{
  template <class F> constexpr auto map_error(F&& f) &
  {
    return map_error_impl(*this, std::forward<F>(f));
  }
  template <class F> constexpr auto map_error(F&& f) &&
  {
    return map_error_impl(std::move(*this), std::forward<F>(f));
  }
  template <class F> constexpr auto map_error(F&& f) const&
  {
    return map_error_impl(*this, std::forward<F>(f));
  }
  template <class F> constexpr auto map_error(F&& f) const&&
  {
    return map_error_impl(std::move(*this), std::forward<F>(f));
  }
  /// @}

  /**
   * @name or_else
   * @brief Carries out some operation on the stored error if there is one.
   * @param f A function takes E and return the a E2
   */
  /// @{
  template <class F> expected constexpr or_else(F&& f) &
  {
    return or_else_impl(*this, std::forward<F>(f));
  }

  template <class F> expected constexpr or_else(F&& f) &&
  {
    return or_else_impl(std::move(*this), std::forward<F>(f));
  }

  template <class F> expected constexpr or_else(F&& f) const&
  {
    return or_else_impl(*this, std::forward<F>(f));
  }

  template <class F> expected constexpr or_else(F&& f) const&&
  {
    return or_else_impl(std::move(*this), std::forward<F>(f));
  }
  /// @}

  /**
   * @name constructors
   */
  /// @{

  template <class... Args>
  constexpr expected(in_place_t, Args&&... args) requires(
      std::is_constructible_v<T, Args&&...>)
      : impl_base(in_place, std::forward<Args>(args)...)
  {
  }

  template <class U, class... Args>
  requires(std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>) //
      constexpr expected(
          in_place_t, std::initializer_list<U> il, Args&&... args)

      : impl_base(in_place, il, std::forward<Args>(args)...)
  {
  }

  template <class G = E>
  explicit(!std::is_convertible_v<const G&, E>)      //
      constexpr expected(const unexpected<G>& e)     //
      requires(std::is_constructible_v<E, const G&>) //
      : impl_base(unexpect, e.value())
  {
  }

  template <class G = E>
  requires(std::is_constructible_v<E, G&&>)    //
      explicit(!std::is_convertible_v<G&&, E>) //
      constexpr expected(unexpected<G>&& e)    //
      noexcept(std::is_nothrow_constructible_v<E, G&&>)
      : impl_base(unexpect, std::move(e.value()))
  {
  }

  template <class... Args>
  requires(std::is_constructible_v<E, Args&&...>) //
      constexpr explicit expected(unexpect_t, Args&&... args)
      : impl_base(unexpect, std::forward<Args>(args)...)
  {
  }

  template <class U, class... Args>
  requires(std::is_constructible_v<E, std::initializer_list<U>&, Args&&...>) //
      constexpr explicit expected(
          unexpect_t, std::initializer_list<U> il, Args&&... args)
      : impl_base(unexpect, il, std::forward<Args>(args)...)
  {
  }

  template <class U, class G>
  requires(
      detail::support_expected_from_other<T, E, U, G, const U&, const G&>) //
      explicit(
          !(std::is_convertible_v<U const&, T> &&
            std::is_convertible_v<G const&, E>)) //
      constexpr expected(const expected<U, G>& rhs)
  {
    if (rhs.has_value()) {
      this->construct(*rhs);
    } else {
      this->construct_error(rhs.error());
    }
  }

  template <class U, class G>
  requires(detail::support_expected_from_other<T, E, U, G, U&&, G&&>) //
      explicit(
          !(std::is_convertible_v<U&&, T> && std::is_convertible_v<G&&, E>)) //
      constexpr expected(expected<U, G>&& rhs)
  {
    if (rhs.has_value()) {
      this->construct(std::move(*rhs));
    } else {
      this->construct_error(std::move(rhs.error()));
    }
  }

  template <class U = T>
  requires(detail::support_expected_forward_value<T, E, U>) //
      explicit(!std::is_convertible_v<U&&, T>)              //
      constexpr expected(U&& v)
      : expected(in_place, std::forward<U>(v))
  {
  }
  /// @}

  /**
   * @name assignment
   */
  /// @{
  template <detail::NotVoid U = T, class G = T>
  auto operator=(U&& v) noexcept -> expected& //
      requires(
          !std::is_same_v<expected<T, E>, std::decay_t<U>> &&
          !std::conjunction_v<
              std::is_scalar<T>, std::is_same<T, std::decay_t<U>>> &&
          std::is_constructible_v<T, U> && std::is_assignable_v<G&, U> &&
          std::is_nothrow_move_constructible_v<E>) //
  {
    if (has_value()) {
      val() = std::forward<U>(v);
    } else {
      if constexpr (std::is_nothrow_constructible_v<T, U&&>) {
        err().~unexpected<E>();
        ::new (valptr()) T(std::forward<U>(v));
        this->m_has_val = true;
      } else {
        auto tmp = std::move(err());
        err().~unexpected<E>();

        try {
          ::new (valptr()) T(std::forward<U>(v));
          this->m_has_val = true;
        } catch (...) {
          err() = std::move(tmp);
          throw;
        }
      }
    }

    return *this;
  }

  template <class G = E>
  auto operator=(const unexpected<G>& rhs) -> expected& //
      requires(
          std::is_nothrow_copy_constructible_v<G>&& std::is_assignable_v<G&, G>)
  {
    if (!has_value()) {
      err() = rhs;
    } else {
      this->destroy_val();
      ::new (errptr()) unexpected<E>(rhs);
      this->m_has_val = false;
    }

    return *this;
  }

  template <class G = E>
  auto operator=(unexpected<G>&& rhs) noexcept -> expected& //
      requires(std::is_nothrow_move_constructible_v<G>&&
                   std::is_move_assignable_v<G>)
  {
    if (!has_value()) {
      err() = std::move(rhs);
    } else {
      this->destroy_val();
      ::new (errptr()) unexpected<E>(std::move(rhs));
      this->m_has_val = false;
    }

    return *this;
  }
  /// @}

  /**
   * @name emplace
   */
  /// @{
  template <class... Args> void emplace(Args&&... args)
  {
    if (has_value()) {
      val() = T(std::forward<Args>(args)...);
    } else {
      if constexpr (std::is_nothrow_constructible_v<T, Args&&...>) {
        err().~unexpected<E>();
        ::new (valptr()) T(std::forward<Args>(args)...);
        this->m_has_val = true;
      } else {
        auto tmp = std::move(err());
        err().~unexpected<E>();

        try {
          ::new (valptr()) T(std::forward<Args>(args)...);
          this->m_has_val = true;
        } catch (...) {
          err() = std::move(tmp);
          throw;
        }
      }
    }
  }

  template <class U, class... Args>
  void emplace(std::initializer_list<U> il, Args&&... args)
  {
    if (has_value()) {
      T t(il, std::forward<Args>(args)...);
      val() = std::move(t);
    } else {
      if constexpr (std::is_nothrow_constructible_v<
                        T, std::initializer_list<U>&, Args&&...>) {
        err().~unexpected<E>();
        ::new (valptr()) T(il, std::forward<Args>(args)...);
        this->m_has_val = true;
      } else {
        auto tmp = std::move(err());
        err().~unexpected<E>();

        try {
          ::new (valptr()) T(il, std::forward<Args>(args)...);
          this->m_has_val = true;
        } catch (...) {
          err() = std::move(tmp);
          throw;
        }
      }
    }
  }
  /// @}

private:
  using t_is_void = std::true_type;
  using t_is_not_void = std::false_type;
  using t_is_nothrow_move_constructible = std::true_type;
  using move_constructing_t_can_throw = std::false_type;
  using e_is_nothrow_move_constructible = std::true_type;
  using move_constructing_e_can_throw = std::false_type;

  void swap_where_both_have_value(expected& /*rhs*/, t_is_void) noexcept
  {
    // swapping void is a no-op
  }

  void swap_where_both_have_value(expected& rhs, t_is_not_void)
  {
    using std::swap;
    swap(val(), rhs.val());
  }

  void swap_where_only_one_has_value(expected& rhs, t_is_void) //
      noexcept(std::is_nothrow_move_constructible<E>::value)
  {
    ::new (errptr()) unexpected_type(std::move(rhs.err()));
    rhs.err().~unexpected_type();
    std::swap(this->m_has_val, rhs.m_has_val);
  }

  void swap_where_only_one_has_value(expected& rhs, t_is_not_void)
  {
    swap_where_only_one_has_value_and_t_is_not_void(
        rhs, typename std::is_nothrow_move_constructible<T>::type{},
        typename std::is_nothrow_move_constructible<E>::type{});
  }

  void swap_where_only_one_has_value_and_t_is_not_void(
      expected& rhs, t_is_nothrow_move_constructible,
      e_is_nothrow_move_constructible) noexcept
  {
    auto temp = std::move(val());
    val().~T();
    ::new (errptr()) unexpected_type(std::move(rhs.err()));
    rhs.err().~unexpected_type();
    ::new (rhs.valptr()) T(std::move(temp));
    std::swap(this->m_has_val, rhs.m_has_val);
  }

  void swap_where_only_one_has_value_and_t_is_not_void(
      expected& rhs, t_is_nothrow_move_constructible,
      move_constructing_e_can_throw)
  {
    auto temp = std::move(val());
    val().~T();
    try {
      ::new (errptr()) unexpected_type(std::move(rhs.err()));
      rhs.err().~unexpected_type();
      ::new (rhs.valptr()) T(std::move(temp));
      std::swap(this->m_has_val, rhs.m_has_val);
    } catch (...) {
      val() = std::move(temp);
      throw;
    }
  }

  void swap_where_only_one_has_value_and_t_is_not_void(
      expected& rhs, move_constructing_t_can_throw,
      t_is_nothrow_move_constructible)
  {
    auto temp = std::move(rhs.err());
    rhs.err().~unexpected_type();
    try {
      ::new (rhs.valptr()) T(val());
      val().~T();
      ::new (errptr()) unexpected_type(std::move(temp));
      std::swap(this->m_has_val, rhs.m_has_val);
    } catch (...) {
      rhs.err() = std::move(temp);
      throw;
    }
  }

public:
  template <std::swappable OT = T, std::swappable OE = E>
  constexpr void swap(expected& rhs) //
      noexcept(std::is_nothrow_move_constructible_v<T>&&
                   std::is_nothrow_swappable_v<T>&&
                       std::is_nothrow_move_constructible_v<E>&&
                           std::is_nothrow_swappable_v<E>) //
      requires(
          std::is_nothrow_move_constructible_v<OT> ||
          std::is_nothrow_move_constructible_v<OE>)
  {
    if (has_value() && rhs.has_value()) {
      swap_where_both_have_value(rhs, typename std::is_void<T>::type{});
    } else if (!has_value() && rhs.has_value()) {
      rhs.swap(*this);
    } else if (has_value()) {
      swap_where_only_one_has_value(rhs, typename std::is_void<T>::type{});
    } else {
      using std::swap;
      swap(err(), rhs.err());
    }
  }

  /**
   * @name accessors
   * @brief Accessing the value of an expected if it has one
   * @warning If the expected does not have a value, the result is undefined
   */
  /// @{
  constexpr auto operator->() const -> const T*
  {
    return valptr();
  }
  constexpr auto operator->() -> T*
  {
    return valptr();
  }

  template <detail::NotVoid U = T> constexpr auto operator*() const& -> const U&
  {
    return val();
  }
  template <detail::NotVoid U = T> constexpr auto operator*() & -> U&
  {
    return val();
  }
  template <detail::NotVoid U = T>
  constexpr auto operator*() const&& -> const U&&
  {
    return std::move(val());
  }
  template <detail::NotVoid U = T> constexpr auto operator*() && -> U&&
  {
    return std::move(val());
  }
  /// @}

  /// @brief Returns true if contain a value
  using detail::expected_storage_base<T, E>::has_value;

  /// @brief Returns true if contain a value
  constexpr explicit operator bool() const noexcept
  {
    return this->m_has_val;
  }

  /**
   * @name value
   * @brief Gets the value of an expected if it has one, `panic()`
   * otherwise
   */
  /// @{
  template <detail::NotVoid U = T>
  [[nodiscard]] constexpr auto value() const& -> const U&
  {
    if (!has_value())
      beyond::panic("Call value() of an expected without a value");
    return val();
  }
  template <detail::NotVoid U = T> [[nodiscard]] constexpr auto value() & -> U&
  {
    if (!has_value())
      beyond::panic("Call value() of an expected without a value");
    return val();
  }
  template <detail::NotVoid U = T>
  [[nodiscard]] constexpr auto value() const&& -> const U&&
  {
    if (!has_value())
      beyond::panic("Call value() of an expected without a value");
    return std::move(val());
  }
  template <detail::NotVoid U = T>
  [[nodiscard]] constexpr auto value() && -> U&&
  {
    if (!has_value())
      beyond::panic("Call value() of an expected without a value");
    return std::move(val());
  }
  /// @}

  /**
   * @name value
   * @brief Gets the value of an expected if it has one
   *
   * `panic()` with the `error_msg` otherwise
   */
  /// @{
  template <detail::NotVoid U = T>
  [[nodiscard]] constexpr auto
  expect(std::string_view error_msg) const& -> const U&
  {
    if (!has_value()) beyond::panic(error_msg);
    return val();
  }
  template <detail::NotVoid U = T>
  [[nodiscard]] constexpr auto expect(std::string_view error_msg) & -> U&
  {
    if (!has_value()) beyond::panic(error_msg);
    return val();
  }
  template <detail::NotVoid U = T>
  [[nodiscard]] constexpr auto
  expect(std::string_view error_msg) const&& -> const U&&
  {
    if (!has_value()) beyond::panic(error_msg);
    return std::move(val());
  }
  template <detail::NotVoid U = T>
  [[nodiscard]] constexpr auto expect(std::string_view error_msg) && -> U&&
  {
    if (!has_value()) beyond::panic(error_msg);
    return std::move(val());
  }
  /// @}

  /**
   * @name error
   * @brief Gets the error of an expected if it has one, `panic` otherwise
   */
  /// @{
  [[nodiscard]] constexpr auto error() const& -> const E&
  {
    if (has_value())
      beyond::panic("Call error() on an expected without an error");

    return err().value();
  }
  [[nodiscard]] constexpr auto error() & -> E&
  {
    if (has_value())
      beyond::panic("Call error() on an expected without an error");

    return err().value();
  }
  [[nodiscard]] constexpr auto error() const&& -> const E&&
  {
    if (has_value())
      beyond::panic("Call error() on an expected without an error");

    return std::move(err().value());
  }
  [[nodiscard]] constexpr auto error() && -> E&&
  {
    if (has_value())
      beyond::panic("Call error() on an expected without an error");

    return std::move(err().value());
  }
  /// @}

  /**
   * @name value_or
   * @brief Returns the stored value if there is one, otherwise returns `v`
   */
  /// @{
  template <class U> [[nodiscard]] constexpr auto value_or(U&& v) const& -> T
  {
    static_assert(
        std::is_copy_constructible<T>::value &&
            std::is_convertible<U&&, T>::value,
        "T must be copy-constructible and convertible to from U&&");
    return bool(*this) ? **this : static_cast<T>(std::forward<U>(v));
  }
  template <class U> [[nodiscard]] constexpr auto value_or(U&& v) && -> T
  {
    static_assert(
        std::is_move_constructible<T>::value &&
            std::is_convertible<U&&, T>::value,
        "T must be move-constructible and convertible to from U&&");
    return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(v));
  }
  /// @}
};

namespace detail {
template <class Exp> using exp_t = typename std::decay_t<Exp>::value_type;
template <class Exp> using err_t = typename std::decay_t<Exp>::error_type;
template <class Exp, class Ret> using ret_t = expected<Ret, err_t<Exp>>;

template <class Exp, class F> constexpr auto and_then_impl(Exp&& exp, F&& f)
{
  if constexpr (!std::is_void_v<exp_t<Exp>>) {
    using Ret = decltype(std::invoke(std::declval<F>(), *std::declval<Exp>()));
    static_assert(detail::is_expected<Ret>::value, "F must return an expected");
    return exp.has_value()
               ? std::invoke(std::forward<F>(f), *std::forward<Exp>(exp))
               : Ret(unexpect, std::forward<Exp>(exp).error());
  } else {
    using Ret = decltype(std::invoke(std::declval<F>()));
    static_assert(detail::is_expected<Ret>::value, "F must return an expected");
    return exp.has_value() ? std::invoke(std::forward<F>(f))
                           : Ret(unexpect, std::forward<Exp>(exp).error());
  }
}

template <class Exp, class F> constexpr auto expected_map_impl(Exp&& exp, F&& f)
{
  if constexpr (!std::is_void_v<exp_t<Exp>>) {
    using Ret = decltype(std::invoke(std::declval<F>(), *std::declval<Exp>()));

    if constexpr (!std::is_void_v<Ret>) {
      using result = ret_t<Exp, std::decay_t<Ret>>;
      return exp.has_value() ? result(std::invoke(
                                   std::forward<F>(f), *std::forward<Exp>(exp)))
                             : result(unexpect, std::forward<Exp>(exp).error());
    } else {
      using result = expected<void, err_t<Exp>>;
      if (exp.has_value()) {
        std::invoke(std::forward<F>(f), *std::forward<Exp>(exp));
        return result();
      }

      return result(unexpect, std::forward<Exp>(exp).error());
    }
  } else {
    using Ret = decltype(std::invoke(std::declval<F>()));
    if constexpr (!std::is_void_v<Ret>) {
      using result = ret_t<Exp, std::decay_t<Ret>>;
      return exp.has_value() ? result(std::invoke(std::forward<F>(f)))
                             : result(unexpect, std::forward<Exp>(exp).error());
    } else {
      using result = expected<void, err_t<Exp>>;
      if (exp.has_value()) {
        std::invoke(std::forward<F>(f));
        return result();
      }

      return result(unexpect, std::forward<Exp>(exp).error());
    }
  }
}

template <class Exp, class F> constexpr auto map_error_impl(Exp&& exp, F&& f)
{
  using Ret =
      decltype(std::invoke(std::declval<F>(), std::declval<Exp>().error()));

  if constexpr (!std::is_void_v<exp_t<Exp>>) {
    if constexpr (std::is_void_v<Ret>) {
      using result = expected<exp_t<Exp>, monostate>;
      if (exp.has_value()) { return result(*std::forward<Exp>(exp)); }

      std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
      return result(unexpect, monostate{});
    } else {
      using result = expected<exp_t<Exp>, std::decay_t<Ret>>;
      return exp.has_value()
                 ? result(*std::forward<Exp>(exp))
                 : result(
                       unexpect,
                       std::invoke(
                           std::forward<F>(f), std::forward<Exp>(exp).error()));
    }
  } else {
    if constexpr (std::is_void_v<Ret>) {
      using result = expected<exp_t<Exp>, monostate>;
      if (exp.has_value()) { return result(); }

      std::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
      return result(unexpect, monostate{});
    } else {
      using result = expected<exp_t<Exp>, std::decay_t<Ret>>;
      return exp.has_value()
                 ? result()
                 : result(
                       unexpect,
                       std::invoke(
                           std::forward<F>(f), std::forward<Exp>(exp).error()));
    }
  }
}

template <class Exp, class F> constexpr auto or_else_impl(Exp&& exp, F&& f)
{
  using Ret =
      decltype(std::invoke(std::declval<F>(), std::declval<Exp>().error()));
  if constexpr (std::is_void_v<Ret>) {
    return exp.has_value()
               ? std::forward<Exp>(exp)
               : (std::invoke(
                      std::forward<F>(f), std::forward<Exp>(exp).error()),
                  std::forward<Exp>(exp));
  } else {
    static_assert(detail::is_expected<Ret>::value, "F must return an expected");
    return exp.has_value()
               ? std::forward<Exp>(exp)
               : std::invoke(
                     std::forward<F>(f), std::forward<Exp>(exp).error());
  }
}

} // namespace detail

template <
    class T, class E, std::equality_comparable_with<T> U,
    std::equality_comparable_with<E> F>
[[nodiscard]] constexpr auto
operator==(const expected<T, E>& lhs, const expected<U, F>& rhs) -> bool
{
  return !(lhs.has_value() != rhs.has_value()) &&
         (!lhs.has_value() ? lhs.error() == rhs.error() : *lhs == *rhs);
}
template <class T, class E, std::equality_comparable_with<T> U>
[[nodiscard]] constexpr auto operator==(const expected<T, E>& x, const U& v)
    -> bool
{
  return x.has_value() && *x == v;
}

template <class T, class E>
[[nodiscard]] constexpr auto
operator==(const expected<T, E>& x, const unexpected<E>& e) -> bool
{
  return !x.has_value() && x.error() == e.value();
}

template <class T, class E>
constexpr void
swap(expected<T, E>& lhs, expected<T, E>& rhs) noexcept(noexcept(lhs.swap(
    rhs))) requires((std::is_void_v<T> || std::is_move_constructible_v<T>)&&std::
                        is_swappable_v<T>&& std::is_move_constructible_v<E>&&
                            std::is_swappable_v<E>)
{
  lhs.swap(rhs);
}

/** @}
 * @} */

} // namespace beyond

/**
 * @brief A helper macro to make a common use case of expected more convinient
 * @remark You should prefer monadic operations of expected over this macro
 *
 * Evaluates the expression. If it evaluates to an unexpected error, return.
 * Otherwise assign the value of the result into a variable `name` with type
 * `type`.
 */
#define BEYOND_EXPECTED_LET(type, name, expr)                                  \
  auto name##_ret = (expr);                                                    \
  if (!name##_ret.has_value()) {                                               \
    return beyond::make_unexpected(name##_ret.error());                        \
  }                                                                            \
  type name = std::forward<decltype(name##_ret)>(name##_ret).value();

#endif // BEYOND_CORE_TYPES_EXPECTED_HPP
