/*
 * Copyright 2013-present Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This file has a member function implementation coming from the Folly library
 * which was adaptated into the Valideur concept.
 */

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
