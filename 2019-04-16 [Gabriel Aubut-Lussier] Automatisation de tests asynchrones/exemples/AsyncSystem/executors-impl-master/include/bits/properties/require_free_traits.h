#ifndef STD_BITS_PROPERTIES_REQUIRE_FREE_TRAITS_H_INCLUDED
#define STD_BITS_PROPERTIES_REQUIRE_FREE_TRAITS_H_INCLUDED

#include <type_traits>

namespace std {
namespace impl {

template<class Entity, class Property, class = std::void_t<>>
struct require_free_traits
{
  static constexpr bool is_valid = false;
  static constexpr bool is_noexcept = false;
};

template<class Entity, class Property>
struct require_free_traits<Entity, Property,
  std::void_t<
    decltype(require(std::declval<Entity>(), std::declval<Property>()))
  >>
{
  static constexpr bool is_valid = true;
  using result_type = decltype(require(std::declval<Entity>(), std::declval<Property>()));
  static constexpr bool is_noexcept = noexcept(require(std::declval<Entity>(), std::declval<Property>()));
};

} // namespace impl
} // namespace std

#endif // STD_BITS_PROPERTIES_REQUIRE_FREE_TRAITS_H_INCLUDED
