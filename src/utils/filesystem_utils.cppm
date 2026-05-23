module;

#include <filesystem>
#include <type_traits>

export module beyond.core:filesystem_utils;

import :optional;

export namespace beyond {

/**
 * Walks up the directory tree from the given directory, testing the
 * condition at each ancestor. Stops at the first directory that satisfies it.
 */
template <typename Fn>
  requires std::is_invocable_r_v<bool, Fn, std::filesystem::path>
auto upward_directory_find(const std::filesystem::path& from, Fn condition)
    -> optional<std::filesystem::path>
{
  for (auto directory_path = from; directory_path != from.root_path();
       directory_path = directory_path.parent_path()) {
    if (condition(directory_path)) { return directory_path; }
  }
  return nullopt;
}

} // namespace beyond
