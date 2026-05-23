#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <latch>
#include <numeric>
#include <thread>

import beyond.core;

TEST_CASE("Empty Thread Pool should not stuck",
          "[beyond.core.concurrency.thread_pool]")
{ [[maybe_unused]] beyond::ThreadPool thread_pool{}; }

TEST_CASE("Thread Pool one worker two tasks",
          "[beyond.core.concurrency.thread_pool]")
{
  beyond::ThreadPool thread_pool{beyond::ThreadPoolInfo{.thread_count = 1}};
  REQUIRE(thread_pool.thread_count() == 1);

  int x = 0;
  int y = 0;
  std::atomic<bool> done_flag = false;
  std::atomic<bool> done_flag2 = false;

  thread_pool.async([&]() {
    x += 41;
    done_flag = true;
  });

  thread_pool.async([&]() {
    y += 42;
    done_flag2 = true;
  });

  while (not done_flag) {}
  while (not done_flag2) {}

  REQUIRE(x == 41);
  REQUIRE(y == 42);
}

TEST_CASE("Thread Pool multi worker two tasks",
          "[beyond.core.concurrency.thread_pool]")
{
  beyond::ThreadPool thread_pool;
  REQUIRE(thread_pool.thread_count() == std::thread::hardware_concurrency());

  int x = 0;
  int y = 0;
  std::atomic<bool> done_flag = false;
  std::atomic<bool> done_flag2 = false;

  thread_pool.async([&]() {
    x += 41;
    done_flag = true;
  });

  thread_pool.async([&]() {
    y += 42;
    done_flag2 = true;
  });

  while (not done_flag) {}
  while (not done_flag2) {}

  REQUIRE(x == 41);
  REQUIRE(y == 42);
}

TEST_CASE("Thread Pool one worker 1k tasks",
          "[beyond.core.concurrency.thread_pool]")
{
  static constexpr size_t task_count = 1000;

  std::latch latch{task_count};

  beyond::ThreadPool thread_pool{beyond::ThreadPoolInfo{.thread_count = 1}};
  REQUIRE(thread_pool.thread_count() == 1);

  std::vector<size_t> results(task_count);
  for (size_t i = 0; i < task_count; ++i) {
    thread_pool.async([&, i] {
      results[i] = i;
      latch.count_down();
    });
  }
  latch.wait();

  const auto result = std::reduce(results.begin(), results.end());
  REQUIRE(result == (task_count - 1) * task_count / 2);
}

TEST_CASE("Thread Pool multi worker 1k tasks",
          "[beyond.core.concurrency.thread_pool]")
{
  static constexpr size_t task_count = 1000;

  std::latch latch{task_count};

  beyond::ThreadPool thread_pool;
  REQUIRE(thread_pool.thread_count() == std::thread::hardware_concurrency());

  std::vector<size_t> results(task_count);
  for (size_t i = 0; i < task_count; ++i) {
    thread_pool.async([&, i] {
      results[i] = i;
      latch.count_down();
    });
  }
  latch.wait();

  const auto result = std::reduce(results.begin(), results.end());
  REQUIRE(result == (task_count - 1) * task_count / 2);
}

TEST_CASE("Thread Pool Benchmarks",
          "[.][beyond.core.concurrency.thread_pool][benchmarks]")
{
  beyond::ThreadPool thread_pool;

  static constexpr size_t task_count = 100;
  size_t results[task_count];

  using namespace std::chrono_literals;

  //  BENCHMARK("Single threaded 100 Tasks")
  //  {
  //    for (size_t i = 0; i < task_count; ++i) {
  //      std::this_thread::sleep_for(200ns);
  //    }
  //  };

  BENCHMARK("Thread Pool All Cores 100 Tasks")
  {
    std::latch latch{task_count};

    for (size_t i = 0; i < task_count; ++i) {
      thread_pool.async([&] {
        std::this_thread::sleep_for(200ns);
        latch.count_down();
      });
    }
    latch.wait();

    return results;
  };
}
