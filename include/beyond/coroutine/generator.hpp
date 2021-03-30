#ifndef BEYOND_CORE_GENERATOR_HPP
#define BEYOND_CORE_GENERATOR_HPP

#include <concepts>
#include <coroutine>
#include <exception>
#include <memory>

namespace beyond {

template <std::movable T> class Generator {
public:
  struct promise_type {
    using value_type = std::remove_reference_t<T>;
    using reference_type = const value_type&;
    using pointer_type = const value_type*;

    [[nodiscard]] auto get_return_object() -> Generator
    {
      return Generator{Handle::from_promise(*this)};
    }
    static auto initial_suspend() noexcept -> std::suspend_always
    {
      return {};
    }
    static auto final_suspend() noexcept -> std::suspend_always
    {
      return {};
    }
    auto yield_value(reference_type v) noexcept -> std::suspend_always
    {
      value_ = std::addressof(v);
      return {};
    }
    // Disallow co_await in generator coroutines.
    void await_transform() = delete;
    void unhandled_exception()
    {
      exception_ = std::current_exception();
    }

    void rethrow_if_exception()
    {
      if (exception_) { std::rethrow_exception(exception_); }
    }

    std::exception_ptr exception_;
    pointer_type value_ = nullptr;
  };

  using Handle = std::coroutine_handle<promise_type>;

  explicit Generator(const Handle handle) : handle_{handle} {}

  Generator() = default;
  ~Generator()
  {
    if (handle_) { handle_.destroy(); }
  }

  Generator(const Generator&) = delete;
  auto operator=(const Generator&) -> Generator& = delete;

  Generator(Generator&& other) noexcept
      : handle_{std::exchange(other.handle_, {})}
  {
  }
  auto operator=(Generator&& other) & noexcept -> Generator&
  {
    if (this != &other) {
      if (handle_) { handle_.destroy(); }
      handle_ = std::exchange(other.handle_, {});
    }
    return *this;
  }

  // Range-based for loop support.
  class Iter {
    Handle handle_;

  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = typename promise_type::value_type;
    using reference_type = typename promise_type::reference_type;
    using pointer_type = typename promise_type::pointer_type;
    using pointer = pointer_type;
    using reference = reference_type;
    using difference_type = std::ptrdiff_t;

    Iter() = default;
    explicit Iter(const Handle coroutine) : handle_{coroutine} {}
    ~Iter() = default;

    // TODO(lesley@lesleylai.info): Delete copy operations once the lwg defect
    // 3391 is resolved https://cplusplus.github.io/LWG/lwg-defects.html#3391
    Iter(const Iter& other) : handle_{other.handle_} {}
    auto operator=(const Iter& other) & -> Iter&
    {
      handle_ = other.handle_;
    }

    Iter(Iter&& other) noexcept : handle_{std::exchange(other.handle_, {})} {}
    auto operator=(Iter&& other) & noexcept -> Iter&
    {
      if (this != &other) { handle_ = std::exchange(other.handle_, {}); }
      return *this;
    }

    auto operator++() -> Iter&
    {
      handle_.resume();
      if (handle_.done()) { handle_.promise().rethrow_if_exception(); }
      return *this;
    }

    void operator++(int)
    {
      this->operator++();
    }

    [[nodiscard]] auto operator*() const& -> const T&
    {
      return *handle_.promise().value_;
    }

    [[nodiscard]] auto operator==(std::default_sentinel_t) const -> bool
    {
      return !handle_ || handle_.done();
    }
  };

  [[nodiscard]] auto begin() -> Iter
  {
    if (handle_) { handle_.resume(); }
    return Iter{handle_};
  }
  [[nodiscard]] auto end() const noexcept -> std::default_sentinel_t
  {
    return {};
  }

private:
  Handle handle_;
};

} // namespace beyond

template <class T>
inline constexpr bool std::ranges::enable_view<beyond::Generator<T>> = true;

#endif // BEYOND_CORE_GENERATOR_HPP
