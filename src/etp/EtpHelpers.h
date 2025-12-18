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
#pragma once

#include "../macroDefinitions.h"
#include "EtpMessages.h"

#if defined(_WIN32) && !defined(FETPAPI_STATIC)
	#ifndef FETPAPI_DLL_IMPORT_OR_EXPORT
		#if defined(Fetpapi_EXPORTS)
			#define FETPAPI_DLL_IMPORT_OR_EXPORT __declspec(dllexport)
		#else
			#define FETPAPI_DLL_IMPORT_OR_EXPORT __declspec(dllimport)
		#endif
	#endif
#else
	#define FETPAPI_DLL_IMPORT_OR_EXPORT
#endif

namespace COMMON_NS
{
	class AbstractObject;
}

namespace ETP_NS
{

	namespace EtpHelpers {

		/**
		* Get the dataobject type from a canonical ETP 1.2 URI.
		*
		* @param uri	The input canonical ETP 1.2 URI
		* @return		The URI dataobject type or the empty string if the operation cannot succeed
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::string getDataObjectType(const std::string& uri);

		/**
		* Get the dataspace URI from a canonical ETP 1.2 URI.
		*
		* @param uri	The input canonical ETP 1.2 URI
		* @return		The dataspace URI or the empty string if the operation cannot succeed
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::string getDataspaceUri(const std::string& uri);

		/**
		* Validate an ETP URI
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT bool validateUri(const std::string& uri);

		/**
		* Validate an ETP dataobject URI
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT bool validateDataObjectUri(const std::string& uri);

		/**
		* Build a protocol exception message which only contains a single error message (not a messsage map).
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT Energistics::Etp::v12::Protocol::Core::ProtocolException buildSingleMessageProtocolException(int32_t m_code, const std::string & m_message);

		/**
		* Extract and return the uuid and the version of a dataobject based on its URI.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::pair<std::string, std::string> getUuidAndVersionFromUri(const std::string & uri);
	}
}