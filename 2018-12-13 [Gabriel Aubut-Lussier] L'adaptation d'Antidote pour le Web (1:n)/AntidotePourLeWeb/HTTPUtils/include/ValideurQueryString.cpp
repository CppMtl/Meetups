#include "ValideurQueryString.hpp"
#include <boost/regex.hpp>

std::vector<std::pair<boost::string_view, boost::string_view>>
ValideurQueryStringBase::getQueryParams(boost::string_view str) const
{
	std::vector<std::pair<boost::string_view, boost::string_view>> result;
	if (!str.empty()) {
		// Parse query string
		static const boost::regex queryParamRegex(
																							"(^|&)" /*start of query or start of parameter "&"*/
																							"([^=&]*)=?" /*parameter name and "=" if value is expected*/
																							"([^=&]*)" /*parameter value*/
																							"(?=(&|$))" /*forward reference, next should be end of query or
																													 start of next parameter*/);
		const boost::cregex_iterator paramBeginItr(str.data(), str.data() + str.size(), queryParamRegex);
		boost::cregex_iterator paramEndItr;
		for (auto itr = paramBeginItr; itr != paramEndItr; ++itr) {
			if (itr->length(2) == 0) {
				// key is empty, ignore it
				continue;
			}
			result.emplace_back(
													boost::string_view{(*itr)[2].first, static_cast<boost::string_view::size_type>(std::distance((*itr)[2].first, (*itr)[2].second))},
													boost::string_view{(*itr)[3].first, static_cast<boost::string_view::size_type>(std::distance((*itr)[3].first, (*itr)[3].second))});
		}
	}
	return result;
}
