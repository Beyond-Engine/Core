#ifndef BEYOND_CORE_CONCURRENCY_THREAD_POOL_HPP
#define BEYOND_CORE_CONCURRENCY_THREAD_POOL_HPP

#include "../utils/unique_function.hpp"
#include "../utils/utils.hpp"

#include <cstdint>
#include <memory>

namespace beyond {

using Task = beyond::unique_function<void() const>;

/**
 * @addtogroup core
 * @{
 * @addtogroup concurrency
 * @{
 */

/// @brief Returns std::thread::hardware_concurrency
[[nodiscard]] auto hardware_concurrency() -> std::size_t;

struct ThreadPoolInfo {
  std::size_t thread_count = hardware_concurrency();
};

/**
 * @brief A work-stealing thread pool
 */
class ThreadPool {
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

#endif // BEYOND_CORE_CONCURRENCY_THREAD_POOL_HPP
