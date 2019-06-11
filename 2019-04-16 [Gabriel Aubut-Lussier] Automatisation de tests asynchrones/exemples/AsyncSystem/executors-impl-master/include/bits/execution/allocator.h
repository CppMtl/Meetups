#ifndef STD_BITS_EXECUTION_ALLOCATOR_H_INCLUDED
#define STD_BITS_EXECUTION_ALLOCATOR_H_INCLUDED

#include <bits/execution/is_executor.h>

namespace std {
namespace execution {
namespace allocator_impl {

template<class Derived>
struct property_base
{
  static constexpr bool is_requirable_concept = false;
  static constexpr bool is_requirable = true;
  static constexpr bool is_preferable = true;

  template<class Executor, class Type = decltype(Executor::query(*static_cast<Derived*>(0)))>
    static constexpr Type static_query_v = Executor::query(Derived());
};

} // namespace allocator_impl

template<class ProtoAllocator>
struct allocator_t : allocator_impl::property_base<allocator_t<ProtoAllocator>>
{
  constexpr explicit allocator_t(const ProtoAllocator& a) : alloc_(a) {}
  constexpr ProtoAllocator value() const { return alloc_; }
  ProtoAllocator alloc_;
};

template<>
struct allocator_t<void> : allocator_impl::property_base<allocator_t<void>>
{
  template<class ProtoAllocator>
  allocator_t<ProtoAllocator> operator()(const ProtoAllocator& a) const
  {
    return allocator_t<ProtoAllocator>(a);
  }
};

constexpr allocator_t<void> allocator;

} // namespace execution

#if defined(__cpp_concepts)
template<execution::Executor E, class T>
struct is_applicable_property<E, execution::allocator_t<T>> : std::true_type {};
#else
template<class Entity, class T>
struct is_applicable_property<Entity, execution::allocator_t<T>,
  std::enable_if_t<execution::is_executor_v<Entity>>> : std::true_type {};
#endif

} // namespace std

#endif // STD_BITS_EXECUTION_ALLOCATOR_H_INCLUDED
