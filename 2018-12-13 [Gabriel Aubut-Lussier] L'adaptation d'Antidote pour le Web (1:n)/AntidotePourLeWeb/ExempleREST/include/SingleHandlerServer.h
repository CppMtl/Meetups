#ifndef SINGLE_HANDLER_SERVER_H
#define SINGLE_HANDLER_SERVER_H

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/status.hpp>
#include <boost/beast/http/string_body.hpp>
#include <functional>
#include <optional>
#include <tuple>

using status_type = boost::beast::http::status;
using return_type = std::tuple<status_type, std::optional<std::string>>;
using body_type = boost::beast::http::string_body;
using request_type = boost::beast::http::request<body_type>;
using handler_type = std::function<return_type(request_type)>;

int handleRequests(handler_type);

#endif
