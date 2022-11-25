#include "assets_folder_locator.hpp"

auto beyond::locate_asset_path(const std::filesystem::path& exe_directory_path)
    -> beyond::optional<std::filesystem::path>
{
  using std::filesystem::path;
  const auto append_asset = [](const path& path) { return path / "assets"; };
  const auto parent_path = upward_directory_find(
      exe_directory_path, [&](const std::filesystem::path& path) {
        const auto assets_path = append_asset(path);
        return exists(assets_path) && is_directory(assets_path);
      });
  return parent_path.map(
      [&](const path& path) { return absolute(append_asset(path)); });
}
