#pragma once

#ifndef BEYOND_UNIQUE_FUNCTION_HPP
#define BEYOND_UNIQUE_FUNCTION_HPP

#include <functional>
#include <type_traits>


namespace beyond {

template <typename R, typename... Args> class unique_function;

namespace detail {

template <typename R, typename... Args> struct unique_function_base;

enum class unique_function_behaviors { move_to, destory };

union unique_function_storage {
  alignas(void*) std::byte small_[32];
  void* large_;

  template <class T>
  static constexpr bool fit_small = sizeof(T) <= sizeof(small_) &&
                                    alignof(T) <= alignof(decltype(small_)) &&
                                    std::is_nothrow_move_constructible_v<T>;

  unique_function_storage() noexcept = default;

  template <typename Func, typename... Data>
  auto emplace(Data&&... args) -> void
  {
    if constexpr (fit_small<Func>) {
      ::new (static_cast<void*>(&small_)) Func(std::forward<Data>(args)...);
    } else {
      large_ = new Func(std::forward<Data>(args)...);
    }
  }

  template <typename R, typename... Args> struct behaviors {
    template <typename Func>
    static R invoke(const unique_function_base<R, Args...>& who, Args&&... args)
    {
      constexpr static bool fit_sm = fit_small<Func>;
      void* data = const_cast<void*>(fit_sm ? &who.storage_.small_
                                            : who.storage_.large_);
      return (*static_cast<Func*>(data))(std::forward<Args>(args)...);
    }

    template <typename Func>
    static auto dispatch(unique_function_behaviors behavior,
                         unique_function_base<R, Args...>& who, void* ret)
        -> void
    {
      constexpr static bool fit_sm = fit_small<Func>;
      void* data = fit_sm ? &who.storage_.small_ : who.storage_.large_;

      switch (behavior) {
      case detail::unique_function_behaviors::destory:
        if constexpr (fit_sm) {
          static_cast<Func*>(data)->~Func();
        } else {
          delete static_cast<Func*>(who.storage_.large_);
        }
        break;
      case detail::unique_function_behaviors::move_to: {
        auto* func_ptr = static_cast<unique_function_base<R, Args...>*>(ret);
        func_ptr->reset();
        func_ptr->storage_.template emplace<Func>(
            std::move(*static_cast<Func*>(data)));
        func_ptr->behaviors_ = behaviors<R, Args...>::template dispatch<Func>;
        func_ptr->function_ptr_ = behaviors<R, Args...>::template invoke<Func>;
        who.reset();
      } break;
      }
    }
  };
};

template <typename R, typename... Args> struct unique_function_base {
public:
  using result_type = R;

  unique_function_base() = default;
  ~unique_function_base()
  {
    this->reset();
  }

  template <
      typename Func, class DFunc = std::decay_t<Func>,
      class = std::enable_if_t<!std::is_same_v<DFunc, unique_function_base> &&
                               std::is_move_constructible_v<DFunc>>>
  explicit unique_function_base(Func&& func)
  {
    static_assert(std::is_invocable_r_v<R, DFunc, Args...>);

    storage_.emplace<DFunc>(std::forward<DFunc>(func));
    behaviors_ = detail::unique_function_storage::behaviors<
        R, Args...>::template dispatch<DFunc>;
    function_ptr_ = detail::unique_function_storage::behaviors<
        R, Args...>::template invoke<DFunc>;
  }

  unique_function_base(const unique_function_base&) = delete;
  auto
  operator=(const unique_function_base&) & -> unique_function_base& = delete;

  unique_function_base(unique_function_base&& other) noexcept
  {
    if (other) {
      other.behaviors_(detail::unique_function_behaviors::move_to, other, this);
    }
  }

  auto operator=(unique_function_base&& other) & noexcept
                                                 -> unique_function_base&
  {
    if (other) {
      other.behaviors_(detail::unique_function_behaviors::move_to, other, this);
    } else {
      this->reset();
    }
    return *this;
  }

  [[nodiscard]] operator bool() const noexcept // NOLINT
  {
    return behaviors_ != nullptr;
  }

  auto swap(unique_function_base& other) noexcept -> void
  {
    unique_function_base temp = std::move(other);
    other = std::move(*this);
    *this = std::move(temp);
  }

protected:
  auto invoke(Args... args) const -> R
  {
#ifndef BEYOND_FUNCTIONS_NO_EXCEPTION
    if (*this) {
      return this->function_ptr_(*this, std::forward<Args>(args)...);
    } else {
      throw std::bad_function_call{};
    }
#else
    return this->function_ptr_(*this, std::forward<Args>(args)...);
#endif
  }

private:
  friend union detail::unique_function_storage;

  void (*behaviors_)(detail::unique_function_behaviors, unique_function_base&,
                     void*) = nullptr;
  R (*function_ptr_)(const unique_function_base&, Args&&...) = nullptr;
  detail::unique_function_storage storage_;

  void reset()
  {
    if (behaviors_) {
      behaviors_(detail::unique_function_behaviors::destory, *this, nullptr);
    }
    function_ptr_ = nullptr;
    behaviors_ = nullptr;
  }
};

} // namespace detail

template <typename R, typename... Args>
class unique_function<R(Args...)>
    : public detail::unique_function_base<R, Args...> {
  using base_type = detail::unique_function_base<R, Args...>;

public:
  unique_function() = default;

  template <typename Func, class DFunc = std::decay_t<Func>,
            class = std::enable_if_t<!std::is_same_v<DFunc, unique_function> &&
                                     std::is_move_constructible_v<DFunc>>>
  explicit unique_function(Func&& func) : base_type{std::forward<Func>(func)}
  {
  }

  unique_function(unique_function<R(Args...) const>&& other)
      : base_type{static_cast<base_type&&>(other)}
  {
  }

  auto operator()(Args... args) -> R
  {
    return this->invoke(std::forward<Args>(args)...);
  }
};

template <typename R, typename... Args>
class unique_function<R(Args...) const>
    : public detail::unique_function_base<R, Args...> {
public:
  unique_function() = default;

  template <typename Func, class DFunc = std::decay_t<Func>,
            class = std::enable_if_t<!std::is_same_v<DFunc, unique_function> &&
                                     std::is_move_constructible_v<DFunc>>,
            class = std::void_t<
                decltype(std::declval<const Func&>()(std::declval<Args>()...))>>
  explicit unique_function(Func&& func)
      : detail::unique_function_base<R, Args...>{std::forward<Func>(func)}
  {
  }

  auto operator()(Args... args) const -> R
  {
    return this->invoke(std::forward<Args>(args)...);
  }
};

template <class Func>
auto swap(unique_function<Func>& lhs, unique_function<Func>& rhs) noexcept
    -> void
{
  lhs.swap(rhs);
}

template <class Func>
auto operator==(const unique_function<Func>& lhs, std::nullptr_t) noexcept
    -> bool
{
  return !lhs;
}

template <class Func>
auto operator==(std::nullptr_t, const unique_function<Func>& lhs) noexcept
    -> bool
{
  return !lhs;
}

template <class Func>
auto operator!=(const unique_function<Func>& lhs, std::nullptr_t) noexcept
    -> bool
{
  return lhs;
}

template <class Func>
auto operator!=(std::nullptr_t, const unique_function<Func>& lhs) noexcept
    -> bool
{
  return lhs;
}

// deduction guides
template <class R, typename... Args>
unique_function(R (*)(Args...))->unique_function<R(Args...) const>;

namespace detail {

// TODO: Support member functions that take this by reference
template <typename T> struct member_function_pointer_trait {
};

#define BEYOND_MEMBER_FUNCTION_POINTER_TRAIT(CV_OPT, NOEXCEPT_OPT)             \
  template <typename R, typename U, typename... Args>                          \
  struct member_function_pointer_trait<R (U::*)(Args...)                       \
                                           CV_OPT NOEXCEPT_OPT> {              \
    using return_type = R;                                                     \
    using guide_type = R(Args...);                                             \
  };

#define BEYOND_NOARG
BEYOND_MEMBER_FUNCTION_POINTER_TRAIT(BEYOND_NOARG, BEYOND_NOARG)
BEYOND_MEMBER_FUNCTION_POINTER_TRAIT(const, BEYOND_NOARG)
BEYOND_MEMBER_FUNCTION_POINTER_TRAIT(const volatile, BEYOND_NOARG)
BEYOND_MEMBER_FUNCTION_POINTER_TRAIT(volatile, BEYOND_NOARG)

BEYOND_MEMBER_FUNCTION_POINTER_TRAIT(BEYOND_NOARG, noexcept)
BEYOND_MEMBER_FUNCTION_POINTER_TRAIT(const, noexcept)
BEYOND_MEMBER_FUNCTION_POINTER_TRAIT(const volatile, noexcept)
BEYOND_MEMBER_FUNCTION_POINTER_TRAIT(volatile, noexcept)

#undef BEYOND_NOARG
#undef BEYOND_MEMBER_FUNCTION_POINTER_TRAIT

// Main template: cannot find &Func::operator()
template <typename Func, typename = void>
struct function_deduce_signature_impl {
};

template <typename Func>
struct function_deduce_signature_impl<
    Func, std::void_t<decltype(&Func::operator())>> {
  using type = member_function_pointer_trait<decltype(&Func::operator())>;
};

template <typename Func>
struct function_deduce_signature
    : function_deduce_signature_impl<std::remove_cv_t<Func>> {
};

} // namespace detail

template <class Func, class = std::enable_if_t<!std::is_pointer_v<Func>>>
unique_function(Func)
    ->unique_function<
        typename detail::function_deduce_signature<Func>::type::guide_type>;

} // namespace beyond

#endif // BEYOND_UNIQUE_FUNCTION_HPP
