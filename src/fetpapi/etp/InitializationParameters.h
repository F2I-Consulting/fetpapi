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

namespace ETP_NS
{
	class InitializationParameters
	{
	public:

		/**
		* @param instanceUuid	The UUID of the client instance.
		* @param etpServerUrl	Must follow the syntax ws://<host>:<port>/<path> or wss://<host>:<port>/<path> or simply <host>:<port>/<path>
		*						where port is optional and is defaulted to 80 if scheme is "ws" or if no scheme is provided.
		*						In "wss" schema cases, port is defaulted to 443.
		* @param proxyUrl		The proxy URL. It must follow the syntax http://<host>:<port> or simply <host>:<port>.
		*						Leave it empty if your connection to eptServerUrl is direct and does not pass throughr any proxy.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT InitializationParameters(boost::uuids::uuid instanceUuid,
			const std::string& etpServerUrl, const std::string& proxyUrl = "");

		/**
		* Mainly for use with SWIG i.e. boost uuid structure is not easily portable whereas strings are.
		* @param instanceUuid	The UUID of the client instance.
		* @param etpServerUrl	Must follow the syntax ws://<host>:<port>/<path> or wss://<host>:<port>/<path> or simply <host>:<port>/<path>
		*						where port is optional and is defaulted to 80 if scheme is "ws" or if no scheme is provided.
		*						In "wss" schema cases, port is defaulted to 443.
		* @param proxyUrl		The proxy URL. It must follow the syntax http://<host>:<port> or simply <host>:<port>.
		*						Leave it empty if your connection to eptServerUrl is direct and does not pass through any proxy.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT InitializationParameters(const std::string& instanceUuid,
			const std::string& etpServerUrl, const std::string& proxyUrl = "");

		/**
		* Only to be used for direct connection to the ETP server URL (not whenpassing through a proxy)
		* @param instanceUuid	The UUID of the client or server instance.
		* @param host			The fully qualified domain name of a network host, or its IP address as a set of four decimal digit groups separated by ".".
		* @param port			The port number to connect to.
		* @param urlPath		The rest of the locator consists of data specific to the scheme, and is known as the "url-path".
		*						It supplies the details of how the specified resource can be accessed.
		*						It must start with a slash or be empty.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT InitializationParameters(boost::uuids::uuid instanceUuid,
			const std::string& host, uint16_t port, const std::string& urlPath = "");
		
		/**
		* Only to be used for direct connection to the ETP server URL (not whenpassing through a proxy)
		* Mainly for use with SWIG i.e. boost uuid structure is not easily portable whereas strings are.
		*
		* @param instanceUuid	The UUID of the client or server instance.
		* @param host			The fully qualified domain name of a network host, or its IP address as a set of four decimal digit groups separated by ".".
		* @param port			The port number to connect to.
		* @param urlPath		The rest of the locator consists of data specific to the scheme, and is known as the "url-path".
		*						It supplies the details of how the specified resource can be accessed.
		*						It must start with a slash or be empty.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT InitializationParameters(const std::string& instanceUuid,
			const std::string& host, uint16_t port, const std::string& urlPath = "");

		~InitializationParameters() = default;

		FETPAPI_DLL_IMPORT_OR_EXPORT void setMaxWebSocketMessagePayloadSize(uint64_t value) { maxWebSocketMessagePayloadSize = value; }
		FETPAPI_DLL_IMPORT_OR_EXPORT uint64_t getMaxWebSocketMessagePayloadSize() const { return maxWebSocketMessagePayloadSize; }

		FETPAPI_DLL_IMPORT_OR_EXPORT void setPreferredMaxFrameSize(uint64_t value) { preferredMaxFrameSize = value; }
		FETPAPI_DLL_IMPORT_OR_EXPORT uint64_t getPreferredMaxFrameSize() const { return preferredMaxFrameSize; }

		FETPAPI_DLL_IMPORT_OR_EXPORT void setAdditionalHandshakeHeaderFields(const std::map<std::string, std::string>& extraHandshakeHeaderFields)
			{ this->additionalHandshakeHeaderFields = extraHandshakeHeaderFields; }
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::map<std::string, std::string>& getAdditionalHandshakeHeaderFields() const { return additionalHandshakeHeaderFields; }

		FETPAPI_DLL_IMPORT_OR_EXPORT void setAdditionalCertificates(const std::string& extraCertificates) { additionalCertificates = extraCertificates; }
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::string& getAdditionalCertificates() const { return additionalCertificates; }

		/**
		* Get the UUID that a client or server assigns itself to uniquely identify the instance of itself in an ETP session
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT const boost::uuids::uuid& getInstanceId() const { return identifier; }
		
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::string& getEtpServerHost() const { return etpServerHost; }
		FETPAPI_DLL_IMPORT_OR_EXPORT uint16_t getEtpServerPort() const { return etpServerPort; }
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::string& getEtpServerUrlPath() const { return etpServerUrlPath; }

		FETPAPI_DLL_IMPORT_OR_EXPORT const std::string& getProxyHost() const { return proxyHost; }
		FETPAPI_DLL_IMPORT_OR_EXPORT uint16_t getProxyPort() const { return proxyPort; }

		/**
		* Allow to force TLS usage even if the connection is not done on the 443 TLS port.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void setForceTls(bool force) { forceTls = force; }
		/**
		* Check if the connection has been asked to run on a secured websocket even if not using the 443  TLS port.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT bool isTlsForced() const { return forceTls; }

		/**
		* Set the string by which the client or server identifies itself, normally a software product or system name.
		* The format is entirely application dependent. Vendors are encouraged to identify their company name as part of this string.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void setApplicationName(std::string_view appName) { applicationName = appName; }
		/**
		* Set the string by which the client or server identifies itself, normally a software product or system name.
		* The format is entirely application dependent. Vendors are encouraged to identify their company name as part of this string.
		* Remark : SWIG pre v4.2 does not support std::string_view
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void setApplicationName(const std::string& appName) { applicationName = appName; }
		/**
		* Get the string by which the client or server identifies itself, normally a software product or system name.
		* The format is entirely application dependent. Vendors are encouraged to identify their company name as part of this string.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::string& getApplicationName() const { return applicationName; }
		/**
		* Set The version of the application identified in applicationName.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void setApplicationVersion(std::string_view appVersion) { applicationVersion = appVersion; }
		/**
		* Set The version of the application identified in applicationName.
		* Remark : SWIG pre v4.2 does not support std::string_view
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void setApplicationVersion(const std::string& appVersion) { applicationVersion = appVersion; }
		/**
		* Get The version of the application identified in applicationName.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::string& getApplicationVersion() const { return applicationVersion; }

		/**
		* Set the data objects that the client wants to use in this session and the information for each.
		* Client and server use this field (in RequestSession and OpenSession messages respectively) to negotiate the objects
		* that will be used during the session and determine the data object capabilities for each.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void setSupportedDataObjects(const std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject>& dataObjects)
			{ supportedDataObjects = dataObjects; }
		/**
		* Get the data objects that the client wants to use in this session and the information for each.
		* Client and server use this field (in RequestSession and OpenSession messages respectively) to negotiate the objects
		* that will be used during the session and determine the data object capabilities for each.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject>& getSupportedDataObjects() const { return supportedDataObjects; }
		/**
		* CLIENT NOTES:
		* Set the ETP sub-protocol(s) and associated information for each sub-protocol that the client expects to communicate on for the ETP session.
		* It is an array of SupportedProtocol records, each of which identifies
		*   a sub-protocol ID,
		*   the role for the sub-protocol it expects the server to fill,
		*   and name-value pairs of protocol capabilities.
		* 1. An ETP sub-protocol MUST appear only once in this array.
		* 2. Each sub-protocol MUST specify only one role (for the server).
		* 3. Core (Protocol 0) MUST NOT be included in this list.
		* 4. Requested roles MUST be consistent across protocols in an ETP session.
		*      EXAMPLE: An endpoint CANNOT request to be customer in one protocol and store in another, in the same ETP session.
		* SERVER NOTES:
		* Set the ETP sub-protocols and associated information for each sub-protocol that the server will support in response to the client's request.
		* It is an array of SupportedProtocol records, each of which identifies
		*   the protocol IDs that the server will support for this session,
		*   the role it will use for each protocol (as assigned by the client in the RequestSession message),
		*   and key-value pairs of related capabilities.
		* 1. This array MUST be all or a subset of the protocols that the client requested in the supportedProtocols field of the RequestSession message.
		* 2. A server may be capable of supporting both roles in an ETP sub-protocol, but in any given session it MUST fill only one role
		*      (the one requested by the client in the RequestSession message).
		* 3. Core (Protocol 0) MUST NOT be included in this list.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void setSupportedProtocols(const std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol>& protocols)
			{ supportedProtocols = protocols; }
		/**
		* CLIENT NOTES:
		* Get the ETP sub-protocol(s) and associated information for each sub-protocol that the client expects to communicate on for the ETP session.
		* It is an array of SupportedProtocol records, each of which identifies
		*   a sub-protocol ID,
		*   the role for the sub-protocol it expects the server to fill,
		*   and name-value pairs of protocol capabilities.
		* 1. An ETP sub-protocol MUST appear only once in this array.
		* 2. Each sub-protocol MUST specify only one role (for the server).
		* 3. Core (Protocol 0) MUST NOT be included in this list.
		* 4. Requested roles MUST be consistent across protocols in an ETP session.
		*      EXAMPLE: An endpoint CANNOT request to be customer in one protocol and store in another, in the same ETP session.
		* SERVER NOTES:
		* Get the ETP sub-protocols and associated information for each sub-protocol that the server will support in response to the client's request.
		* It is an array of SupportedProtocol records, each of which identifies
		*   the protocol IDs that the server will support for this session,
		*   the role it will use for each protocol (as assigned by the client in the RequestSession message),
		*   and key-value pairs of related capabilities.
		* 1. This array MUST be all or a subset of the protocols that the client requested in the supportedProtocols field of the RequestSession message.
		* 2. A server may be capable of supporting both roles in an ETP sub-protocol, but in any given session it MUST fill only one role
		*      (the one requested by the client in the RequestSession message).
		* 3. Core (Protocol 0) MUST NOT be included in this list.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol>& getSupportedProtocols() const { return supportedProtocols; }
		FETPAPI_DLL_IMPORT_OR_EXPORT std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> makeEndpointCapabilities() const;

	protected:
		boost::uuids::uuid identifier;
		std::string etpServerHost;
		uint16_t etpServerPort = 0;
		std::string etpServerUrlPath;
		std::string proxyHost;
		uint16_t proxyPort = 0;
		std::map<std::string, std::string> additionalHandshakeHeaderFields;
		std::string additionalCertificates;
		bool forceTls = false;

		std::string applicationName = "F2I-CONSULTING ETP CLIENT";
		std::string applicationVersion = "0.0";
		std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> supportedDataObjects;
		std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> supportedProtocols;

		/**
		* @param preferredFrameSize The preferred websocket frame payload to use by this client.
		*							If the ETP websocket server on the other end has a lower related MaxWebSocketFramePayloadSize capability, then the used websocket frame payload will be the server one.
		*							Default value corresponds to the default Boost.Beast value : https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/ref/boost__beast__websocket__stream/write_buffer_bytes/overload1.html.
		*/
		uint64_t preferredMaxFrameSize = 4096;

		/**
		* Capabilities
		* see https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/using_websocket/messages.html
		* and https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/ref/boost__beast__websocket__stream/read_message_max/overload1.html
		*
		* The default value is 16000000 according to Boost.Beast.
		*/
		uint64_t maxWebSocketMessagePayloadSize = 16000000;

	private:
		void initFromUrl(const std::string& etpUrl, const std::string& proxyUrl);
		std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> makeSupportedDataObjects() const;
		std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> makeSupportedProtocols() const;
	};
}
