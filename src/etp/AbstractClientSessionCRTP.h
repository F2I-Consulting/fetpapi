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

#include "ClientSession.h"

namespace ETP_NS
{
	// This uses the Curiously Recurring Template Pattern so that the same code works with both SSL streams and regular sockets.
	template<class Derived>
	class AbstractClientSessionCRTP : public ETP_NS::ClientSession
	{
	public:

		virtual ~AbstractClientSessionCRTP() = default;

		void on_connect(boost::system::error_code ec) {
			if (ec) {
				std::cerr << "on_connect : " << ec.message() << std::endl;
			}

#if BOOST_VERSION < 107000
			// Perform the websocket handshake
			derived().ws().async_handshake_ex(responseType,
				etpServerHost + ":" + etpServerPort, etpServerTarget,
				[&](websocket::request_type& m)
				{
					m.insert(boost::beast::http::field::sec_websocket_protocol, "etp12.energistics.org");
					m.insert(boost::beast::http::field::authorization, etpServerAuthorization);
					if (!proxyHost.empty() && !isTls()) {
						m.insert(boost::beast::http::field::proxy_authorization, proxyAuthorization);
					}
					m.insert("etp-encoding", "binary");
					for (const auto& mapEntry : additionalHandshakeHeaderFields_) {
						m.insert(mapEntry.first, mapEntry.second);
					}
				},
				std::bind(
					&AbstractClientSessionCRTP::on_handshake,
					std::static_pointer_cast<AbstractClientSessionCRTP>(shared_from_this()),
					std::placeholders::_1));
#else
			derived().ws().set_option(websocket::stream_base::decorator(
				[&](websocket::request_type& m)
				{
					m.insert(boost::beast::http::field::sec_websocket_protocol, "etp12.energistics.org");
					m.insert(boost::beast::http::field::authorization, etpServerAuthorization);
					if (!proxyHost.empty() && !isTls()) {
						m.insert(boost::beast::http::field::proxy_authorization, proxyAuthorization);
					}
					m.insert("etp-encoding", "binary");
					for (const auto& mapEntry : additionalHandshakeHeaderFields_) {
						m.insert(mapEntry.first, mapEntry.second);
					}
				})
			);
			// Perform the websocket handshake
			derived().ws().async_handshake(responseType,
				etpServerHost + ":" + etpServerPort, etpServerTarget,
				std::bind(
					&AbstractClientSessionCRTP::on_handshake,
					std::static_pointer_cast<AbstractClientSessionCRTP>(shared_from_this()),
					std::placeholders::_1));
#endif
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
				fesapi_log("CLOSED : NOTHING MORE TO DO");
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

		void setMaxWebSocketMessagePayloadSize(int64_t value) final {
			maxWebSocketMessagePayloadSize = value;
			derived().ws().read_message_max(value);
		}

	protected:
		using ClientSession::ClientSession;

		// Access the derived class, this is part of the Curiously Recurring Template Pattern idiom.
		Derived& derived() { return static_cast<Derived&>(*this); }

		void do_write() {
			const std::lock_guard<std::mutex> specificProtocolHandlersLock(specificProtocolHandlersMutex);
			if (sendingQueue.empty()) {
				fesapi_log("The sending queue is empty.");
				return;
			}

			bool previousSentMessageCompleted = specificProtocolHandlers.find(std::get<0>(sendingQueue.front())) == specificProtocolHandlers.end();

			if (!previousSentMessageCompleted) {
				fesapi_log("Cannot send Message id :", std::to_string(std::get<0>(sendingQueue.front())), "because the previous message has not finished to be sent.");
			}
			else {
				fesapi_log("Sending Message id :", std::to_string(std::get<0>(sendingQueue.front())));

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
	};
}
