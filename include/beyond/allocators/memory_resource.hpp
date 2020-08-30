#ifndef BEYOND_CORE_ALLOCATORS_MEMORY_RESOURCE_HPP
#define BEYOND_CORE_ALLOCATORS_MEMORY_RESOURCE_HPP

#include <cstddef>

namespace beyond {

/**
 * The MemoryResource class is an abstract interface for memory resources such
 * as .
 */
class MemoryResource {
  static constexpr std::size_t max_align = alignof(std::max_align_t);

public:
  virtual ~MemoryResource() noexcept = default;

  /**
   *
   * @brief Allocates memory
   *
   * Allocates with a size of at least bytes. The returned  storage is  aligned
   * to the specified alignment, if such alignment is supported; otherwise it is
   * aligned to max_align.
   *
   * @pre Alignment shall be a power of two.
   * @return a pointer o allocated storage
   * @warning A derived class implementation may `panic()` if it is unable to
   * allocate memory with the requested size and alignment.
   */
  [[nodiscard]] auto allocate(std::size_t bytes,
                              std::size_t alignment = max_align) -> void*
  {
    return this->do_allocate(bytes, alignment);
  }

  /**
   * @brief Deallocates memory
   *
   * @pre p shall have been returned from a prior call to `allocate(bytes,
   * alignment)` on a memory resource equal to *this, and the storage at p shall
   * not yet have been deallocated.
   */
  auto deallocate(void* p, std::size_t bytes, std::size_t alignment = max_align)
  {
    return this->do_deallocate(p, bytes, alignment);
  }

  /**
   * @brief Checks if this memory resource equals to other
   */
  [[nodiscard]] auto is_equal(const MemoryResource& other) const noexcept
      -> bool
  {
    return this->do_is_equal(other);
  }

  /**
   * @brief Checks if the two MemoryResource are equal to each other
   * @see is_equal
   */
  [[nodiscard]] friend auto operator==(const MemoryResource& lhs,
                                       const MemoryResource& rhs) noexcept
      -> bool
  {
    return &lhs == &rhs || lhs.is_equal(rhs);
  }

private:
  /// @see allocate
  [[nodiscard]] virtual auto do_allocate(std::size_t bytes,
                                         std::size_t alignment) -> void* = 0;

  /// @see deallocate
  virtual auto do_deallocate(void* p, std::size_t bytes, std::size_t alignment)
      -> void = 0;

  /// @see is_equal
  [[nodiscard]] virtual auto
  do_is_equal(const MemoryResource& other) const noexcept -> bool = 0;
};

} // namespace beyond

#endif // BEYOND_CORE_ALLOCATORS_MEMORY_RESOURCE_HPP
