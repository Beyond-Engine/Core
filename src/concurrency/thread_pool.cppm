module;

#include <beyond/prelude.hpp>

#include <cstdint>
#include <memory>

export module beyond.core:concurrency.thread_pool;

import :unique_function;

namespace beyond {

using Task = beyond::unique_function<void() const>;

/**
 * @addtogroup core
 * @{
 * @addtogroup concurrency
 * @{
 */

/// @brief Returns std::thread::hardware_concurrency
export [[nodiscard]] auto hardware_concurrency() -> std::size_t;

export struct ThreadPoolInfo {
  std::size_t thread_count = hardware_concurrency();
};

/**
 * @brief A work-stealing thread pool
 */
export class ThreadPool {
  std::unique_ptr<class ThreadPoolImpl> impl_;

public:
  explicit ThreadPool(const ThreadPoolInfo& info = {});
  ~ThreadPool();

  /// @brief Number of worker threads
  [[nodiscard]] auto thread_count() const noexcept -> std::size_t;

  void async(Task task);

  ThreadPool(const ThreadPool&) = delete;
  auto operator=(const ThreadPool&) & -> ThreadPool& = delete;
  ThreadPool(ThreadPool&&) = delete;
  auto operator=(ThreadPool&&) & -> ThreadPool& = delete;
};

/** @}@} */

} // namespace beyond
