#ifndef BEYOND_CORE_CONTAINER_SLOT_MAP_HPP
#define BEYOND_CORE_CONTAINER_SLOT_MAP_HPP

#include "../types/optional.hpp"
#include "../utils/assert.hpp"
#include "../utils/handle.hpp"
#include "../utils/utils.hpp"

#include <concepts>
#include <vector>

namespace beyond {

// A slot map is an associative container where each keys is an integer handle.
// Upon insertion, a key is returned for future reference. Each key is tagged
// with a generation to prevent dangling.
// Insertion, removal, and access are all guaranteed to take O(1) time
template <std::derived_from<HandleBase> Key, class Value,
          template <class...> class Container = std::vector>
class SlotMap {
public:
  using KeyIndex = typename Key::Index;
  using KeyGeneration = typename Key::Generation;

  using size_type = typename Container<Value>::size_type;
  using SizeType = size_type;

private:
  Container<Key> slots_; // Slots stores generational indices to data
  Container<Value> data_;
  Container<KeyIndex> reverse_map_; // from index of data to slots
  // free list
  KeyIndex free_list_first_index_{};
  KeyIndex free_list_last_index_{};

public:
  [[nodiscard]] constexpr auto values() -> std::span<Value>
  {
    return data_;
  }

  [[nodiscard]] constexpr auto values() const -> std::span<const Value>
  {
    return data_;
  }

  [[nodiscard]] constexpr auto size() noexcept -> SizeType
  {
    return data_.size();
  }

  [[nodiscard]] constexpr auto capacity() noexcept -> SizeType
  {
    return data_.capacity();
  }

  constexpr auto try_get(Key key) const -> beyond::optional<const Value&>
  {
    const Key slot = slots_.at(key.index());
    if (slot.generation() != key.generation()) { return beyond::nullopt; }
    return data_.at(slot.index());
  }

  constexpr auto try_get(Key key) -> beyond::optional<Value&>
  {
    const Key slot = slots_.at(key.index());
    if (slot.generation() != key.generation()) { return beyond::nullopt; }
    return data_.at(slot.index());
  }

  [[nodiscard]] constexpr auto insert(const Value& v) -> Key
  {
    this->emplace(v);
  }
  [[nodiscard]] constexpr auto insert(Value&& value) -> Key
  {
    return this->emplace(std::move(value));
  }

  constexpr auto erase(Key key)
  {
    const auto slot_index = key.index();
    auto& slot = slots_.at(slot_index);
    BEYOND_ENSURE(key.generation() == slot.generation());
    const auto data_index = slot.index();

    slot = Key{free_list_first_index_,
               static_cast<KeyGeneration>(slot.generation() + 1)};
    free_list_first_index_ = slot_index;
    if (free_list_last_index_ == slots_.size()) { // free list is empty
      free_list_last_index_ = free_list_first_index_;
    }

    data_[data_index] = std::move(data_.back());
    data_.pop_back();

    reverse_map_[data_index] = std::move(reverse_map_.back());
    reverse_map_.pop_back();

    slots_.at(reverse_map_[data_index]).set_index(data_index);
  }

  template <class... Args> constexpr auto emplace(Args&&... args) -> Key
  {
    const auto data_index = static_cast<KeyIndex>(data_.size());
    data_.emplace_back(std::forward<Args>(args)...);

    KeyGeneration generation = 0;
    const KeyIndex slot_location = free_list_first_index_;
    if (free_list_last_index_ == slots_.size()) {  // free list is empty
      slots_.emplace_back(data_index, generation); // make a new slot

      free_list_first_index_ = static_cast<KeyIndex>(slots_.size());
      free_list_last_index_ = static_cast<KeyIndex>(slots_.size());
    } else {
      auto& slot = slots_.at(slot_location);
      if (free_list_first_index_ == free_list_last_index_) { // Last free slot
        free_list_last_index_ = static_cast<KeyIndex>(slots_.size());
      }
      free_list_first_index_ = slot.index();
      generation = slot.generation();
      slot = Key{data_index, generation};
    }

    reverse_map_.emplace_back(slot_location);

    return Key{slot_location, generation};
  }
};

} // namespace beyond

#endif // BEYOND_CORE_CONTAINER_SLOT_MAP_HPP
