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

#include <boost/uuid/uuid_io.hpp>

#include "../nsDefinitions.h"
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

namespace ETP_NS
{
	class InitializationParameters
	{
	protected:
		boost::uuids::uuid identifier_;
		std::string host_;
		unsigned short port_;

		// Capabilities
		// https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/using_websocket/messages.html
		// and https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/ref/boost__beast__websocket__stream/read_message_max/overload1.html
		int64_t maxWebSocketMessagePayloadSize_ = 16000000;

	public:
		/**
		* Set the identifier of the server a default value.
		* You should set the identifer as wanted in your own derived class.
		*/
		InitializationParameters(boost::uuids::uuid instanceUuid, const std::string & host, unsigned short port) : identifier_(instanceUuid), host_(host), port_(port) {}
		/**
		* Mainly for use with SWIG i.e. boost uuid structure is not easily portable whereas strings are.
		*/
		InitializationParameters(const std::string & instanceUuid, const std::string & host, unsigned short port) : host_(host), port_(port) {
			std::stringstream ss(instanceUuid);
			ss >> identifier_;
		}
		virtual ~InitializationParameters() = default;

		/**
		* The default value is 16000000 according to Boost.Beast. This method allows you to modify it.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void setMaxWebSocketMessagePayloadSize(int64_t value) { maxWebSocketMessagePayloadSize_ = value; }
		int64_t getMaxWebSocketMessagePayloadSize() const { return maxWebSocketMessagePayloadSize_; }

		const boost::uuids::uuid& getInstanceId() const { return identifier_; }
		const std::string& getHost() const { return host_; }
		unsigned short getPort() const { return port_; }
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::string getApplicationName() const { return "F2I-CONSULTING ETP CLIENT"; }
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::string getApplicationVersion() const { return "0.0"; }

		FETPAPI_DLL_IMPORT_OR_EXPORT std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> makeEndpointCapabilities() const;
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> makeSupportedDataObjects() const;
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> makeSupportedProtocols() const;

		/**
		* Override this method in order to register some dedicated protocol handlers for a session.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual void postSessionCreationOperation(class AbstractSession* session) const;
	};
}
