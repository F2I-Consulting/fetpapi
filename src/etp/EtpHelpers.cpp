/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
#include "EtpHelpers.h"

#include <regex>

#include "EtpException.h"

std::string ETP_NS::EtpHelpers::getDataObjectType(const std::string& uri)
{
	if (!ETP_NS::EtpHelpers::validateDataObjectUri(uri)) {
		throw ETP_NS::EtpException(9, "The dataobject ETP URI \"" + uri + "\" is invalid.");
	}

	const std::size_t lastSlash = uri.find_last_of("/");
	const std::size_t lastOpenParenthesis = uri.find_last_of("(");
	return lastSlash != std::string::npos && lastOpenParenthesis != std::string::npos
		? uri.substr(lastSlash + 1, lastOpenParenthesis - lastSlash - 1)
		: "";
}

std::string ETP_NS::EtpHelpers::getDataspaceUri(const std::string& uri)
{
	if (!ETP_NS::EtpHelpers::validateUri(uri)) {
		throw ETP_NS::EtpException(9, "The ETP URI \"" + uri + "\" is invalid.");
	}

	const size_t dataspacePos = uri.find("dataspace('");
	if (dataspacePos == std::string::npos) {
		return "eml:///";
	}

	const size_t closingParenthesisPos = uri.find(')', dataspacePos);
	if (closingParenthesisPos == std::string::npos) {
		throw std::invalid_argument("The uri " + uri + " is malformed because it does not contain a closing parenthesis after the dataspace path.");
	}
	return uri.substr(0, closingParenthesisPos+1);
}

bool ETP_NS::EtpHelpers::validateUri(const std::string & uri)
{
	// Regular expressions are not handled before GCC 4.9
	// https://stackoverflow.com/questions/12530406/is-gcc-4-8-or-earlier-buggy-about-regular-expressions
	return
		std::regex_match(uri, std::regex("^eml:///(dataspace[(]'.*'[)])?", std::regex::ECMAScript)) ||
		std::regex_match(uri, std::regex("^eml:///(dataspace[(]'.*'[)]/)?(resqml20|eml20)\.obj_[a-zA-Z0-9]+[(][a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}(,.*)?[)]", std::regex::ECMAScript)) ||
		std::regex_match(uri, std::regex("^eml:///(dataspace[(]'.*'[)]/)?(witsml|resqml|prodml|eml)([0-9]{2})\[a-zA-Z0-9]+[(][a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}(,.*)?[)]", std::regex::ECMAScript));
}

bool ETP_NS::EtpHelpers::validateDataObjectUri(const std::string & uri)
{
	// Regular expressions are not handled before GCC 4.9
	// https://stackoverflow.com/questions/12530406/is-gcc-4-8-or-earlier-buggy-about-regular-expressions
	return (uri.find("resqml20") != std::string::npos || uri.find("eml20") != std::string::npos)
		? std::regex_match(uri, std::regex("^eml:///(dataspace[(]'.*'[)]/)?(resqml20|eml20)\.obj_[a-zA-Z0-9]+[(][a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}(,.*)?[)]", std::regex::ECMAScript))
		: std::regex_match(uri, std::regex("^eml:///(dataspace[(]'.*'[)]/)?(witsml|resqml|prodml|eml)([0-9]{2})\.[a-zA-Z0-9]+[(][a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}(,.*)?[)]", std::regex::ECMAScript));
}

Energistics::Etp::v12::Protocol::Core::ProtocolException ETP_NS::EtpHelpers::buildSingleMessageProtocolException(int32_t m_code, const std::string & m_message)
{
	Energistics::Etp::v12::Datatypes::ErrorInfo errorInfo;
	errorInfo.code = m_code;
	errorInfo.message = m_message;
	Energistics::Etp::v12::Protocol::Core::ProtocolException peMessage;
	peMessage.error.emplace(errorInfo);

	return peMessage;
}

std::pair<std::string, std::string> ETP_NS::EtpHelpers::getUuidAndVersionFromUri(const std::string & uri)
{
	std::pair<std::string, std::string> result;

	if (!ETP_NS::EtpHelpers::validateDataObjectUri(uri)) {
		throw ETP_NS::EtpException(9, "Invalid URI");
	}

	// old code to be updated
	if (uri[6] != '/') {
		throw ETP_NS::EtpException(2, "The URI " + uri + " uses some dataspaces. This agent does not support dataspace.");
	}

	const size_t openingParenthesisPos = uri.find('(');
	const std::string uuid = uri.substr(openingParenthesisPos + 1, 36);
	const size_t comma = uri.find(',');
	const size_t closingParenthesisPos = comma == std::string::npos ? std::string::npos : uri.find(')');
	const std::string version = (comma == std::string::npos || closingParenthesisPos == std::string::npos) ? "" : uri.substr(comma + 1, closingParenthesisPos - comma - 1);

	return std::pair<std::string, std::string>(uuid, version);
}
