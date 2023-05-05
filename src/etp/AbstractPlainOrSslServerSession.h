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

#include <unordered_map>

#include <boost/asio/strand.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/uuid/random_generator.hpp>

#include "ServerInitializationParameters.h"

namespace ETP_NS
{
	// IMPORTANT : This class is no more maintained now that the OSDU RDDMS acts as a Reference Implementation


	// Echoes back all received WebSocket messages.
	// This uses the Curiously Recurring Template Pattern so that the same code works with both SSL streams and regular sockets.
	template<class Derived>
	class AbstractPlainOrSslServerSession : public ETP_NS::AbstractSession
	{
	private:
		// Access the derived class, this is part of the Curiously Recurring Template Pattern idiom.
		Derived& derived() { return static_cast<Derived&>(*this); }

	protected:
		boost::asio::strand<boost::asio::io_context::executor_type> strand;
		ServerInitializationParameters* serverInitializationParams_;

		void do_write() {
			if (sendingQueue.empty()) {
				fesapi_log("The sending queue is empty.");
				return;
			}

			bool previousSentMessageCompleted = specificProtocolHandlers.find(std::get<0>(sendingQueue.front())) == specificProtocolHandlers.end();

			if (!previousSentMessageCompleted) {
				fesapi_log("Cannot send Message id :", std::to_string(std::get<0>(sendingQueue.front())), " because the previous messgae has not finished to be sent.");
			}
			else {
				derived().ws().async_write(
					boost::asio::buffer(std::get<1>(sendingQueue.front())),
					boost::asio::bind_executor(
						strand,
						std::bind(
							&AbstractSession::on_write,
							shared_from_this(),
							std::placeholders::_1,
							std::placeholders::_2)));

				// Register the handler to respond to the sent message
				specificProtocolHandlers[std::get<0>(sendingQueue.front())] = std::get<2>(sendingQueue.front());
			}
		}

	public:
		AbstractPlainOrSslServerSession(boost::asio::io_context& ioc, ServerInitializationParameters* serverInitializationParams) :
			strand(ioc.get_executor()),
			serverInitializationParams_(serverInitializationParams)
		{
			messageId = 1; // The client side of the connection MUST use ONLY non-zero even-numbered messageIds.
			boost::uuids::random_generator gen;
			identifier = gen();
		}

		virtual ~AbstractPlainOrSslServerSession() = default;

		boost::asio::io_context& getIoContext() {
			return static_cast<boost::asio::io_context&> (derived().ws().get_executor().context());
		}

		ServerInitializationParameters const* getServerInitializationParameters() const { return serverInitializationParams_; }

		/**
		* The key is the UUID of the subscription.
		* The vector allows to buffer the dataobjects which we want to be notified about
		*/
		std::vector<Energistics::Etp::v12::Datatypes::Object::SubscriptionInfo> subscriptions;

		virtual bool run(boost::beast::http::request<boost::beast::http::string_body> req) = 0;

		/**
		* This method is done after ssl handshake
		* or directly if no SSL mode
		*/
		void on_handshake(boost::system::error_code ec,
			boost::beast::http::request<boost::beast::http::string_body> req)
		{
			if (ec) {
				std::cerr << "on_handshake : " << ec.message() << std::endl;
			}

#ifndef LINUX
			// Accept the websocket handshake
#if BOOST_VERSION < 107000
			derived().ws().async_accept_ex(req,
				[](websocket::response_type& m)
			{
				m.insert(boost::beast::http::field::sec_websocket_protocol, "etp12.energistics.org");
			},
#else
			derived().ws().set_option(websocket::stream_base::decorator(
				[](websocket::response_type& m)
				{
					m.insert(boost::beast::http::field::sec_websocket_protocol, "etp12.energistics.org");
				})
			);
			derived().ws().async_accept(req,
#endif
			boost::asio::bind_executor(
					strand,
					std::bind(
						&AbstractPlainOrSslServerSession::on_accept,
						std::static_pointer_cast<AbstractPlainOrSslServerSession>(shared_from_this()),
						std::placeholders::_1)));
#else
#if BOOST_VERSION < 107000
			derived().ws().async_accept_ex(
				[](websocket::response_type& m)
			{
				m.insert(boost::beast::http::field::sec_websocket_protocol, "etp12.energistics.org");
			},
#else
			derived().ws().set_option(websocket::stream_base::decorator(
				[](websocket::response_type& m)
				{
					m.insert(boost::beast::http::field::sec_websocket_protocol, "etp12.energistics.org");
				})
			);
			derived().ws().async_accept(
#endif
				boost::asio::bind_executor(
					strand,
					std::bind(
						&AbstractPlainOrSslServerSession::on_accept,
						std::static_pointer_cast<AbstractPlainOrSslServerSession>(shared_from_this()),
						std::placeholders::_1)));
#endif
		}

		 void do_close() {
			 derived().ws().async_close(websocket::close_code::normal,
				boost::asio::bind_executor(
					strand,
					std::bind(
						&AbstractSession::on_close,
						shared_from_this(),
						std::placeholders::_1)));
		 }

		 void on_accept(boost::system::error_code ec) {
			 if (ec) {
				 std::cerr << "on_accept : " << ec.message() << std::endl;
			 }

			 webSocketSessionClosed = false;

			 // Read a message
			 do_read();
		}

		void do_read()
		{
			if (webSocketSessionClosed) {
				fesapi_log("CLOSED : NOTHING MORE TO DO");
				return;
			}

			// Read a message into our buffer
			derived().ws().async_read(
				receivedBuffer,
				boost::asio::bind_executor(
					strand,
					std::bind(
						&AbstractSession::on_read,
						shared_from_this(),
						std::placeholders::_1,
						std::placeholders::_2)));
		}
		
		void setMaxWebSocketMessagePayloadSize(int64_t value) final {
			maxWebSocketMessagePayloadSize = value;
			derived().ws().read_message_max(value);
		}
	};
}
