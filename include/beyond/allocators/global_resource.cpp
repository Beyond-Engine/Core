#include <atomic>
#include <new>

#include "../utils/panic.hpp"
#include "global_resource.hpp"

namespace beyond {

struct NewDeleteResource : MemoryResource {
  auto do_allocate(std::size_t bytes, std::size_t alignment) -> void* override
  {
    return ::operator new[](bytes, std::align_val_t{alignment});
  }

  void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override
  {
    ::operator delete[](static_cast<char*>(p), bytes,
                        std::align_val_t{alignment});
  }

  [[nodiscard]] auto do_is_equal(const MemoryResource& other) const noexcept
      -> bool override
  {
    return &other == this;
  }
} new_delete_resource_instance;

struct PanicResource : MemoryResource {
  auto do_allocate(std::size_t, std::size_t) -> void* override
  {
    beyond::panic("Invoke panic resource!");
  }

  void do_deallocate(void*, std::size_t, std::size_t) override {}

  [[nodiscard]] auto do_is_equal(const MemoryResource& other) const noexcept
      -> bool override
  {
    return &other == this;
  }
} panic_resource_instance;

auto new_delete_resource() noexcept -> MemoryResource&
{
  return new_delete_resource_instance;
}

auto panic_resource() noexcept -> MemoryResource&
{
  return panic_resource_instance;
}

static std::atomic<MemoryResource*> default_memory_resource =
    &new_delete_resource_instance;

auto get_default_resource() noexcept -> MemoryResource&
{
  return *default_memory_resource.load();
}

auto set_default_resource(MemoryResource& r) noexcept -> MemoryResource&
{
  return *default_memory_resource.exchange(&r);
}

} // namespace beyond