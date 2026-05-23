module;

#include "beyond/utils/utils.hpp"

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

module beyond.core;

import :unique_function;

namespace {

class TaskQueue {
  std::deque<beyond::Task> queue_{}; // Protected by the mutex
  bool done_{false};                 // Protected by the mutex
  mutable std::mutex mutex_;
  std::condition_variable ready_;

public:
  TaskQueue() = default;

  /**
   * @brief Gives up the rest of the tasks in the task queue
   */
  auto done() -> void
  {
    {
      std::scoped_lock lock{mutex_};
      done_ = true;
    }
    ready_.notify_all();
  }

  /**
   * @brief Pops a element from the SerialQueue
   *
   * If the queue is empty, this function will block. If the queue is done, then
   * this function will return a default constructed Task.
   * @note Block if the queue is empty
   */
  [[nodiscard]] auto pop() -> beyond::Task
  {
    std::unique_lock lock{mutex_};
    ready_.wait(lock, [this]() { return !queue_.empty() || done_; });
    if (done_) { return beyond::Task{}; }

    beyond::Task task = std::move(queue_.front());
    queue_.pop_front();
    return task;
  }

  /**
   * @brief Pushes elements into the TaskQueue
   */
  template <typename Func> void push(Func&& f)
  {
    {
      std::lock_guard lock{mutex_};
      queue_.emplace_back(BEYOND_FWD(f));
    }
    ready_.notify_one();
  }

  /**
   * @brief Tries to pop a task from the queue
   *
   * If the queue is busy or if queue is empty, return a default constructed
   * task. Otherwise return the task popped from the queue.
   */
  [[nodiscard]] auto try_pop() -> beyond::Task
  {
    std::unique_lock lock{mutex_, std::try_to_lock};
    if (!lock || queue_.empty()) { return beyond::Task{}; }

    beyond::Task task = std::move(queue_.front());
    queue_.pop_front();
    return task;
  }

  /**
   * @brief Tries to push a task to the queue
   *
   * If the queue is busy, does not push to the queue and returns false.
   * Otherwise pushes to the queue and returns true.
   */
  template <typename F> auto try_push(F&& f) -> bool
  {
    {
      std::unique_lock lock{mutex_, std::try_to_lock};
      if (not lock.owns_lock()) { return false; }
      queue_.emplace_back(std::forward<F>(f));
    }
    ready_.notify_one();
    return true;
  }
};

} // namespace

namespace beyond {

auto hardware_concurrency() -> std::size_t
{ return std::thread::hardware_concurrency(); }

class ThreadPoolImpl {
  std::size_t thread_count_ = std::thread::hardware_concurrency();
  std::vector<std::jthread> threads_;

  std::vector<TaskQueue> queues_;
  std::atomic<std::size_t> index_ = 0;

public:
  explicit ThreadPoolImpl(const ThreadPoolInfo& info)
      : thread_count_{info.thread_count}, queues_(thread_count_)
  {
    threads_.reserve(thread_count_);
    for (std::uint32_t i = 0; i < thread_count_; ++i) {
      threads_.emplace_back([this, i] { this->run(i); });
    }
  }

  ~ThreadPoolImpl()
  {
    for (auto& queue : queues_) {
      queue.done();
    }
    for (auto& thread : threads_) {
      thread.join();
    }
  }

  ThreadPoolImpl(const ThreadPoolImpl&) = delete;
  auto operator=(const ThreadPoolImpl&) & -> ThreadPoolImpl& = delete;

  /// @brief Number of worker threads
  [[nodiscard]] auto thread_count() const noexcept -> std::size_t
  { return threads_.size(); }

  void async(Task task)
  {
    const auto i = index_++;

    for (std::size_t n = 0; n != thread_count_; ++n) {
      const auto queue_index = (i + n) % thread_count_;
      if (queues_[queue_index].try_push(BEYOND_MOV(task))) { return; }
    }

    queues_[i % thread_count_].push(BEYOND_MOV(task));
  }

private:
  void run(unsigned i)
  {
    while (true) {
      Task task;

      for (std::size_t n = 0; n != thread_count_; ++n) {
        task = queues_[(i + n) % thread_count_].try_pop();
        if (task != nullptr) { break; }
      }
      if (!task) { task = queues_[i].pop(); }
      if (!task) { break; } // Only possible if the queue is done
      task();
    }
  }
};

ThreadPool::ThreadPool(const beyond::ThreadPoolInfo& info)
    : impl_{std::make_unique<ThreadPoolImpl>(info)}
{
}

ThreadPool::~ThreadPool() = default;

[[nodiscard]] auto ThreadPool::thread_count() const noexcept -> std::size_t
{ return impl_->thread_count(); }

void ThreadPool::async(Task task)
{ impl_->async(BEYOND_MOV(task)); }

} // namespace beyond
