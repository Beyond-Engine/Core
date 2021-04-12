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

} // namespace beyond

#endif // BEYOND_CORE_UTILS_REF_HPP
