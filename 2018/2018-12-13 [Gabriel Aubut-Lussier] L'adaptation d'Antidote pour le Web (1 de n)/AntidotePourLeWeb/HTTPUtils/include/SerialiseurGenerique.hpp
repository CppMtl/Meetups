#ifndef SERIALISEUR_GENERIQUE_HPP
#define SERIALISEUR_GENERIQUE_HPP

#include <string>
#include <type_traits>

template <typename T>
struct SerialiseurGenerique
{
	std::string operator()(const T&) const
	{
		static_assert(!std::is_same_v<T, T>, "Specialization required.");
	}
};

#endif
