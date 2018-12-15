#ifndef VALIDEUR_JSON_HPP
#define VALIDEUR_JSON_HPP

#include <boost/utility/string_view.hpp>
#include <optional>
#include <rapidjson/document.h>
#include <type_traits>

template <typename T>
struct ValideurJSON;

template <typename T>
std::optional<T> ValiderJSON(const rapidjson::Value&)
{
	static_assert(!std::is_same_v<T, T>, "Specialization required.");
	return std::nullopt;
}

template <>
std::optional<int> ValiderJSON<int>(const rapidjson::Value& json);
template <>
std::optional<int64_t> ValiderJSON<int64_t>(const rapidjson::Value& json);
template <>
std::optional<unsigned int> ValiderJSON<unsigned int>(const rapidjson::Value& json);
template <>
std::optional<uint64_t> ValiderJSON<uint64_t>(const rapidjson::Value& json);
template <>
std::optional<float> ValiderJSON<float>(const rapidjson::Value& json);
template <>
std::optional<double> ValiderJSON<double>(const rapidjson::Value& json);
template <>
std::optional<bool> ValiderJSON<bool>(const rapidjson::Value& json);
template <>
std::optional<std::string> ValiderJSON<std::string>(const rapidjson::Value& json);

template <typename T>
std::optional<T> ValiderJSON(const rapidjson::Value& json, boost::string_view key)
{
	if (json.IsObject()) {
		const auto it = json.FindMember(key.data());
		const auto itEnd = json.MemberEnd();
		if (it != itEnd) {
			return ValiderJSON<T>(it->value);
		}
	}
	return std::nullopt;
}

template <typename T>
std::optional<std::vector<T>> ValiderJSONArray(const rapidjson::Value& json)
{
	if (json.IsArray()) {
		std::vector<T> elements;
		const auto size = json.Size();
		elements.reserve(size);
		for (rapidjson::SizeType i = 0; i < size; ++i) {
			auto elemOpt = ValiderJSON<T>(json[i]);
			if (elemOpt) {
				elements.push_back(std::move(*elemOpt));
			} else {
				return std::nullopt;
			}
		}
		return elements;
	}
	return std::nullopt;
}

template <typename T>
std::optional<std::vector<T>> ValiderJSONArray(const rapidjson::Value& json, boost::string_view key)
{
	const auto it = json.FindMember(key.data());
	const auto itEnd = json.MemberEnd();
	if (it == itEnd) {
		return std::nullopt;
	} else {
		return ValiderJSONArray<T>(it->value);
	}
}

template <typename T>
struct ValideurJSON
{
	std::optional<T> operator()(boost::string_view sv) const
	{
		rapidjson::Document doc;
		if (!doc.Parse(sv.data(), sv.size()).HasParseError()) {
			return ValiderJSON<T>(doc);
		}
		return std::nullopt;
	}
};

#endif
