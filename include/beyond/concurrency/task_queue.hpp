#pragma once

#ifndef BEYOND_CORE_CONCURRENCY_TASK_QUEUE_HPP
#define BEYOND_CORE_CONCURRENCY_TASK_QUEUE_HPP

#include <beyond/utils/unique_function.hpp>

#include "../types/optional.hpp"

#include <condition_variable>
#include <mutex>
#include <queue>

/**
 * @defgroup concurrency Concurrency
 * @brief  contains high level constructs for concurrent programming.
 * @ingroup core
 */

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup concurrency
 * @{
 */

/**
 * @brief A serial queue of tasks
 */
class TaskQueue {
public:
  using Task = beyond::unique_function<void() const>;

  TaskQueue() = default;

  /**
   * @brief Gives up the rest of the tasks in the task queue
   */
  auto done() -> void
  {
    {
      std::lock_guard lock{mutex_};
      done_ = true;
    }
    ready_.notify_all();
  }

  /**
   * @brief Pops a element from the SerialQueue
   *
   * IF the queue is empty, this function will block. If the queue is done. then
   * this function will return a `std::nullopt`.
   * @note Block if the queue is empty
   */
  [[nodiscard]] auto pop() -> beyond::optional<Task>
  {
    std::unique_lock lock{mutex_};
    ready_.wait(lock, [&]() { return !queue_.empty() || done_; });

    if (done_) { return beyond::nullopt; }

    beyond::optional<Task> task{beyond::in_place, std::move(queue_.front())};
    queue_.pop();
    return task;
  }

  /**
   * @brief Pushes elements into the SerialQueue
   */
  template <typename Func> auto push(Func&& f) -> void
  {
    {
      std::lock_guard lock{mutex_};
      queue_.emplace(std::forward<Func>(f));
    }
    ready_.notify_one();
  }

  /**
   * @brief Tries to pop a task from the queue
   *
   * If the queue is busy or if queue is empty, return `std::nullopt`. Otherwise
   * return the task poped from the queue.
   */
  [[nodiscard]] auto try_pop() -> beyond::optional<Task>
  {
    std::unique_lock lock{mutex_, std::try_to_lock};
    if (!lock || queue_.empty()) { return beyond::nullopt; }

    beyond::optional<Task> task{beyond::in_place, std::move(queue_.front())};
    queue_.pop();
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
      if (lock.owns_lock()) { return false; }
      queue_.emplace(std::forward<F>(f));
    }
    ready_.notify_one();
    return true;
  }

  /**
   * @brief Returns `true` if the queue is empty
   */
  [[nodiscard]] auto empty() const noexcept -> bool
  {
    std::lock_guard lock{mutex_};
    return queue_.empty();
  }

private:
  std::queue<Task> queue_; // Protected by the mutex
  bool done_{false};       // Protected by the mutex
  mutable std::mutex mutex_;
  std::condition_variable ready_;
};

/** @}@} */

} // namespace beyond

#endif // BEYOND_CORE_CONCURRENCY_TASK_QUEUE_HPP
