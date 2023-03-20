// #include <atomic>
// #include <cstdint>
// #include <jthread.hpp>
//
// #include <beyond/concurrency/task_queue.hpp>
//
//  namespace beyond {
//
///**
// * @addtogroup core
// * @{
// * @addtogroup concurrency
// * @{
// */
//
// class ThreadPool {
//  std::size_t count_ = std::thread::hardware_concurrency();
//  std::vector<nostd::jthread> threads_;
//  std::vector<TaskQueue> queues_{count_};
//  std::atomic<std::size_t> index_ = 0;
//
// public:
//  ThreadPool()
//  {
//    threads_.reserve(count_);
//    for (std::uint32_t i = 0; i < count_; ++i) {
//      threads_.emplace_back([this, i] { run(i); });
//    }
//  }
//
//  ~ThreadPool()
//  {
//    for (auto& queue : queues_) {
//      queue.done();
//    }
//  }
//
//  ThreadPool(const ThreadPool&) = delete;
//  auto operator=(const ThreadPool&) & -> ThreadPool& = delete;
//  ThreadPool(ThreadPool&&) noexcept = delete;
//  auto operator=(ThreadPool&&) & noexcept -> ThreadPool = delete;
//
//  [[nodiscard]] auto size() const noexcept -> std::size_t
//  {
//    return threads_.size();
//  }
//
//  template <typename F> auto async(F&& f) -> void
//  {
//    const auto i = index_++;
//
//    for (std::size_t n = 0; n != count_; ++n) {
//      if (queues_[(i + n) % count_].try_push(std::forward<F>(f))) return;
//    }
//
//    queues_[i % count_].push(std::forward<F>(f));
//  }
//
// private:
//  auto run(unsigned i) -> void
//  {
//    beyond::unique_function<void() const> f;
//
//    while (true) {
//      for (std::size_t n = 0; n != count_; ++n) {
//        if (auto f_out = queues_[(i + n) % count_].try_pop();
//            f_out != beyond::nullopt) {
//          f = std::move(*f_out);
//          break;
//        }
//      }
//      if (f) {
//        f();
//      } else {
//        auto f_opt = queues_[i].pop();
//        if (!f_opt) break;
//        (*f_opt)();
//      }
//    }
//  }
//};
//
///** @}@} */
//
//} // namespace beyond
//
// #include <catch2/catch_test_macros.hpp>
//
// TEST_CASE("Thread Pool", "[beyond.core.concurrency.thread_pool]")
//{
//  beyond::ThreadPool thread_pool;
//  REQUIRE(thread_pool.size() == std::thread::hardware_concurrency());
//
//  int x = 0;
//  int y = 1;
//  std::atomic<bool> done_flag = false;
//  std::atomic<bool> done_flag2 = false;
//
//  thread_pool.async([&]() {
//    std::cout << "Task 1" << std::endl;
//    x = 42;
//    done_flag.store(true);
//  });
//
//  thread_pool.async([&]() {
//    std::cout << "Task 2" << std::endl;
//    y = 42;
//    done_flag2.store(true);
//  });
//
//  while (!done_flag.load()) {}
//  while (!done_flag2.load()) {}
//
//  REQUIRE(x == 42);
//  REQUIRE(y == 42);
//}
