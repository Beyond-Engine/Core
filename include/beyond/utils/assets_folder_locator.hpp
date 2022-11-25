#ifndef CHARLIE3D_ASSETS_FOLDER_LOCATOR_HPP
#define CHARLIE3D_ASSETS_FOLDER_LOCATOR_HPP

#include <concepts>
#include <filesystem>

#include "../types/optional.hpp"

namespace beyond {

template <typename Fn>
requires std::is_invocable_r_v<bool, Fn, std::filesystem::path>
auto upward_directory_find(const std::filesystem::path& from, Fn condition)
    -> beyond::optional<std::filesystem::path>
{
  for (auto directory_path = from; directory_path != from.root_path();
       directory_path = directory_path.parent_path()) {
    if (condition(directory_path)) { return directory_path; }
  }
  return beyond::nullopt;
}

auto locate_asset_path(const std::filesystem::path& exe_directory_path)
    -> beyond::optional<std::filesystem::path>;

} // namespace beyond

#endif // CHARLIE3D_ASSETS_FOLDER_LOCATOR_HPP
