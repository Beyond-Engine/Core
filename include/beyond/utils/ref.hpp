#ifndef BEYOND_CORE_UTILS_REF_HPP
#define BEYOND_CORE_UTILS_REF_HPP

#include <memory>

namespace beyond {

/// @brief A lightweight alternative to std::reference_wrapper
template <class T> class Ref {
  T* data_;

public:
  explicit constexpr Ref(T& ref) noexcept : data_{std::addressof(ref)} {}

  // access
  constexpr operator T&() const noexcept
  {
    return *data_;
  }
  [[nodiscard]] constexpr auto get() const noexcept -> T&
  {
    return *data_;
  }
};

template <typename T> Ref(T&) -> Ref<T>;

template <class T> [[nodiscard]] constexpr auto ref(T& t) noexcept -> Ref<T>
{
  return Ref{t};
}
template <class T>
[[nodiscard]] constexpr auto ref(Ref<T>& r) noexcept -> Ref<T>
{
  return Ref{r};
}
template <class T> void ref(T&& t) noexcept = delete;

template <class T>
[[nodiscard]] constexpr auto cref(T& t) noexcept -> Ref<const T>
{
  return Ref<const T>{t};
}
template <class T>
[[nodiscard]] constexpr auto cref(Ref<T>& r) noexcept -> Ref<const T>
{
  return Ref<const T>{r.get()};
}
template <class T> void cref(T&& t) noexcept = delete;

} // namespace beyond

#endif // BEYOND_CORE_UTILS_REF_HPP
