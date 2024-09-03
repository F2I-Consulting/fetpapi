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

#include "AbstractSession.h"

#include "InitializationParameters.h"

namespace ETP_NS
{
	class ClientSession : public ETP_NS::AbstractSession
	{
	public:

		virtual ~ClientSession() = default;

		boost::asio::io_context& getIoContext() {
			return ioc;
		}

		const std::string& getEtpServerHost() const { return etpServerHost; }
		const std::string& getEtpServerPort() const { return etpServerPort; }
		const std::string& getEtpServerTarget() const { return etpServerTarget; }
		const std::string& getEtpServerAuthorization() const { return etpServerAuthorization; }
		const std::string& getProxyHost() const { return proxyHost; }
		const std::string& getProxyPort() const { return proxyPort; }
		const std::string& getProxyAuthorization() const { return proxyAuthorization; }

		/**
		* Run the websocket and then the ETP session in a processing loop.
		* Everything related to this session (including the completion handlers) will operate on the current thread in a single event loop.
		* Since this is a loop, you may want to operate this method on a dedicated thread not to block your program.
		* This method returns only when the session is closed.
		*/
		bool run() {
			successfulConnection = false;

			// Look up the domain name before to run the session
			// It is important to do this before to run the io context. Otherwise running the io context would return immediately if nothing has to be done.
			resolver.async_resolve(
				proxyHost.empty() ? etpServerHost : proxyHost,
				proxyHost.empty() ? etpServerPort : proxyPort,
				std::bind(
					&ClientSession::on_resolve,
					std::static_pointer_cast<ClientSession>(shared_from_this()),
					std::placeholders::_1,
					std::placeholders::_2));

			// Run the io_context to perform the resolver and all other binding functions
			// Run will return only when there will no more be any uncomplete operations (such as a reading operation for example)
			getIoContext().run();

			return successfulConnection;
		}
		
		virtual void on_resolve(boost::system::error_code ec, tcp::resolver::results_type results) = 0;
		virtual bool isTls() const = 0;

		void on_handshake(boost::system::error_code ec)
		{
			if (ec) {
				std::cerr << "on_handshake : " << ec.message() << std::endl;
				std::cerr << "Sometimes some ETP server require a trailing slash at the end of their URL. Did you also check your optional \"data-partition-id\" additional Header Field?" << std::endl;
				return;
			}

			if (!responseType.count(boost::beast::http::field::sec_websocket_protocol) ||
				responseType[boost::beast::http::field::sec_websocket_protocol] != "etp12.energistics.org")
				std::cerr << "The client MUST specify the Sec-Websocket-Protocol header value of etp12.energistics.org, and the server MUST reply with the same" << std::endl;

			successfulConnection = true;
			webSocketSessionClosed = false;

			send(requestSession, 0, 0x02);
			do_read();
		}

	protected:
		boost::asio::io_context ioc;
		tcp::resolver resolver;
		std::string etpServerHost;
		std::string etpServerPort;
		std::string etpServerTarget;
		std::string etpServerAuthorization;
		std::string proxyHost;
		std::string proxyPort;
		std::string proxyAuthorization;
		std::map<std::string, std::string> additionalHandshakeHeaderFields_;
		websocket::response_type responseType; // In order to check handshake sec_websocket_protocol
		Energistics::Etp::v12::Protocol::Core::RequestSession requestSession;
		bool successfulConnection = false;

		/**
		 * @param initializationParams  The initialization parameters of the session including IP host, port, requestedProtocols, supportedDataObjects
		 * @param target				usually "/" but a server can decide to serve etp on a particular target
		 * @param etpServerAuth			The HTTP authorization attribute to send to the ETP server. It may be empty if not needed.
		 * @param proxyAuth				The HTTP authorization attribute to send to the proxy server. It may be empty if not needed.
		 */
		ClientSession(
			InitializationParameters const* initializationParams, const std::string& target, const std::string& etpServerAuth, const std::string& proxyAuth = "") :
			ioc(4),
			resolver(ioc),
			etpServerHost(initializationParams->getEtpServerHost()),
			etpServerPort(std::to_string(initializationParams->getEtpServerPort())),
			etpServerTarget(target),
			etpServerAuthorization(etpServerAuth),
			proxyHost(initializationParams->getProxyHost()),
			proxyPort(std::to_string(initializationParams->getProxyPort())),
			proxyAuthorization(proxyAuth)
		{
			messageId = 2; // The client side of the connection MUST use ONLY non-zero even-numbered messageIds. 

			initializationParams->postSessionCreationOperation(this);

			// Build the request session
			requestSession.applicationName = initializationParams->getApplicationName();
			requestSession.applicationVersion = initializationParams->getApplicationVersion();

			std::copy(initializationParams->getInstanceId().begin(), initializationParams->getInstanceId().end(), requestSession.clientInstanceId.array.begin());

			requestSession.requestedProtocols = initializationParams->makeSupportedProtocols();
			requestSession.supportedDataObjects = initializationParams->makeSupportedDataObjects();
			requestSession.supportedFormats.push_back("xml");
			requestSession.currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			auto caps = initializationParams->makeEndpointCapabilities();
			if (!caps.empty()) {
				requestSession.endpointCapabilities = caps;
			}

			maxWebSocketMessagePayloadSize = initializationParams->getMaxWebSocketMessagePayloadSize();
		}
	};
}
