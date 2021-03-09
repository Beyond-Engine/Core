#ifndef BEYOND_CORE_ALGORITHM_SORT_BY_KEY_HPP
#define BEYOND_CORE_ALGORITHM_SORT_BY_KEY_HPP

#include <algorithm>
#include <cstdint>

namespace beyond {

namespace detail {

template <typename KeyItr, typename MappedItr> struct SortByKeyVal {
  typename std::iterator_traits<KeyItr>::value_type key;
  typename std::iterator_traits<MappedItr>::value_type mapped;
};

template <typename KeyItr, typename MappedItr> struct SortByKeyRef {
  KeyItr key;
  MappedItr mapped;

  constexpr SortByKeyRef(KeyItr key_, MappedItr mapped_)
      : key{key_}, mapped{mapped_}
  {
  }

  constexpr SortByKeyRef(SortByKeyRef&& r) noexcept
  {
    *key = std::exchange(*r.key, {});
    *mapped = std::exchange(*r.mapped, {});
  }

  constexpr auto operator=(SortByKeyRef&& r) noexcept -> SortByKeyRef&
  {
    *key = std::exchange(*r.key, {});
    *mapped = std::exchange(*r.mapped, {});
    return *this;
  }

  constexpr auto operator=(SortByKeyVal<KeyItr, MappedItr>&& r) noexcept
      -> SortByKeyRef&
  {
    *key = std::exchange(r.key, {});
    *mapped = std::exchange(r.mapped, {});
    return *this;
  }

  friend constexpr void swap(SortByKeyRef a, SortByKeyRef b) noexcept
  {
    std::swap(*a.key, *b.key);
    std::swap(*a.mapped, *b.mapped);
  }

  explicit(false) constexpr operator SortByKeyVal<KeyItr, MappedItr>() &&
  {
    return {std::move(*key), std::move(*mapped)};
  }
};

template <typename KeyItr, typename MappedItr>
constexpr auto operator<(const SortByKeyRef<KeyItr, MappedItr>& a,
                         const SortByKeyVal<KeyItr, MappedItr>& b) noexcept
    -> bool
{
  return *a.key < b.key;
}

template <typename KeyItr, typename MappedItr>
constexpr auto operator<(const SortByKeyVal<KeyItr, MappedItr>& a,
                         const SortByKeyRef<KeyItr, MappedItr>& b) noexcept
    -> bool
{
  return a.key < *b.key;
}

template <typename KeyItr, typename MappedItr>
constexpr auto operator<(const SortByKeyRef<KeyItr, MappedItr>& a,
                         const SortByKeyRef<KeyItr, MappedItr>& b) noexcept
    -> bool
{
  return *a.key < *b.key;
}

template <typename KeyItr, typename MappedItr> struct SortByKeyIterator {
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::int64_t;
  using value_type = SortByKeyVal<KeyItr, MappedItr>;
  using pointer = value_type*;
  using reference = SortByKeyRef<KeyItr, MappedItr>;

  size_t index = 0;
  KeyItr key = {};
  MappedItr mapped = {};

  [[nodiscard]] constexpr auto
  operator==(const SortByKeyIterator& r) const noexcept -> bool
  {
    return index == r.index;
  }

  [[nodiscard]] constexpr auto operator+(difference_type i) const noexcept
      -> SortByKeyIterator
  {
    return {index + i, key, mapped};
  }
  [[nodiscard]] friend constexpr auto operator+(difference_type i,
                                                SortByKeyIterator itr) noexcept
      -> SortByKeyIterator
  {
    return {itr.index + i, itr.key, itr.mapped};
  }

  [[nodiscard]] constexpr auto operator-(difference_type i) const noexcept
      -> SortByKeyIterator
  {
    return {index - i, key, mapped};
  }

  constexpr auto operator+=(difference_type i) noexcept -> SortByKeyIterator&
  {
    index += i;
    return *this;
  }

  constexpr auto operator-=(difference_type i) noexcept -> SortByKeyIterator&
  {
    index -= i;
    return *this;
  }

  [[nodiscard]] constexpr auto
  operator-(SortByKeyIterator const& r) const noexcept -> difference_type
  {
    return difference_type(index) - difference_type(r.index);
  }

  constexpr auto operator++() noexcept -> SortByKeyIterator&
  {
    ++index;
    return *this;
  }
  constexpr auto operator--() noexcept -> SortByKeyIterator&
  {
    --index;
    return *this;
  }
  constexpr auto operator++(int) noexcept -> SortByKeyIterator
  {
    return SortByKeyIterator{++index, key, mapped};
  }
  constexpr auto operator--(int) noexcept -> SortByKeyIterator
  {
    return SortByKeyIterator{--index, key, mapped};
  }

  constexpr auto operator[](std::size_t i) const noexcept -> reference
  {
    return {key + index + i, mapped + index + i};
  }

  [[nodiscard]] constexpr auto
  operator<=>(SortByKeyIterator const& r) const noexcept -> std::strong_ordering
  {
    return index <=> r.index;
  }

  [[nodiscard]] constexpr auto operator*() const noexcept -> reference
  {
    return reference{key + index, mapped + index};
  }
};

} // namespace detail

/**
 * sort_by_key performs a key-value sort. That is, sort_by_key sorts the
 elements in [keys_begin, keys_end) and [mapped_begin, mapped_begin + (keys_end
 - keys_begin)) into ascending key order, meaning that if i and j are any two
 valid iterators in [keys_first, keys_last) such that i precedes j, and p and q
 are iterators in [mapped_begin, mapped_begin + (keys_end - keys_begin))
 corresponding to i and j respectively, then *j is not less than *i.
 * @param keys_begin The beginning of the key sequence.
 * @param keys_end The end of the mapped sequence.
 * @param mapped_begin The beginning of the value sequence.
 * @pre The range [keys_begin, keys_end)) shall not overlap the range
 [mapped_begin, mapped_begin + (keys_end - keys_begin)).
 */
template <std::random_access_iterator KeyItr,
          std::random_access_iterator MappedItr>
constexpr void sort_by_key(KeyItr keys_begin, KeyItr keys_end,
                           MappedItr mapped_begin)
{
  std::sort(
      detail::SortByKeyIterator<KeyItr, MappedItr>{0, keys_begin, mapped_begin},
      detail::SortByKeyIterator<KeyItr, MappedItr>{
          static_cast<std::size_t>(keys_end - keys_begin), keys_begin,
          mapped_begin});
}
} // namespace beyond

#endif // BEYOND_CORE_ALGORITHM_SORT_BY_KEY_HPP
