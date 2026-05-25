module;

#include <beyond/prelude.hpp>
#include <memory>

export module beyond.core:ref;

export namespace beyond {

/// @brief A lightweight alternative to std::reference_wrapper
template <class T> class Ref {
  T* data_;

public:
  BEYOND_FORCE_INLINE
  explicit constexpr Ref(T& ref) noexcept : data_{std::addressof(ref)} {}

  // access
  BEYOND_FORCE_INLINE constexpr operator T&() const noexcept { return *data_; }
  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto get() const noexcept -> T&
  { return *data_; }

  [[nodiscard]] BEYOND_FORCE_INLINE auto operator->() const noexcept -> T*
  { return data_; }
};

template <typename T> Ref(T&) -> Ref<T>;

template <class T>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto ref(T& t) noexcept -> Ref<T>
{ return Ref{t}; }
template <class T>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto ref(Ref<T>& r) noexcept
    -> Ref<T>
{ return Ref{r}; }
template <class T> BEYOND_FORCE_INLINE void ref(T&& t) noexcept = delete;

template <class T>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto cref(T& t) noexcept
    -> Ref<const T>
{ return Ref<const T>{t}; }
template <class T>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto cref(Ref<T>& r) noexcept
    -> Ref<const T>
{ return Ref<const T>{r.get()}; }
template <class T> BEYOND_FORCE_INLINE void cref(T&& t) noexcept = delete;

} // namespace beyond
