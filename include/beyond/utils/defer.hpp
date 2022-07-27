#ifndef BEYOND_CORE_UTILS_DEFER_HPP
#define BEYOND_CORE_UTILS_DEFER_HPP

namespace beyond {

template <typename Func> class Defer {
  Func func_;

public:
  explicit Defer(Func func) : func_{std::move(func)} {}
  ~Defer()
  {
    func_();
  }
  Defer(const Defer&) = delete;
  auto operator=(const Defer&) -> Defer& = delete;
};

} // namespace beyond

#define BEYOND_CONCAT_IMPL(x, y) x##y
#define BEYOND_CONCAT(x, y) BEYOND_CONCAT_IMPL(x, y)

/**
 * @brief A go-like "defer" utility
 * @note In most cases a "proper" RAII type is probably a better option
 *
 * This macro automatically runs expr when exiting the current scope
 *
 * @example
 * ```cpp
 * {
 * auto buffer = create_buffer():
 * BEYOND_DEFER(destroy_buffer(buffer));
 *
 * do things with the buffer
 * }
 * ```
 */
#define BEYOND_DEFER(expr)                                                     \
  beyond::Defer BEYOND_CONCAT(defer_, __COUNTER__)                             \
  {                                                                            \
    [&]() { expr; }                                                            \
  }

#endif // BEYOND_CORE_UTILS_DEFER_HPP