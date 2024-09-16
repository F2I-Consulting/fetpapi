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
	private:
		void initFromUrl(const std::string& etpUrl, const std::string& proxyUrl);

	protected:
		boost::uuids::uuid identifier;
		std::string etpServerHost;
		uint16_t etpServerPort;
		std::string etpServerUrlPath;
		std::string proxyHost;
		uint16_t proxyPort;
		std::map<std::string, std::string> additionalHandshakeHeaderFields;
		std::string additionalCertificates;
		bool forceTls = false;

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
			const std::string& etpServerUrl, const std::string& proxyUrl = "")
		{
			initFromUrl(etpServerUrl, proxyUrl);
			identifier = instanceUuid;
		}

		/**
		* Mainly for use with SWIG i.e. boost uuid structure is not easily portable whereas strings are.
		* @param instanceUuid	The UUID of the client instance.
		* @param etpServerUrl	Must follow the syntax ws://<host>:<port>/<path> or wss://<host>:<port>/<path> or simply <host>:<port>/<path>
		*						where port is optional and is defaulted to 80 if scheme is "ws" or if no scheme is provided.
		*						In "wss" schema cases, port is defaulted to 443.
		* @param proxyUrl		The proxy URL. It must follow the syntax http://<host>:<port> or simply <host>:<port>.
		*						Leave it empty if your connection to eptServerUrl is direct and does not pass throughr any proxy.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT InitializationParameters(const std::string& instanceUuid,
			const std::string& etpServerUrl, const std::string& proxyUrl = "")
		{
			initFromUrl(etpServerUrl, proxyUrl);
			std::stringstream ss(instanceUuid);
			ss >> identifier;
		}

		/**
		* Only to be used for direct connection to the ETP server URL (not whenpassing through a proxy)
		* @param instanceUuid	The UUID of the client or server instance.
		* @param host			The fully qualified domain name of a network host, or its IP address as a set of four decimal digit groups separated by ".".
		* @param port			The port number to connect to.
		* @param urlPath		The rest of the locator consists of data specific to the scheme, and is known as the "url-path".
		*						It supplies the details of how the specified resource can be accessed.
		*						Note that the "/" between the host (or port) and the url-path is NOT part of the url-path.
		*/
		InitializationParameters(boost::uuids::uuid instanceUuid,
			const std::string& host, uint16_t port, const std::string& urlPath = "") :
			identifier(instanceUuid), etpServerHost(host), etpServerPort(port), etpServerUrlPath(urlPath)
		{}
		
		/**
		* Only to be used for direct connection to the ETP server URL (not whenpassing through a proxy)
		* Mainly for use with SWIG i.e. boost uuid structure is not easily portable whereas strings are.
		*
		* @param instanceUuid	The UUID of the client or server instance.
		* @param host			The fully qualified domain name of a network host, or its IP address as a set of four decimal digit groups separated by ".".
		* @param port			The port number to connect to.
		* @param urlPath		The rest of the locator consists of data specific to the scheme, and is known as the "url-path".
		*						It supplies the details of how the specified resource can be accessed.
		*						Note that the "/" between the host (or port) and the url-path is NOT part of the url-path.
		*/
		InitializationParameters(const std::string & instanceUuid,
			const std::string& host, uint16_t port, const std::string& urlPath = "") :
			etpServerHost(host), etpServerPort(port), etpServerUrlPath(urlPath)
		{
			std::stringstream ss(instanceUuid);
			ss >> identifier;
		}

		virtual ~InitializationParameters() = default;

		FETPAPI_DLL_IMPORT_OR_EXPORT void setMaxWebSocketMessagePayloadSize(uint64_t value) { maxWebSocketMessagePayloadSize = value; }
		FETPAPI_DLL_IMPORT_OR_EXPORT uint64_t getMaxWebSocketMessagePayloadSize() const { return maxWebSocketMessagePayloadSize; }

		FETPAPI_DLL_IMPORT_OR_EXPORT void setPreferredMaxFrameSize(uint64_t value) { preferredMaxFrameSize = value; }
		FETPAPI_DLL_IMPORT_OR_EXPORT uint64_t getPreferredMaxFrameSize() const { return preferredMaxFrameSize; }

		FETPAPI_DLL_IMPORT_OR_EXPORT void setAdditionalHandshakeHeaderFields(const std::map<std::string, std::string>& extraHandshakeHeaderFields)
			{ this->additionalHandshakeHeaderFields = extraHandshakeHeaderFields; }
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::map<std::string, std::string>& getAdditionalHandshakeHeaderFields() const { return additionalHandshakeHeaderFields; }

		FETPAPI_DLL_IMPORT_OR_EXPORT void setAdditionalCertificates(const std::string& extraCertificates) { this->additionalCertificates = extraCertificates; }
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::string& getAdditionalCertificates() const { return additionalCertificates; }

		FETPAPI_DLL_IMPORT_OR_EXPORT const boost::uuids::uuid& getInstanceId() const { return identifier; }
		
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::string& getEtpServerHost() const { return etpServerHost; }
		FETPAPI_DLL_IMPORT_OR_EXPORT uint16_t getEtpServerPort() const { return etpServerPort; }
		FETPAPI_DLL_IMPORT_OR_EXPORT const std::string& getEtpServerUrlPath() const { return etpServerUrlPath; }

		FETPAPI_DLL_IMPORT_OR_EXPORT const std::string& getProxyHost() const { return proxyHost; }
		FETPAPI_DLL_IMPORT_OR_EXPORT uint16_t getProxyPort() const { return proxyPort; }

		FETPAPI_DLL_IMPORT_OR_EXPORT void setForceTls(bool force) { forceTls = force; }
		FETPAPI_DLL_IMPORT_OR_EXPORT bool isTlsForced() const { return forceTls; }

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
