#ifndef VALIDATEUR_GENERIQUE_H
#define VALIDATEUR_GENERIQUE_H

#include <boost/utility/string_view.hpp>
#include <optional>
#include <string>
#include <type_traits>

template <typename T>
struct ValideurGenerique
{
	std::optional<T> operator()(boost::string_view str)
	{
		static_assert(!std::is_same_v<T, T>, "Specialization required.");
		return std::nullopt;
	}
};

template <>
struct ValideurGenerique<int>
{
	std::optional<int> operator()(boost::string_view sv)
	{
		std::string str{sv.data(), sv.size()};
		return std::stoi(str);
	}
};

template <>
struct ValideurGenerique<long>
{
	std::optional<long> operator()(boost::string_view sv)
	{
		std::string str{sv.data(), sv.size()};
		return std::stol(str);
	}
};

template <>
struct ValideurGenerique<long long>
{
	std::optional<long long> operator()(boost::string_view sv)
	{
		std::string str{sv.data(), sv.size()};
		return std::stoll(str);
	}
};

template <>
struct ValideurGenerique<unsigned int>
{
	std::optional<unsigned int> operator()(boost::string_view sv)
	{
		std::string str{sv.data(), sv.size()};
		return static_cast<unsigned int>(std::stoul(str));
	}
};

template <>
struct ValideurGenerique<unsigned long>
{
	std::optional<unsigned long> operator()(boost::string_view sv)
	{
		std::string str{sv.data(), sv.size()};
		return std::stoul(str);
	}
};

template <>
struct ValideurGenerique<unsigned long long>
{
	std::optional<unsigned long long> operator()(boost::string_view sv)
	{
		std::string str{sv.data(), sv.size()};
		return std::stoull(str);
	}
};

template <>
struct ValideurGenerique<float>
{
	std::optional<float> operator()(boost::string_view sv)
	{
		std::string str{sv.data(), sv.size()};
		return std::stof(str);
	}
};

template <>
struct ValideurGenerique<double>
{
	std::optional<double> operator()(boost::string_view sv)
	{
		std::string str{sv.data(), sv.size()};
		return std::stod(str);
	}
};

template <>
struct ValideurGenerique<long double>
{
	std::optional<long double> operator()(boost::string_view sv)
	{
		std::string str{sv.data(), sv.size()};
		return std::stold(str);
	}
};

template <>
struct ValideurGenerique<std::string>
{
	std::optional<std::string> operator()(boost::string_view sv)
	{
		return std::string{sv.data(), sv.size()};
	}
};

template <>
struct ValideurGenerique<boost::string_view>
{
	std::optional<boost::string_view> operator()(boost::string_view sv)
	{
		return sv;
	}
};

#endif
