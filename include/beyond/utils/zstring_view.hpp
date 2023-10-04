#ifndef BEYOND_CORE_ZSTRING_VIEW_HPP
#define BEYOND_CORE_ZSTRING_VIEW_HPP

#include "utils.hpp"

#include <cstdlib>
#include <cstring>
#include <string_view>

#include <fmt/core.h>

namespace beyond {

/// @brief A guarantee null-terminated string view
///
/// This is useful to interop with C librar
///
///
/// ies
struct ZStringView {
  using size_type = std::size_t;

private:
  size_type size_ = 0;
  const char* data_ = "";

public:
  constexpr ZStringView() noexcept = default;

  /// @brief Constructs a ZStringView from a string literal
  template <size_type N>
  constexpr explicit(false) ZStringView( // NOLINT(hicpp-explicit-conversions)
      const char (&str)[N]) noexcept
      : size_{N - 1}, data_{str}
  {
  }

  /// @brief Constructs a ZStringView from a c string
  constexpr explicit ZStringView(const char* str) noexcept : data_{str}
  {
    if (std::is_constant_evaluated()) {
      size_t i = 0;
      for (; str[i] != '\0'; ++i) {}
      size_ = i;
    } else {
      size_ = std::strlen(str);
    }
  }

  // NOLINTNEXTLINE(hicpp-explicit-conversions)
  constexpr explicit(false) ZStringView(const std::string& str) noexcept
      : size_{str.size()}, data_{str.data()}
  {
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto size() const noexcept
      -> std::size_t
  {
    return size_;
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto empty() const noexcept
      -> bool
  {
    return size_ == 0;
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto data() const noexcept
      -> const char*
  {
    return data_;
  }

  [[nodiscard]] BEYOND_FORCE_INLINE constexpr auto c_str() const noexcept
      -> const char*
  {
    return data_;
  }

  [[nodiscard]] BEYOND_FORCE_INLINE explicit(false) constexpr
  operator std::string_view() // NOLINT(hicpp-explicit-conversions)
  {
    // NOLINTNEXTLINE(modernize-return-braced-init-list)
    return std::string_view(data_, size_);
  };

  [[nodiscard]] BEYOND_FORCE_INLINE friend constexpr auto
  operator==(ZStringView lhs, ZStringView rhs) -> bool
  {
    if (lhs.size_ != rhs.size_) { return false; }
    for (size_t i = 0; i < lhs.size_; ++i) {
      if (lhs.data_[i] != rhs.data_[i]) { return false; }
    }
    return true;
  }
};

} // namespace beyond

template <>
struct fmt::formatter<beyond::ZStringView> : formatter<string_view> {
  auto format(beyond::ZStringView sv, format_context& ctx) const
  {
    return formatter<string_view>::format(std::string_view{sv}, ctx);
  }
};

#endif // BEYOND_CORE_ZSTRING_VIEW_HPP
