#ifndef BEYOND_CORE_TEST_RAII_COUNTER_HPP
#define BEYOND_CORE_TEST_RAII_COUNTER_HPP

struct Counters {
  int constructor = 0;
  int destructor = 0;
  int move = 0;
  int copy = 0;
  int invoke = 0;
};

struct Small {
  Small(Counters& c) noexcept : counters{c}
  {
    ++(counters.constructor);
  }

  ~Small() noexcept
  {
    ++(counters.destructor);
  }

  Small(const Small& other) noexcept : counters{other.counters}
  {
    ++(counters.copy);
  }

  auto operator=(const Small& other) noexcept -> Small&
  {
    counters = other.counters;
    ++(counters.copy);
    return *this;
  }

  Small(Small&& other) noexcept : counters{other.counters}
  {
    ++(counters.move);
  }

  auto operator=(Small&& other) noexcept -> Small&
  {
    counters = other.counters;
    ++(counters.move);
    return *this;
  }

  void operator()() noexcept
  {
    ++(counters.invoke);
  }

  Counters& counters;
};

#endif // BEYOND_CORE_TEST_RAII_COUNTER_HPP
