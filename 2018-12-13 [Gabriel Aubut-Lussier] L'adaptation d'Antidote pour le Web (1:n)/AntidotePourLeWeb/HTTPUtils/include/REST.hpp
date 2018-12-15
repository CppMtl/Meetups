#ifndef REST_HPP
#define REST_HPP

#include <boost/beast/http.hpp>
#include <optional>
#include "SerialiseurGenerique.hpp"
#include <string>
#include <tuple>
#include <type_traits>
#include "typestring.h"
#include "ValideurGenerique.hpp"
#include "ValideurJSON.hpp"
#include "ValideurQueryString.hpp"

/**
 * Résumé de la syntaxe :
 *   RequestHandler<OutputDesc, InputDesc ...>
 *   OutputDesc<ContentType, SerialiseurGenerique>
 *   InputDesc<ValueType, Source, ValideurGenerique>
 *   InputDesc<ValueType, Source, ValideurJSON>
 *   InputDesc<ValueType, Source, ValideurQueryString>
 *   Source => HeaderParam<typestring_is("host")> | BodyParam | VerbParam | PathParam
 * Exemple d'utilisation :
 *
 
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
 
 *
 */

template <typename Key>
struct HeaderParam
{
	static_assert(!std::is_same_v<Key, typestring_is("")>, "Reading from headers requires a name parameter.");
	
	template <typename BodyType>
	boost::beast::string_view operator()(const boost::beast::http::request<BodyType>& req) const
	{
		Key key;
		boost::string_view name = boost::beast::string_view{key.data(), key.size()};
		auto itEnd = req.end();
		auto it = req.find(name);
		if (it != itEnd) {
			return it->value();
		}
		return boost::beast::string_view{};
	}
};
struct PathParam
{
	template <typename BodyType>
	boost::beast::string_view operator()(const boost::beast::http::request<BodyType>& req) const
	{
			return req.target();
	}
};
struct VerbParam
{
	template <typename BodyType>
	boost::beast::string_view operator()(const boost::beast::http::request<BodyType>& req) const
	{
			return req.method_string();
	}
};
struct BodyParam
{
	template <typename BodyType>
	boost::beast::string_view operator()(const boost::beast::http::request<BodyType>& req) const
	{
			return req.body();
	}
};

namespace detail
{
	template <typename ... Inputs, typename BodyType>
	auto readInput(const boost::beast::http::request<BodyType>& req) -> std::tuple<std::optional<typename Inputs::value_type>...>
	{
		return std::make_tuple(
			typename Inputs::valideur_type{}(
				typename Inputs::source_type{}.template operator()(req)
			)...
		);
	}
	
	template <typename Output, typename Handler, typename ... Params>
	auto invokeHandlerImpl(Handler handler, const Params& ... params) -> std::tuple<boost::beast::http::status, std::optional<typename Output::value_type>>
	{
		if ((... && static_cast<bool>(params))) {
			return std::apply(handler, std::make_tuple(*params...));
		}
		return {boost::beast::http::status::bad_request, std::nullopt};
	}
	
	template <typename Output, typename Handler, typename ... Params>
	auto invokeHandler(Handler&& handler, const std::tuple<Params...>& params) -> std::tuple<boost::beast::http::status, std::optional<typename Output::value_type>>
	{
		return std::apply(&invokeHandlerImpl<Output, Handler, Params...>, std::tuple_cat(std::tuple<Handler>{std::forward<Handler>(handler)}, params));
	}
}

template <typename T, template <typename> typename Serialiseur = SerialiseurGenerique>
struct OutputDesc
{
	using value_type = T;
	using serialiseur_type = Serialiseur<T>;
};

template <typename T, typename Source, template <typename> typename Valideur = ValideurGenerique>
struct InputDesc
{
	using value_type = T;
	using source_type = Source;
	using valideur_type = Valideur<T>;
};

template <typename Output, typename ... Inputs, typename Handler, typename Request>
auto handleRequest(Request&& req, Handler&& handler) -> std::tuple<boost::beast::http::status, std::optional<typename Output::value_type>>
{
	return detail::invokeHandler<Output>(std::forward<Handler>(handler), detail::readInput<Inputs...>(std::forward<Request>(req)));
}

template <typename Output, typename ... Inputs>
struct RequestHandler
{
	using return_type = std::tuple<boost::beast::http::status, std::optional<typename Output::value_type>>;
	using handler_type = std::function<return_type(typename Inputs::value_type...)>;
	
	RequestHandler(handler_type handler) : handler(handler) {}
	
	template <typename Request>
	return_type operator()(Request&& req)
	{
		return detail::invokeHandler<Output>(handler, detail::readInput<Inputs...>(std::forward<Request>(req)));
	}
	
	handler_type handler;
};

#endif

