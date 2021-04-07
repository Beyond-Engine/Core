/*
 optional.hpp
 An implementation of std::optional with monadic extensions
 Written in 2017 by Simon Brand (simonrbrand@gmail.com, @TartanLlama)
 Forked and modified in 2020 by Lesley Lai
*/

#ifndef BEYOND_CORE_TYPES_OPTIONAL_HPP
#define BEYOND_CORE_TYPES_OPTIONAL_HPP

#include <exception>
#include <functional>
#include <new>
#include <type_traits>
#include <utility>

#include "../utils/panic.hpp"
#include "in_place.hpp"
#include "monostate.hpp"

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup types
 * @{
 */

template <class T> class optional;

namespace detail {
// Trait for checking if a type is a beyond::optional
template <class T> struct is_optional_impl : std::false_type {
};
template <class T> struct is_optional_impl<optional<T>> : std::true_type {
};
template <class T> using is_optional = is_optional_impl<std::decay_t<T>>;
template <class T> constexpr bool is_optional_v = is_optional<T>::value;
template <class T>
concept not_optional = !detail::is_optional_v<std::decay_t<T>>;

// Check if invoking F for some Us returns void
template <class F, class = void, class... U> struct returns_void_impl;
template <class F, class... U>
struct returns_void_impl<F, std::void_t<std::invoke_result_t<F, U...>>, U...>
    : std::is_void<std::invoke_result_t<F, U...>> {
};
template <class F, class... U>
constexpr bool returns_void = returns_void_impl<F&&, void, U...>::value;

template <class T, class U>
concept can_forward_value = std::is_constructible_v<T, U&&> &&
                            !std::is_same_v<std::decay_t<U>, in_place_t> &&
                            !std::is_same_v<optional<T>, std::decay_t<U>>;

template <class T, class U, class Other>
concept can_convert_from_other =
    std::is_constructible_v<T, Other> &&
    !std::is_constructible_v<T, optional<U>&> &&
    !std::is_constructible_v<T, optional<U>&&> &&
    !std::is_constructible_v<T, const optional<U>&> &&
    !std::is_constructible_v<T, const optional<U>&&> &&
    !std::is_convertible_v<optional<U>&, T> &&
    !std::is_convertible_v<optional<U>&&, T> &&
    !std::is_convertible_v<const optional<U>&, T> &&
    !std::is_convertible_v<const optional<U>&&, T>;

template <class T, class U>
concept can_assign_forward =
    !std::is_same_v<optional<T>, std::decay_t<U>> &&
    !std::conjunction_v<std::is_scalar<T>, std::is_same<T, std::decay_t<U>>> &&
    std::is_constructible_v<T, U> && std::is_assignable_v<T&, U>;

template <class T, class U, class Other>
concept can_assign_from_other =
    std::is_constructible_v<T, Other>&& std::is_assignable_v<T&, Other> &&
    !std::is_constructible_v<T, optional<U>&> &&
    !std::is_constructible_v<T, optional<U>&&> &&
    !std::is_constructible_v<T, const optional<U>&> &&
    !std::is_constructible_v<T, const optional<U>&&> &&
    !std::is_convertible_v<optional<U>&, T> &&
    !std::is_convertible_v<optional<U>&&, T> &&
    !std::is_convertible_v<const optional<U>&, T> &&
    !std::is_convertible_v<const optional<U>&&, T> &&
    !std::is_assignable_v<T&, optional<U>&> &&
    !std::is_assignable_v<T&, optional<U>&&> &&
    !std::is_assignable_v<T&, const optional<U>&> &&
    !std::is_assignable_v<T&, const optional<U>&&>;

// This base class provides some handy member functions which can be used in
// further derived classes
template <class T> struct optional_base {
  struct dummy {
  };
  union {
    dummy m_dummy;
    T m_value;
  };

  bool m_has_value;

  // Constructors
  constexpr optional_base() noexcept : m_dummy(), m_has_value(false) {}

  template <class... U>
  constexpr optional_base(in_place_t, U&&... u)
      : m_value(std::forward<U>(u)...), m_has_value(true)
  {
  }

  // Copy Constructor
  constexpr optional_base(const optional_base& rhs)
  {
    if (rhs.has_value()) {
      this->construct(rhs.get());
    } else {
      this->m_has_value = false;
    }
  }
  constexpr optional_base(const optional_base&) //
      requires(std::is_trivially_copy_constructible_v<T>) = default;
  constexpr optional_base(const optional_base&) //
      requires(!std::is_copy_constructible_v<T>) = delete;

  // Move Constructor
  constexpr optional_base(optional_base&& rhs) noexcept
  {
    static_assert(
        std::is_nothrow_move_constructible_v<T>,
        "Throwing move-constructible types are not supported");
    if (rhs.has_value()) {
      this->construct(std::move(rhs.get()));
    } else {
      this->m_has_value = false;
    }
  }
  constexpr optional_base(optional_base&&) noexcept
      requires(std::is_trivially_move_constructible_v<T>) = default;
  constexpr optional_base(optional_base&&) noexcept
      requires(!std::is_move_constructible_v<T>) = delete;

  // Copy Assignment
  constexpr auto operator=(const optional_base& rhs) -> optional_base&
  {
    this->assign(rhs);
    return *this;
  }
  constexpr auto operator=(const optional_base&) -> optional_base& //
      requires(std::is_trivially_copy_assignable_v<T>&&
                   std::is_trivially_copy_constructible_v<T>&&
                       std::is_trivially_destructible_v<T>) = default;
  constexpr auto operator=(const optional_base&) -> optional_base& //
      requires(
          !std::is_copy_assignable_v<T> ||
          !std::is_copy_constructible_v<T>) = delete;

  // Move Assignment
  constexpr auto operator=(optional_base&& rhs) noexcept -> optional_base&
  {
    static_assert(
        std::is_nothrow_move_constructible_v<T> &&
        std::is_nothrow_move_assignable_v<T>);
    this->assign(std::move(rhs));
    return *this;
  }
  constexpr auto operator=(optional_base&&) noexcept -> optional_base& //
      requires(std::is_trivially_destructible_v<T>&&
                   std::is_trivially_move_constructible_v<T>&&
                       std::is_trivially_move_assignable_v<T>) = default;
  constexpr auto operator=(optional_base&&) noexcept -> optional_base& //
      requires(
          !std::is_move_constructible_v<T> &&
          !std::is_move_assignable_v<T>) = delete;

  // Destructor
  constexpr ~optional_base()
  {
    if (m_has_value) {
      m_value.~T();
      m_has_value = false;
    }
  }
  constexpr ~optional_base() noexcept
      requires(::std::is_trivially_destructible_v<T>) = default;

  // Other functions
  constexpr void hard_reset() noexcept
  {
    get().~T();
    this->m_has_value = false;
  }

  template <class... Args> constexpr void construct(Args&&... args) noexcept
  {
    new (std::addressof(this->m_value)) T(std::forward<Args>(args)...);
    this->m_has_value = true;
  }

  template <class Opt> constexpr void assign(Opt&& rhs)
  {
    if (this->has_value()) {
      if (rhs.has_value()) {
        this->m_value = std::forward<Opt>(rhs).get();
      } else {
        this->m_value.~T();
        this->m_has_value = false;
      }
    }

    else if (rhs.has_value()) {
      construct(std::forward<Opt>(rhs).get());
    }
  }

  [[nodiscard]] constexpr auto has_value() const noexcept -> bool
  {
    return this->m_has_value;
  }

  [[nodiscard]] constexpr auto get() & noexcept -> T&
  {
    return this->m_value;
  }
  [[nodiscard]] constexpr auto get() const& noexcept -> const T&
  {
    return this->m_value;
  }
  [[nodiscard]] constexpr auto get() && noexcept -> T&&
  {
    return std::move(this->m_value);
  }
  [[nodiscard]] constexpr auto get() const&& noexcept -> const T&&
  {
    return std::move(this->m_value);
  }
};

} // namespace detail

/// @brief A tag type to represent an empty optional
struct nullopt_t {
  struct do_not_use {
  };
  constexpr explicit nullopt_t(do_not_use, do_not_use) noexcept {}
};
/// @brief Represents an empty optional
static constexpr nullopt_t nullopt{
    nullopt_t::do_not_use{}, nullopt_t::do_not_use{}};

/// @brief An optional implementation with monadic extension
///
/// An optional object is an object that contains the storage for another
/// object and manages the lifetime of this contained object, if any. The
/// contained object may be initialized after the optional object has been
/// initialized, and may be destroyed before the optional object has been
/// destroyed. The initialization state of the contained object is tracked by
/// the optional object.
template <class T> class optional : private detail::optional_base<T> {
  using base = detail::optional_base<T>;

  static_assert(
      !std::is_same<T, in_place_t>::value,
      "instantiation of optional with in_place_t is ill-formed");
  static_assert(
      !std::is_same<std::decay_t<T>, nullopt_t>::value,
      "instantiation of optional with nullopt_t is ill-formed");

public:
  using value_type = T;

  /// @brief Constructs an optional that does not contain a value.
  constexpr optional() noexcept = default;

  constexpr optional(nullopt_t) noexcept {}

  /// @brief Copy constructor
  ///
  /// If `rhs` contains a value, the stored value is direct-initialized with
  /// it. Otherwise, the constructed optional is empty.
  constexpr optional(const optional& rhs) = default;

  /// @brief Move constructor
  ///
  /// If `rhs` contains a value, the stored value is direct-initialized with
  /// it. Otherwise, the constructed optional is empty.
  constexpr optional(optional&& rhs) = default;

  /// @brief Constructs the stored value in-place using the given arguments.
  template <class... Args>
  constexpr explicit optional(in_place_t, Args&&... args) //
      requires(std::is_constructible_v<T, Args...>)
      : base(in_place, std::forward<Args>(args)...)
  {
  }

  /// @overload
  template <class U, class... Args>
  constexpr explicit optional(
      in_place_t, std::initializer_list<U> il, Args&&... args) //
      requires(std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>)
  {
    this->construct(il, std::forward<Args>(args)...);
  }

  /// @brief Constructs the stored value with `u`.
  template <class U = T>
  constexpr explicit(!std::is_convertible_v<U&&, T>) //
      optional(U&& u)                                //
      requires(detail::can_forward_value<T, U>)
      : base(in_place, std::forward<U>(u))
  {
  }

  /// @brief Converting copy constructor.
  template <class U>
  constexpr explicit(!std::is_convertible_v<const U&, T>) //
      optional(const optional<U>& rhs)                    //
      requires(detail::can_convert_from_other<T, U, const U&>)
  {
    if (rhs.has_value()) { this->construct(*rhs); }
  }

  /// @brief Converting move constructor.
  template <class U>
  constexpr explicit(!std::is_convertible_v<U&&, T>) //
      optional(optional<U>&& rhs)                    //
      requires(detail::can_convert_from_other<T, U, U&&>)
  {
    if (rhs.has_value()) { this->construct(std::move(*rhs)); }
  }

  /// @brief Destroys the stored value if there is one.
  constexpr ~optional() = default;

  /// @brief Assignment to empty.
  ///
  /// Destroys the current value if there is one.
  constexpr auto operator=(nullopt_t) noexcept -> optional&
  {
    if (has_value()) {
      this->m_value.~T();
      this->m_has_value = false;
    }

    return *this;
  }

  /// @brief Copy assignment.
  ///
  /// Copies the value from `rhs` if there is one. Otherwise resets the stored
  /// value in `*this`.
  constexpr auto operator=(const optional& rhs) & -> optional& = default;

  /// @brief Move assignment.
  ///
  /// Moves the value from `rhs` if there is one. Otherwise resets the stored
  /// value in `*this`.
  constexpr auto operator=(optional&& rhs) & noexcept -> optional& = default;

  /// @brief Assigns the stored value from `u`, destroying the old value if
  /// there was one.
  template <detail::can_assign_forward<T> U = T>
  constexpr auto operator=(U&& u) -> optional&
  {
    if (has_value()) {
      this->m_value = std::forward<U>(u);
    } else {
      this->construct(std::forward<U>(u));
    }

    return *this;
  }

  /// @brief Converting copy assignment operator.
  ///
  /// Copies the value from `rhs` if there is one. Otherwise resets the stored
  /// value in `*this`.
  template <class U>
  constexpr auto operator=(const optional<U>& rhs) -> optional& //
      requires(detail::can_assign_from_other<T, U, const U&>)
  {
    if (has_value()) {
      if (rhs.has_value()) {
        this->m_value = *rhs;
      } else {
        this->hard_reset();
      }
    }

    if (rhs.has_value()) { this->construct(*rhs); }

    return *this;
  }

  /// @brief Moves the value from `rhs` if there is one. Otherwise resets the
  /// stored value in `*this`.
  template <class U>
  constexpr auto operator=(optional<U>&& rhs) noexcept -> optional& //
      requires(detail::can_assign_from_other<T, U, U>)
  {
    if (has_value()) {
      if (rhs.has_value()) {
        this->m_value = std::move(*rhs);
      } else {
        this->hard_reset();
      }
    }

    if (rhs.has_value()) { this->construct(std::move(*rhs)); }

    return *this;
  }

  /// @brief Carries out some operation which returns an optional on the stored
  /// object if there is one.

  /**
   * @name and_then
   * @brief  Carries out some operation which returns an optional on the
  stored object if there is one.
   * @param f A function takes T and return the a optional<T2>
   *
   */
  /// @{
  template <class F> constexpr auto and_then(F&& f) &
  {
    using result = std::invoke_result_t<F, T&>;
    static_assert(
        detail::is_optional<result>::value, "F must return an optional");

    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : result(nullopt);
  }

  template <class F> constexpr auto and_then(F&& f) &&
  {
    using result = std::invoke_result_t<F, T&&>;
    static_assert(
        detail::is_optional<result>::value, "F must return an optional");

    return has_value() ? std::invoke(std::forward<F>(f), std::move(**this))
                       : result(nullopt);
  }

  template <class F> constexpr auto and_then(F&& f) const&
  {
    using result = std::invoke_result_t<F, const T&>;
    static_assert(
        detail::is_optional<result>::value, "F must return an optional");

    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : result(nullopt);
  }

  template <class F> constexpr auto and_then(F&& f) const&&
  {
    using result = std::invoke_result_t<F, const T&&>;
    static_assert(
        detail::is_optional<result>::value, "F must return an optional");

    return has_value() ? std::invoke(std::forward<F>(f), std::move(**this))
                       : result(nullopt);
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
    return optional_map_impl(*this, std::forward<F>(f));
  }

  template <class F> constexpr auto map(F&& f) &&
  {
    return optional_map_impl(std::move(*this), std::forward<F>(f));
  }

  template <class F> constexpr auto map(F&& f) const&
  {
    return optional_map_impl(*this, std::forward<F>(f));
  }

  template <class F> constexpr auto map(F&& f) const&&
  {
    return optional_map_impl(std::move(*this), std::forward<F>(f));
  }

  template <class F> constexpr auto transform(F&& f) &
  {
    return optional_map_impl(*this, std::forward<F>(f));
  }

  template <class F> constexpr auto transform(F&& f) &&
  {
    return optional_map_impl(std::move(*this), std::forward<F>(f));
  }

  template <class F> constexpr auto transform(F&& f) const&
  {
    return optional_map_impl(*this, std::forward<F>(f));
  }

  template <class F> constexpr auto transform(F&& f) const&&
  {
    return optional_map_impl(std::move(*this), std::forward<F>(f));
  }
  /// @}

  /**
   * @name or_else
   * @brief Calls f if the optional is empty
   * @param f A `void()` function that gets invoked if the optional is empty
   */
  /// @{
  template <class F> constexpr auto or_else(F&& f) & -> optional<T>
  {
    if constexpr (detail::returns_void<F>) {
      if (has_value()) return *this;
      std::forward<F>(f)();
      return nullopt;
    } else {
      return has_value() ? *this : std::forward<F>(f)();
    }
  }

  template <class F> constexpr auto or_else(F&& f) && -> optional<T>
  {
    if constexpr (detail::returns_void<F>) {
      if (has_value()) return std::move(*this);

      std::forward<F>(f)();
      return nullopt;
    } else {
      return has_value() ? std::move(*this) : std::forward<F>(f)();
    }
  }

  template <class F> constexpr auto or_else(F&& f) const& -> optional<T>
  {
    if constexpr (detail::returns_void<F>) {
      if (has_value()) return *this;

      std::forward<F>(f)();
      return nullopt;
    } else {
      return has_value() ? *this : std::forward<F>(f)();
    }
  }

  template <class F> constexpr auto or_else(F&& f) const&& -> optional<T>
  {
    if constexpr (detail::returns_void<F>) {
      if (has_value()) return std::move(*this);

      std::forward<F>(f)();
      return nullopt;
    } else {
      return has_value() ? std::move(*this) : std::forward<F>(f)();
    }
  }
  /// @}

  /**
   * @name map_or
   * @brief Maps the stored value with `f` if there is one, otherwise
   * returns `u`.
   */
  /// @{
  template <class F, class U> constexpr auto map_or(F&& f, U&& u) & -> U
  {
    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : std::forward<U>(u);
  }

  template <class F, class U> constexpr auto map_or(F&& f, U&& u) && -> U
  {
    return has_value() ? std::invoke(std::forward<F>(f), std::move(**this))
                       : std::forward<U>(u);
  }

  template <class F, class U> constexpr auto map_or(F&& f, U&& u) const& -> U
  {
    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : std::forward<U>(u);
  }

  template <class F, class U> constexpr auto map_or(F&& f, U&& u) const&& -> U
  {
    return has_value() ? std::invoke(std::forward<F>(f), std::move(**this))
                       : std::forward<U>(u);
  }
  /// @}

  /**
   * @name map_or_else
   * @brief Maps the stored value with `f` if there is one, otherwise calls
   * `u` and returns the result.
   */
  /// @{
  template <class F, class U>
  constexpr auto map_or_else(F&& f, U&& u) & -> std::invoke_result_t<U>
  {
    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : std::forward<U>(u)();
  }

  template <class F, class U>
  constexpr auto map_or_else(F&& f, U&& u) && -> std::invoke_result_t<U>
  {
    return has_value() ? std::invoke(std::forward<F>(f), std::move(**this))
                       : std::forward<U>(u)();
  }

  template <class F, class U>
  constexpr auto map_or_else(F&& f, U&& u) const& -> std::invoke_result_t<U>
  {
    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : std::forward<U>(u)();
  }

  template <class F, class U>
  constexpr auto map_or_else(F&& f, U&& u) const&& -> std::invoke_result_t<U>
  {
    return has_value() ? std::invoke(std::forward<F>(f), std::move(**this))
                       : std::forward<U>(u)();
  }
  /// @}

  /// @name conjunction
  /// @brief Returns `u` if `*this` has a value, otherwise an empty optional.
  template <class U>
  constexpr auto conjunction(U&& u) const -> optional<std::decay_t<U>>
  {
    using result = optional<std::decay_t<U>>;
    return has_value() ? result{u} : result{nullopt};
  }

  /**
   * @name disjunction
   * @brief Returns `rhs` if `*this` is empty, otherwise the current value.
   *
   */
  /// @{
  constexpr auto disjunction(const optional& rhs) & -> optional
  {
    return has_value() ? *this : rhs;
  }

  constexpr auto disjunction(const optional& rhs) const& -> optional
  {
    return has_value() ? *this : rhs;
  }

  constexpr auto disjunction(const optional& rhs) && -> optional
  {
    return has_value() ? std::move(*this) : rhs;
  }

  constexpr auto disjunction(const optional& rhs) const&& -> optional
  {
    return has_value() ? std::move(*this) : rhs;
  }

  constexpr auto disjunction(optional&& rhs) & -> optional
  {
    return has_value() ? *this : std::move(rhs);
  }

  constexpr auto disjunction(optional&& rhs) const& -> optional
  {
    return has_value() ? *this : std::move(rhs);
  }

  constexpr auto disjunction(optional&& rhs) && -> optional
  {
    return has_value() ? std::move(*this) : std::move(rhs);
  }

  constexpr auto disjunction(optional&& rhs) const&& -> optional
  {
    return has_value() ? std::move(*this) : std::move(rhs);
  }
  /// @}

  /// @name take
  /// @brief Takes the value out of the optional, leaving it empty
  /// @{
  constexpr auto take() -> optional
  {
    optional ret = std::move(*this);
    reset();
    return ret;
  }
  /// @}

  /**
   * @name emplace
   * @brief Constructs the value in-place, destroying the current one if there
   * is one.
   */
  /// @{
  template <class... Args> constexpr auto emplace(Args&&... args) -> T&
  {
    static_assert(
        std::is_constructible<T, Args&&...>::value,
        "T must be constructible with Args");

    *this = nullopt;
    this->construct(std::forward<Args>(args)...);
    return value();
  }
  /// @}

  /// @name swap
  /// @brief Swaps this optional with the other.
  ///
  /// If neither optionals have a value, nothing happens.
  /// If both have a value, the values are swapped.
  /// If one has a value, it is moved to the other and the movee is left
  /// valueless.
  void swap(optional& rhs) noexcept
  {
    static_assert(
        std::is_nothrow_move_constructible_v<T> &&
        std::is_nothrow_swappable_v<T>);

    using std::swap;
    if (has_value()) {
      if (rhs.has_value()) {
        swap(**this, *rhs);
      } else {
        new (std::addressof(rhs.m_value)) T(std::move(this->m_value));
        this->m_value.T::~T();
      }
    } else if (rhs.has_value()) {
      new (std::addressof(this->m_value)) T(std::move(rhs.m_value));
      rhs.m_value.T::~T();
    }
    swap(this->m_has_value, rhs.m_has_value);
  }

  /**
   * @name operator->
   * @brief Returns a pointer to the stored value.
   * @warning The result is undefined if the optional does not store a value.
   */
  /// @{
  [[nodiscard]] constexpr auto operator->() const -> const T*
  {
    return std::addressof(this->m_value);
  }

  [[nodiscard]] constexpr auto operator->() -> T*
  {
    return std::addressof(this->m_value);
  }
  /// @}

  /**
   * @name operator*
   * @brief Returns the stored value
   * @warning The result is undefined if the optional does not store a value.
   */
  /// @{
  [[nodiscard]] constexpr auto operator*() & -> T&
  {
    return this->m_value;
  }

  [[nodiscard]] constexpr auto operator*() const& -> const T&
  {
    return this->m_value;
  }

  [[nodiscard]] constexpr auto operator*() && -> T&&
  {
    return std::move(this->m_value);
  }

  [[nodiscard]] constexpr auto operator*() const&& -> const T&&
  {
    return std::move(this->m_value);
  }
  /// @}

  /// @name has_value
  /// @brief Returns whether or not the optional has a value
  using base::has_value;

  /// @name operator bool()
  /// @brief Returns whether or not the optional has a value
  constexpr explicit operator bool() const noexcept
  {
    return this->m_has_value;
  }

  /// @name value
  /// @brief Returns the contained value if there is one
  /// @warning Calls `panic()` if there the optional does not contain value
  /// @{
  constexpr T& value() &
  {
    if (has_value()) return this->m_value;
    beyond::panic("Try to access an empty optional");
  }

  /// @overload
  constexpr const T& value() const&
  {
    if (has_value()) return this->m_value;
    beyond::panic("Try to access an empty optional");
  }

  /// @overload
  constexpr T&& value() &&
  {
    if (has_value()) return std::move(this->m_value);
    beyond::panic("Try to access an empty optional");
  }

  /// @overload
  constexpr const T&& value() const&&
  {
    if (has_value()) return std::move(this->m_value);
    beyond::panic("Try to access an empty optional");
  }
  /// @}

  /// @name expect
  /// @brief Returns the contained value if there is one
  ///
  /// Panic with error error_msg
  /// @{
  constexpr T& expect(std::string_view error_msg) &
  {
    if (has_value()) return this->m_value;
    beyond::panic(error_msg);
  }

  /// @overload
  constexpr const T& expect(std::string_view error_msg) const&
  {
    if (has_value()) return this->m_value;
    beyond::panic(error_msg);
  }

  /// @overload
  constexpr T&& expect(std::string_view error_msg) &&
  {
    if (has_value()) return std::move(this->m_value);
    beyond::panic(error_msg);
  }

  /// @overload
  constexpr const T&& expect(std::string_view error_msg) const&&
  {
    if (has_value()) return std::move(this->m_value);
    beyond::panic(error_msg);
  }
  /// @}

  /**
   * @name value_or
   * @brief Returns the stored value if there is one, otherwise returns `u`
   */
  /// @{
  template <class U> constexpr T value_or(U&& u) const&
  {
    static_assert(
        std::is_copy_constructible<T>::value &&
            std::is_convertible<U&&, T>::value,
        "T must be copy constructible and convertible from U");
    return has_value() ? **this : static_cast<T>(std::forward<U>(u));
  }

  /// @overload
  template <class U> constexpr T value_or(U&& u) &&
  {
    static_assert(
        std::is_move_constructible<T>::value &&
            std::is_convertible<U&&, T>::value,
        "T must be move constructible and convertible from U");
    return has_value() ? **this : static_cast<T>(std::forward<U>(u));
  }
  /// @}

  /// @brief Destroys the stored value if one exists, making the optional empty
  void reset() noexcept
  {
    if (has_value()) {
      this->m_value.~T();
      this->m_has_value = false;
    }
  }
}; // namespace beyond

/// Compares two optional objects
template <class T, class U>
inline constexpr bool operator==(const optional<T>& lhs, const optional<U>& rhs)
{
  return lhs.has_value() == rhs.has_value() &&
         (!lhs.has_value() || *lhs == *rhs);
}
template <class T, class U>
inline constexpr bool operator<(const optional<T>& lhs, const optional<U>& rhs)
{
  return rhs.has_value() && (!lhs.has_value() || *lhs < *rhs);
}
template <class T, class U>
inline constexpr bool operator>(const optional<T>& lhs, const optional<U>& rhs)
{
  return lhs.has_value() && (!rhs.has_value() || *lhs > *rhs);
}
template <class T, class U>
inline constexpr bool operator<=(const optional<T>& lhs, const optional<U>& rhs)
{
  return !lhs.has_value() || (rhs.has_value() && *lhs <= *rhs);
}
template <class T, class U>
inline constexpr bool operator>=(const optional<T>& lhs, const optional<U>& rhs)
{
  return !rhs.has_value() || (lhs.has_value() && *lhs >= *rhs);
}

/// Compares an optional to a `nullopt`
template <class T>
inline constexpr bool operator==(const optional<T>& lhs, nullopt_t) noexcept
{
  return !lhs.has_value();
}

template <class T>
inline constexpr bool operator<(const optional<T>&, nullopt_t) noexcept
{
  return false;
}
template <class T>
inline constexpr bool operator<(nullopt_t, const optional<T>& rhs) noexcept
{
  return rhs.has_value();
}
template <class T>
inline constexpr bool operator<=(const optional<T>& lhs, nullopt_t) noexcept
{
  return !lhs.has_value();
}
template <class T>
inline constexpr bool operator<=(nullopt_t, const optional<T>&) noexcept
{
  return true;
}
template <class T>
inline constexpr bool operator>(const optional<T>& lhs, nullopt_t) noexcept
{
  return lhs.has_value();
}
template <class T>
inline constexpr bool operator>(nullopt_t, const optional<T>&) noexcept
{
  return false;
}
template <class T>
inline constexpr bool operator>=(const optional<T>&, nullopt_t) noexcept
{
  return true;
}
template <class T>
inline constexpr bool operator>=(nullopt_t, const optional<T>& rhs) noexcept
{
  return !rhs.has_value();
}

/// Compares the optional with a value.
template <class T, class U>
inline constexpr bool operator==(const optional<T>& lhs, const U& rhs)
{
  return lhs.has_value() ? *lhs == rhs : false;
}
template <class T, class U>
inline constexpr bool operator<(const optional<T>& lhs, const U& rhs)
{
  return lhs.has_value() ? *lhs < rhs : true;
}
template <class T, class U>
inline constexpr bool operator<(const U& lhs, const optional<T>& rhs)
{
  return rhs.has_value() ? lhs < *rhs : false;
}
template <class T, class U>
inline constexpr bool operator<=(const optional<T>& lhs, const U& rhs)
{
  return lhs.has_value() ? *lhs <= rhs : true;
}
template <class T, class U>
inline constexpr bool operator<=(const U& lhs, const optional<T>& rhs)
{
  return rhs.has_value() ? lhs <= *rhs : false;
}
template <class T, class U>
inline constexpr bool operator>(const optional<T>& lhs, const U& rhs)
{
  return lhs.has_value() ? *lhs > rhs : false;
}
template <class T, class U>
inline constexpr bool operator>(const U& lhs, const optional<T>& rhs)
{
  return rhs.has_value() ? lhs > *rhs : true;
}
template <class T, class U>
inline constexpr bool operator>=(const optional<T>& lhs, const U& rhs)
{
  return lhs.has_value() ? *lhs >= rhs : false;
}
template <class T, class U>
inline constexpr bool operator>=(const U& lhs, const optional<T>& rhs)
{
  return rhs.has_value() ? lhs >= *rhs : true;
}

template <class T> void swap(optional<T>& lhs, optional<T>& rhs) noexcept
{
  return lhs.swap(rhs);
}

namespace detail {
struct i_am_secret {
};
} // namespace detail

template <
    class T = detail::i_am_secret, class U,
    class Ret = std::conditional_t<
        std::is_same_v<T, detail::i_am_secret>, std::decay_t<U>, T>>
inline constexpr optional<Ret> make_optional(U&& v)
{
  return optional<Ret>(std::forward<U>(v));
}

template <class T, class... Args>
inline constexpr optional<T> make_optional(Args&&... args)
{
  return optional<T>(in_place, std::forward<Args>(args)...);
}
template <class T, class U, class... Args>
inline constexpr optional<T>
make_optional(std::initializer_list<U> il, Args&&... args)
{
  return optional<T>(in_place, il, std::forward<Args>(args)...);
}

template <class T> optional(T) -> optional<T>;

namespace detail {
template <
    class Opt, class F,
    class Ret = decltype(std::invoke(std::declval<F>(), *std::declval<Opt>()))>
constexpr auto optional_map_impl(Opt&& opt, F&& f)
{
  if constexpr (std::is_void_v<Ret>) {
    if (opt.has_value()) {
      std::invoke(std::forward<F>(f), *std::forward<Opt>(opt));
      return make_optional(monostate{});
    }
    return optional<monostate>(nullopt);
  } else {
    return opt.has_value()
               ? std::invoke(std::forward<F>(f), *std::forward<Opt>(opt))
               : optional<Ret>(nullopt);
  }
}

} // namespace detail

// Specialization for when `T` is a reference. `optional<T&>` acts similarly
// to a `T*`, but provides more operations and shows intent more clearly.
template <class T> class optional<T&> {
public:
  /// Carries out some operation which returns an optional on the stored
  /// object if there is one.
  template <class F> constexpr auto and_then(F&& f) &
  {
    using result = std::invoke_result_t<F, T&>;
    static_assert(
        detail::is_optional<result>::value, "F must return an optional");

    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : result(nullopt);
  }

  template <class F> constexpr auto and_then(F&& f) &&
  {
    using result = std::invoke_result_t<F, T&>;
    static_assert(
        detail::is_optional<result>::value, "F must return an optional");

    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : result(nullopt);
  }

  template <class F> constexpr auto and_then(F&& f) const&
  {
    using result = std::invoke_result_t<F, const T&>;
    static_assert(
        detail::is_optional<result>::value, "F must return an optional");

    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : result(nullopt);
  }

  template <class F> constexpr auto and_then(F&& f) const&&
  {
    using result = std::invoke_result_t<F, const T&>;
    static_assert(
        detail::is_optional<result>::value, "F must return an optional");

    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : result(nullopt);
  }

  /// Carries out some operation on the stored object if there is one.
  template <class F> constexpr auto map(F&& f) &
  {
    return detail::optional_map_impl(*this, std::forward<F>(f));
  }

  template <class F> constexpr auto map(F&& f) &&
  {
    return detail::optional_map_impl(std::move(*this), std::forward<F>(f));
  }

  template <class F> constexpr auto map(F&& f) const&
  {
    return detail::optional_map_impl(*this, std::forward<F>(f));
  }

  template <class F> constexpr auto map(F&& f) const&&
  {
    return detail::optional_map_impl(std::move(*this), std::forward<F>(f));
  }

  /// Carries out some operation on the stored object if there is one.
  template <class F> constexpr auto transform(F&& f) &
  {
    return detail::optional_map_impl(*this, std::forward<F>(f));
  }

  template <class F> constexpr auto transform(F&& f) &&
  {
    return detail::optional_map_impl(std::move(*this), std::forward<F>(f));
  }

  template <class F> constexpr auto transform(F&& f) const&
  {
    return detail::optional_map_impl(*this, std::forward<F>(f));
  }

  template <class F> constexpr auto transform(F&& f) const&&
  {
    return detail::optional_map_impl(std::move(*this), std::forward<F>(f));
  }

  /// Calls `f` if the optional is empty
  template <class F> constexpr auto or_else(F&& f) & -> optional<T>
  {
    if constexpr (detail::returns_void<F>) {
      if (has_value()) return *this;

      std::forward<F>(f)();
      return nullopt;
    } else {
      return has_value() ? *this : std::forward<F>(f)();
    }
  }

  template <class F> constexpr auto or_else(F&& f) && -> optional<T>
  {
    if constexpr (detail::returns_void<F>) {
      if (has_value()) return std::move(*this);
      std::forward<F>(f)();
      return nullopt;
    } else {
      return has_value() ? std::move(*this) : std::forward<F>(f)();
    }
  }

  template <class F> constexpr auto or_else(F&& f) const& -> optional<T>
  {
    if constexpr (detail::returns_void<F>) {
      if (has_value()) return *this;
      std::forward<F>(f)();
      return nullopt;
    } else {
      return has_value() ? *this : std::forward<F>(f)();
    }
  }

  template <class F> constexpr auto or_else(F&& f) const&& -> optional<T>
  {
    if constexpr (detail::returns_void<F>) {
      if (has_value()) return std::move(*this);
      std::forward<F>(f)();
      return nullopt;
    } else {
      return has_value() ? std::move(*this) : std::forward<F>(f)();
    }
  }

  /// Maps the stored value with `f` if there is one, otherwise returns `u`
  template <class F, class U> constexpr auto map_or(F&& f, U&& u) & -> U
  {
    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : std::forward<U>(u);
  }

  template <class F, class U> constexpr auto map_or(F&& f, U&& u) && -> U
  {
    return has_value() ? std::invoke(std::forward<F>(f), std::move(**this))
                       : std::forward<U>(u);
  }

  template <class F, class U> constexpr auto map_or(F&& f, U&& u) const& -> U
  {
    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : std::forward<U>(u);
  }

  template <class F, class U> constexpr auto map_or(F&& f, U&& u) const&& -> U
  {
    return has_value() ? std::invoke(std::forward<F>(f), std::move(**this))
                       : std::forward<U>(u);
  }

  /// Maps the stored value with `f` if there is one, otherwise calls
  /// `u` and returns the result.
  template <class F, class U>
  constexpr auto map_or_else(F&& f, U&& u) & -> std::invoke_result_t<U>
  {
    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : std::forward<U>(u)();
  }

  template <class F, class U>
  constexpr auto map_or_else(F&& f, U&& u) && -> std::invoke_result_t<U>
  {
    return has_value() ? std::invoke(std::forward<F>(f), std::move(**this))
                       : std::forward<U>(u)();
  }

  template <class F, class U>
  constexpr auto map_or_else(F&& f, U&& u) const& -> std::invoke_result_t<U>
  {
    return has_value() ? std::invoke(std::forward<F>(f), **this)
                       : std::forward<U>(u)();
  }

  template <class F, class U>
  constexpr auto map_or_else(F&& f, U&& u) const&& -> std::invoke_result_t<U>
  {
    return has_value() ? std::invoke(std::forward<F>(f), std::move(**this))
                       : std::forward<U>(u)();
  }

  /// Returns `u` if `*this` has a value, otherwise an empty optional.
  template <class U>
  constexpr auto conjunction(U&& u) const
      -> optional<typename std::decay<U>::type>
  {
    using result = optional<std::decay_t<U>>;
    return has_value() ? result{u} : result{nullopt};
  }

  /// Returns `rhs` if `*this` is empty, otherwise the current value.
  constexpr auto disjunction(const optional& rhs) & -> optional
  {
    return has_value() ? *this : rhs;
  }

  constexpr auto disjunction(const optional& rhs) const& -> optional
  {
    return has_value() ? *this : rhs;
  }

  constexpr auto disjunction(const optional& rhs) && -> optional
  {
    return has_value() ? std::move(*this) : rhs;
  }

  constexpr auto disjunction(const optional& rhs) const&& -> optional
  {
    return has_value() ? std::move(*this) : rhs;
  }

  constexpr auto disjunction(optional&& rhs) & -> optional
  {
    return has_value() ? *this : std::move(rhs);
  }

  constexpr auto disjunction(optional&& rhs) const& -> optional
  {
    return has_value() ? *this : std::move(rhs);
  }

  constexpr auto disjunction(optional&& rhs) && -> optional
  {
    return has_value() ? std::move(*this) : std::move(rhs);
  }

  constexpr auto disjunction(optional&& rhs) const&& -> optional
  {
    return has_value() ? std::move(*this) : std::move(rhs);
  }

  /// Takes the value out of the optional, leaving it empty
  constexpr auto take() -> optional
  {
    optional ret = std::move(*this);
    reset();
    return ret;
  }

  using value_type = T&;

  /// Constructs an optional that does not contain a value.
  constexpr optional() noexcept : m_value(nullptr) {}

  constexpr optional(nullopt_t) noexcept : m_value(nullptr) {}

  /// Copy constructor
  ///
  /// If `rhs` contains a value, the stored value is direct-initialized with
  /// it. Otherwise, the constructed optional is empty.
  constexpr optional(const optional& rhs) noexcept = default;

  /// Move constructor
  ///
  /// If `rhs` contains a value, the stored value is direct-initialized with
  /// it. Otherwise, the constructed optional is empty.
  constexpr optional(optional&& rhs) = default;

  /// Constructs the stored value with `u`.
  template <class U = T>
  constexpr optional(U&& u) noexcept requires(detail::not_optional<U>)
      : m_value(std::addressof(u))
  {
    static_assert(std::is_lvalue_reference<U>::value, "U must be an lvalue");
  }

  template <class U>
  constexpr explicit optional(const optional<U>& rhs) noexcept : optional(*rhs)
  {
  }

  /// No-op
  ~optional() noexcept = default;

  /// Assignment to empty.
  ///
  /// Destroys the current value if there is one.
  optional& operator=(nullopt_t) noexcept
  {
    m_value = nullptr;
    return *this;
  }

  /// Copy assignment.
  ///
  /// Rebinds this optional to the referee of `rhs` if there is one. Otherwise
  /// resets the stored value in `*this`.
  optional& operator=(const optional& rhs) = default;

  /// Rebinds this optional to `u`.
  template <class U = T>
  optional& operator=(U&& u) requires(detail::not_optional<U>)
  {
    static_assert(std::is_lvalue_reference<U>::value, "U must be an lvalue");
    m_value = std::addressof(u);
    return *this;
  }

  /// Converting copy assignment operator.
  ///
  /// Rebinds this optional to the referee of `rhs` if there is one. Otherwise
  /// resets the stored value in `*this`.
  template <class U> optional& operator=(const optional<U>& rhs) noexcept
  {
    m_value = std::addressof(rhs.value());
    return *this;
  }

  /// Rebinds this optional to `u`.
  template <class U = T>
  optional& emplace(U&& u) noexcept requires(detail::not_optional<U>)
  {
    return *this = std::forward<U>(u);
  }

  void swap(optional& rhs) noexcept
  {
    std::swap(m_value, rhs.m_value);
  }

  /// Returns a pointer to the stored value
  constexpr const T* operator->() const noexcept
  {
    return m_value;
  }

  constexpr T* operator->() noexcept
  {
    return m_value;
  }

  /// Returns the stored value
  constexpr T& operator*() noexcept
  {
    return *m_value;
  }

  constexpr const T& operator*() const noexcept
  {
    return *m_value;
  }

  constexpr bool has_value() const noexcept
  {
    return m_value != nullptr;
  }

  constexpr explicit operator bool() const noexcept
  {
    return m_value != nullptr;
  }

  /// Returns the contained value if there is one, calls panic() otherwise
  constexpr T& value()
  {
    if (has_value()) return *m_value;
    beyond::panic("Try to access an empty optional");
  }
  constexpr const T& value() const
  {
    if (has_value()) return *m_value;
    beyond::panic("Try to access an empty optional");
  }

  constexpr T& expect(std::string_view error_msg)
  {
    if (has_value()) return *m_value;
    beyond::panic(error_msg);
  }
  constexpr const T& expect(std::string_view error_msg) const
  {
    if (has_value()) return *m_value;
    beyond::panic(error_msg);
  }

  /// @brief Returns the stored value if there is one, otherwise returns `u`
  template <class U> constexpr T value_or(U&& u) const& noexcept
  {
    static_assert(
        std::is_copy_constructible<T>::value &&
            std::is_convertible<U&&, T>::value,
        "T must be copy constructible and convertible from U");
    return has_value() ? **this : static_cast<T>(std::forward<U>(u));
  }

  /// @overload
  template <class U> constexpr T value_or(U&& u) && noexcept
  {
    static_assert(
        std::is_move_constructible<T>::value &&
            std::is_convertible<U&&, T>::value,
        "T must be move constructible and convertible from U");
    return has_value() ? **this : static_cast<T>(std::forward<U>(u));
  }

  /// Destroys the stored value if one exists, making the optional empty
  void reset() noexcept
  {
    m_value = nullptr;
  }

private:
  T* m_value = nullptr;
};

/** @}
 * @} */

} // namespace beyond

namespace std {

/**
 * @addtogroup core
 * @{
 * @addtogroup types
 * @{
 */

template <class T> struct hash<beyond::optional<T>> {
  ::std::size_t operator()(const beyond::optional<T>& o) const
  {
    if (!o.has_value()) return 0;

    return std::hash<std::remove_const_t<T>>()(*o);
  }
};

/** @}
 * @} */

} // namespace std

#endif // BEYOND_CORE_TYPES_OPTIONAL_HPP
