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
		void run() {
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

			if (!isCloseRequested_) {
				// Try to reconnect up to 10 times
				if (reconnectionTryCount_ < maxReconnectionTryCount_) {
					++reconnectionTryCount_;
					std::cerr << "Session " << getIdentifier() << " has been disconnected, trying to reconnect... " << reconnectionTryCount_ << "/" << maxReconnectionTryCount_ << std::endl;
					getIoContext().restart();
					run();
				}
				else {
					std::cerr << "Could not reconnect after " << maxReconnectionTryCount_ << " retries... Give up and close" << reconnectionTryCount_ << "/" << maxReconnectionTryCount_ << std::endl;
					isCloseRequested_ = true;
				}
			}
		}
		
		void on_resolve(boost::system::error_code ec, tcp::resolver::results_type results) {
			if (ec) {
				std::cerr << "on_resolve : " << ec.message() << std::endl;
				return;
			}

			asyncConnect(results);
		}

		virtual void asyncConnect(const tcp::resolver::results_type& results) = 0;

		virtual bool isTls() const = 0;

		void on_handshake(boost::system::error_code ec)
		{
			if (ec) {
				std::cerr << "on WS handshake, error code number : " << ec.value() << std::endl;
				std::cerr << "on WS handshake, error message : " << ec.message() << std::endl;
				std::cerr << "on WS handshake, error category : " << ec.category().name() << std::endl;
				std::cerr << "Sometimes some ETP server require a trailing slash at the end of their URL. Did you also check your optional \"data-partition-id\" additional Header Field? Has your token expired?" << std::endl;
				return;
			}

			if (!responseType.count(boost::beast::http::field::sec_websocket_protocol) ||
				responseType[boost::beast::http::field::sec_websocket_protocol] != "etp12.energistics.org")
				std::cerr << "The client MUST specify the Sec-Websocket-Protocol header value of etp12.energistics.org, and the server MUST reply with the same" << std::endl;

			fesapi_log("Now connected to Websocket");
			webSocketSessionClosed = false;

			if (reconnectionTryCount_ > 0) {
				auto resumeSession = std::make_shared<Energistics::Etp::v12::Protocol::CoreOSDU::ResumeSession>();
				resumeSession->applicationName = requestSession->applicationName;
				resumeSession->applicationVersion = requestSession->applicationVersion;
				resumeSession->clientInstanceId = requestSession->clientInstanceId;
				std::copy(identifier.begin(), identifier.end(), resumeSession->sessionId.array.begin());
				send(resumeSession, 0, 0x02);
			}
			else {
				send(requestSession, 0, 0x02);
			}
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
		std::shared_ptr<Energistics::Etp::v12::Protocol::Core::RequestSession> requestSession;

		/**
		 * @param initializationParams  The initialization parameters of the session including IP host, port, requestedProtocols, supportedDataObjects
		 * @param etpServerAuth			The HTTP authorization attribute to send to the ETP server. It may be empty if not needed.
		 * @param proxyAuth				The HTTP authorization attribute to send to the proxy server. It may be empty if not needed.
		 */
		ClientSession(
			InitializationParameters const* initializationParams, const std::string& etpServerAuth, const std::string& proxyAuth = "") :
			ioc(),
			resolver(ioc),
			etpServerHost(initializationParams->getEtpServerHost()),
			etpServerPort(std::to_string(initializationParams->getEtpServerPort())),
			etpServerTarget(initializationParams->getEtpServerUrlPath()),
			etpServerAuthorization(etpServerAuth),
			proxyHost(initializationParams->getProxyHost()),
			proxyPort(std::to_string(initializationParams->getProxyPort())),
			proxyAuthorization(proxyAuth)
		{
			messageId = 2; // The client side of the connection MUST use ONLY non-zero even-numbered messageIds. 

			setCoreProtocolHandlers(std::make_shared<CoreHandlers>(this));
			setDiscoveryProtocolHandlers(std::make_shared<DiscoveryHandlers>(this));
			setStoreProtocolHandlers(std::make_shared<StoreHandlers>(this));
			setStoreNotificationProtocolHandlers(std::make_shared<StoreNotificationHandlers>(this));
			setDataArrayProtocolHandlers(std::make_shared<DataArrayHandlers>(this));
			setTransactionProtocolHandlers(std::make_shared<TransactionHandlers>(this));
			setDataspaceProtocolHandlers(std::make_shared<DataspaceHandlers>(this));
			setCoreOSDUProtocolHandlers(std::make_shared<CoreOSDUHandlers>(this));
			setStoreOSDUProtocolHandlers(std::make_shared<StoreOSDUHandlers>(this));
			setDataspaceOSDUProtocolHandlers(std::make_shared<DataspaceOSDUHandlers>(this));

			// Build the request session
			requestSession = std::make_shared<Energistics::Etp::v12::Protocol::Core::RequestSession>();
			requestSession->applicationName = initializationParams->getApplicationName();
			requestSession->applicationVersion = initializationParams->getApplicationVersion();

			std::copy(initializationParams->getInstanceId().begin(), initializationParams->getInstanceId().end(), requestSession->clientInstanceId.array.begin());

			requestSession->requestedProtocols = initializationParams->getSupportedProtocols();
			requestSession->supportedDataObjects = initializationParams->getSupportedDataObjects();
			requestSession->supportedFormats.push_back("xml");
			requestSession->currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			auto caps = initializationParams->makeEndpointCapabilities();
			if (!caps.empty()) {
				requestSession->endpointCapabilities = caps;
			}

			maxWebSocketMessagePayloadSize = initializationParams->getMaxWebSocketMessagePayloadSize();
		}
	};
}
