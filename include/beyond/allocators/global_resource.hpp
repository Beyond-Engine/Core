#ifndef BEYOND_CORE_ALLOCATORS_GLOBAL_RESOURCE_HPP
#define BEYOND_CORE_ALLOCATORS_GLOBAL_RESOURCE_HPP

#include "memory_resource.hpp"

namespace beyond {

auto new_delete_resource() noexcept -> MemoryResource&;
auto panic_resource() noexcept -> MemoryResource&;
auto get_default_resource() noexcept -> MemoryResource&;
auto set_default_resource(MemoryResource& r) noexcept -> MemoryResource&;

} // namespace beyond

#endif // BEYOND_CORE_ALLOCATORS_GLOBAL_RESOURCE_HPP
