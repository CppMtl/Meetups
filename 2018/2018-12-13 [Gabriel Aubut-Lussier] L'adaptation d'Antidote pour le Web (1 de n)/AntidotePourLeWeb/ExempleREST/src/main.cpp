#include "Address.h"
#include "CustomerInfo.h"
#include <iostream>
#include <optional>
#include "REST.hpp"
#include "SingleHandlerServer.h"
#include <string>
#include <tuple>

template <>
std::optional<Address> ValiderQueryString<Address>(const QueryString& queryString)
{
	auto numberOpt = ValiderQueryString<int>(queryString, "number");
	auto streetOpt = ValiderQueryString<std::string>(queryString, "street");
	if (numberOpt && streetOpt) {
		return Address{*numberOpt, *streetOpt};
	}
	return std::nullopt;
}

template <>
std::optional<CustomerInfo> ValiderQueryString<CustomerInfo>(const QueryString& queryString)
{
	auto firstNameOpt = ValiderQueryString<std::string>(queryString, "firstName");
	auto lastNameOpt = ValiderQueryString<std::string>(queryString, "lastName");
	auto addressOpt = ValiderQueryString<Address>(queryString, "address", ValideurQueryString<Address>{});
	if (firstNameOpt && lastNameOpt && addressOpt) {
		return CustomerInfo{*firstNameOpt, *lastNameOpt, *addressOpt};
	}
	return std::nullopt;
}

template <>
std::optional<Address> ValiderJSON<Address>(const rapidjson::Value& json)
{
	auto numberOpt = ValiderJSON<int>(json, "number");
	auto streetOpt = ValiderJSON<std::string>(json, "street");
	if (numberOpt && streetOpt) {
		return Address{*numberOpt, *streetOpt};
	}
	return std::nullopt;
}

template <>
std::optional<CustomerInfo> ValiderJSON<CustomerInfo>(const rapidjson::Value& json)
{
	auto firstNameOpt = ValiderJSON<std::string>(json, "firstName");
	auto lastNameOpt = ValiderJSON<std::string>(json, "lastName");
	auto addressOpt = ValiderJSON<Address>(json, "address");
	if (firstNameOpt && lastNameOpt && addressOpt) {
		return CustomerInfo{*firstNameOpt, *lastNameOpt, *addressOpt};
	}
	return std::nullopt;
}

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	RequestHandler<
		OutputDesc<std::string>,
		InputDesc<boost::string_view, HeaderParam<typestring_is("host")>>,
		InputDesc<CustomerInfo, HeaderParam<typestring_is("customer")>, ValideurJSON>,
		InputDesc<CustomerInfo, BodyParam, ValideurQueryString>,
		InputDesc<boost::string_view, VerbParam>,
		InputDesc<boost::string_view, PathParam>
	> reqHandler{
		[](auto p1, auto p2, auto p3, auto p4, auto p5) -> std::tuple<boost::beast::http::status, std::optional<std::string>> {
			std::cout << p1 << '\n' << p2 << '\n' << p3 << '\n' << p4 << '\n' << p5 << '\n';
			return {boost::beast::http::status::ok, "TEST\n"};
		}
	};
	std::cout << "Server is starting up... Use the following command to try it out...\n"
						<< "curl -v \"http://127.0.0.1:8080/Exemple\" -d \"firstName=Gabriel&lastName=Aubut-Lussier&address=number%3D25%26street%3DC%252B%252B%2520Montr%25C3%25A9al\" -H \"customer: {\\\"firstName\\\":\\\"Gabriel\\\",\\\"lastName\\\":\\\"Aubut-Lussier\\\",\\\"address\\\":{\\\"number\\\":25,\\\"street\\\":\\\"C++ Montréal\\\"}}\"\n";
	handleRequests(reqHandler);
}
