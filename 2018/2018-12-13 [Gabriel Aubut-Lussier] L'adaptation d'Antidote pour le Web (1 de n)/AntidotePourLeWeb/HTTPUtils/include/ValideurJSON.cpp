#include "ValideurJSON.hpp"

template <>
std::optional<int> ValiderJSON<int>(const rapidjson::Value& json)
{
	if (json.IsNumber()) {
		return json.GetInt();
	}
	return std::nullopt;
}

template <>
std::optional<int64_t> ValiderJSON<int64_t>(const rapidjson::Value& json)
{
	if (json.IsNumber()) {
		return json.GetInt64();
	}
	return std::nullopt;
}

template <>
std::optional<unsigned int> ValiderJSON<unsigned int>(const rapidjson::Value& json)
{
	if (json.IsNumber()) {
		return json.GetUint();
	}
	return std::nullopt;
}

template <>
std::optional<uint64_t> ValiderJSON<uint64_t>(const rapidjson::Value& json)
{
	if (json.IsNumber()) {
		return json.GetUint64();
	}
	return std::nullopt;
}

template <>
std::optional<float> ValiderJSON<float>(const rapidjson::Value& json)
{
	if (json.IsNumber()) {
		return json.GetFloat();
	}
	return std::nullopt;
}

template <>
std::optional<double> ValiderJSON<double>(const rapidjson::Value& json)
{
	if (json.IsNumber()) {
		return json.GetDouble();
	}
	return std::nullopt;
}

template <>
std::optional<bool> ValiderJSON<bool>(const rapidjson::Value& json)
{
	if (json.IsBool()) {
		return json.GetBool();
	}
	return std::nullopt;
}

template <>
std::optional<std::string> ValiderJSON<std::string>(const rapidjson::Value& json)
{
	if (json.IsString()) {
		return std::string{json.GetString(), json.GetStringLength()};
	}
	return std::nullopt;
}
