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

#include <thread>

#include <boost/asio/connect.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "InitializationParameters.h"

namespace ETP_NS
{
	// Echoes back all received WebSocket messages.
	// This uses the Curiously Recurring Template Pattern so that the same code works with both SSL streams and regular sockets.
	template<class Derived>
	class AbstractClientSession : public ETP_NS::AbstractSession
	{
	public:

		virtual ~AbstractClientSession() = default;

		boost::asio::io_context& getIoContext() {
			return ioc;
		}

		const std::string& getHost() const { return host; }
		const std::string& getPort() const { return port; }
		const std::string& getTarget() const { return target; }
		const std::string& getAuthorization() const { return authorization; }

		/**
		* Run the websocket and then the ETP session.
		* Everything related to this session (including the completion handlers) will operate on the same unique thread in a single event loop.
		*/
		bool run() {
			// We run the io_service off in its own thread so that it operates completely asynchronously with respect to the rest of the program.
			auto work = boost::asio::make_work_guard(ioc);
			std::thread thread([this]() {
				std::cerr << "Start IOC" << std::endl;
				this->getIoContext().run(); // Run the I/O service. The call will never return since we have used boost::asio::make_work_guard. We need to reset the worker if we want it to return.
				std::cerr << "End IOC" << std::endl;
			});

			successfulConnection = false;
			// Look up the domain name
			resolver.async_resolve(
				host,
				port,
				std::bind(
					&AbstractClientSession::on_resolve,
					std::static_pointer_cast<AbstractClientSession>(shared_from_this()),
					std::placeholders::_1,
					std::placeholders::_2));

			// Resetting the work make ioc (in a separate thread) return when there will no more be any uncomplete operations (such as a reading operation for example)
			// ioc does no more honor boost::asio::make_work_guard.
			work.reset();
			// Wait for ioc.run() (in the other thread) to return
			thread.join();

			return successfulConnection;
		}

		void on_connect(boost::system::error_code ec) {
			if (ec) {
				std::cerr << "on_connect : " << ec.message() << std::endl;
			}

#if BOOST_VERSION < 107000
			// Perform the websocket handshake
			derived().ws().async_handshake_ex(responseType,
				host + ":" + port, target,
				[&](websocket::request_type& m)
				{
					m.insert(boost::beast::http::field::sec_websocket_protocol, "etp12.energistics.org");
					m.insert(boost::beast::http::field::authorization, authorization);
					m.insert("etp-encoding", "binary");
				},
				std::bind(
					&AbstractClientSession::on_handshake,
					std::static_pointer_cast<AbstractClientSession>(shared_from_this()),
					std::placeholders::_1));
#else
			derived().ws().set_option(websocket::stream_base::decorator(
				[&](websocket::request_type& m)
				{
					m.insert(boost::beast::http::field::sec_websocket_protocol, "etp12.energistics.org");
					m.insert(boost::beast::http::field::authorization, authorization);
				})
			);
			// Perform the websocket handshake
			derived().ws().async_handshake(responseType,
				host + ":" + port, target,
				std::bind(
					&AbstractClientSession::on_handshake,
					std::static_pointer_cast<AbstractClientSession>(shared_from_this()),
					std::placeholders::_1));
#endif
		}

		void do_write() {
			if (sendingQueue.empty()) {
				std::cout << "*************************************************" << std::endl;
				std::cout << "The sending queue is empty." << std::endl;
				std::cout << "*************************************************" << std::endl;
				return;
			}

			if (specificProtocolHandlers.size() == maxSentAndNonRespondedMessageCount) {
				std::cout << "*************************************************" << std::endl;
				std::cout << "Cannot send Message id : " << std::get<0>(sendingQueue.front()) << " because the max number of setn and non processed message has been reached." << std::endl;
				std::cout << "*************************************************" << std::endl;
				return;
			}

			bool previousSentMessageCompleted = specificProtocolHandlers.find(std::get<0>(sendingQueue.front())) == specificProtocolHandlers.end();

			if (!previousSentMessageCompleted) {
				std::cout << "*************************************************" << std::endl;
				std::cout << "Cannot send Message id : " << std::get<0>(sendingQueue.front()) << " because the previous messgae has not finished to be sent." << std::endl;
				std::cout << "*************************************************" << std::endl;
			}
			else {
				std::cout << "*************************************************" << std::endl;
				std::cout << "Sending Message id : " << std::get<0>(sendingQueue.front()) << std::endl;
				std::cout << "*************************************************" << std::endl;

				derived().ws().async_write(
					boost::asio::buffer(std::get<1>(sendingQueue.front())),
					std::bind(
						&AbstractSession::on_write,
						shared_from_this(),
						std::placeholders::_1,
						std::placeholders::_2));

				// Register the handler to respond to the sent message
				specificProtocolHandlers[std::get<0>(sendingQueue.front())] = std::get<2>(sendingQueue.front());
			}
		}

		/**
		* Close the Websocket Session
		* The ETP session had to be closed before.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void do_close() {
			derived().ws().async_close(websocket::close_code::normal,
				std::bind(
					&AbstractSession::on_close,
					shared_from_this(),
					std::placeholders::_1));
		}
		
		FETPAPI_DLL_IMPORT_OR_EXPORT void do_read()
		{
			if (webSocketSessionClosed) {
				std::cout << "CLOSED : NOTHING MORE TO DO" << std::endl;
				return;
			}

			// Read a message into our buffer
			derived().ws().async_read(
				receivedBuffer,
				std::bind(
					&AbstractSession::on_read,
					shared_from_this(),
					std::placeholders::_1,
					std::placeholders::_2));
		}
		
		virtual void on_resolve(boost::system::error_code ec, tcp::resolver::results_type results) = 0;

		void on_handshake(boost::system::error_code ec)
		{
#ifndef NDEBUG
			std::cout << responseType << std::endl;
#endif

			if (ec) {
				std::cerr << "on_handshake : " << ec.message() << std::endl;
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

		void setMaxWebSocketMessagePayloadSize(int64_t value) final {
			maxWebSocketMessagePayloadSize = value;
			derived().ws().read_message_max(value);
		}

	protected:
		boost::asio::io_context ioc;
		tcp::resolver resolver;
		std::string host;
		std::string port;
		std::string target;
		std::string authorization;
		websocket::response_type responseType; // In order to check handshake sec_websocket_protocol
		Energistics::Etp::v12::Protocol::Core::RequestSession requestSession;
		bool successfulConnection;

		// Access the derived class, this is part of the Curiously Recurring Template Pattern idiom.
		Derived& derived() { return static_cast<Derived&>(*this); }

		AbstractClientSession() :
			ioc(4),
			resolver(ioc),
			host(),
			port(),
			target(),
			authorization(),
			successfulConnection(false) {
			messageId = 2; // The client side of the connection MUST use ONLY non-zero even-numbered messageIds. 
		}

		/**
		 * @param initializationParams  The initialization parameters of the session including IP host, port, requestedProtocols, supportedDataObjects
		 * @param target				usually "/" but a server can decide to serve etp on a particular target
		 * @param authorization			The HTTP authorization attribute to send to the server. It may be empty if not needed.
		 */
		AbstractClientSession(
			InitializationParameters* initializationParams, const std::string & target, const std::string & authorization) :
			ioc(4),
			resolver(ioc),
			host(initializationParams->getHost()),
			port(std::to_string(initializationParams->getPort())),
			target(target),
			authorization(authorization),
			successfulConnection(false)
		{
			messageId = 2; // The client side of the connection MUST use ONLY non-zero even-numbered messageIds. 

			initializationParams->postSessionCreationOperation(this);

			// Build the request session
			requestSession.applicationName = initializationParams->getApplicationName();
			requestSession.applicationVersion = initializationParams->getApplicationVersion();

			boost::uuids::random_generator gen;
			auto instanceUuid = gen();
			std::copy(std::begin(initializationParams->getInstanceId().data), std::end(initializationParams->getInstanceId().data), requestSession.clientInstanceId.array.begin());

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
