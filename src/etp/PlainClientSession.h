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

#include "AbstractClientSessionCRTP.h"

namespace ETP_NS
{
	class PlainClientSession : public AbstractClientSessionCRTP<PlainClientSession>
	{
	public:
		/*
		* @param frameSize				Sets the size of the write buffer used by the implementation to send frames : https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/ref/boost__beast__websocket__stream/write_buffer_bytes/overload1.html.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT PlainClientSession(
			InitializationParameters const* initializationParams, const std::string & target, const std::string & authorization, const std::string& proxyAuthorization = "",
			const std::map<std::string, std::string>& additionalHandshakeHeaderFields = {}, std::size_t frameSize = 4096);

		virtual ~PlainClientSession() = default;

		// Called by the base class
#if BOOST_VERSION < 107000
		FETPAPI_DLL_IMPORT_OR_EXPORT std::unique_ptr<websocket::stream<tcp::socket>>& ws() { return ws_; }
#else
		FETPAPI_DLL_IMPORT_OR_EXPORT std::unique_ptr<websocket::stream<boost::beast::tcp_stream>>& ws() { return ws_; }
#endif

		bool isTls() const final{ return false; }

		void asyncConnect(const tcp::resolver::results_type& results)
		{
			// Reality check: IPv6 is unlikely to be available yet
			std::vector<tcp::endpoint> endpoints = std::vector<tcp::endpoint>(results.begin(), results.end());
			std::stable_partition(endpoints.begin(), endpoints.end(), [](auto entry) {return entry.protocol() == tcp::v4(); });

#if BOOST_VERSION < 107000
			ws_.reset(new websocket::stream<tcp::socket>(ioc));
			ws_->write_buffer_size(frameSize_);
			ws_->binary(true);

			// Make the connection on the IP address we get from a lookup
			boost::asio::async_connect(
				ws_->next_layer(),
				endpoints.begin(),
				endpoints.end(),
				std::bind(
					&AbstractClientSessionCRTP::on_ssl_handshake,
					std::static_pointer_cast<AbstractClientSessionCRTP>(shared_from_this()),
					std::placeholders::_1));
#else
			ws_.reset(new websocket::stream<boost::beast::tcp_stream>(ioc));
			ws_->write_buffer_bytes(frameSize_);
			ws_->binary(true);

			// Make the connection on the IP address we get from a lookup
			boost::beast::get_lowest_layer(*ws_).async_connect(
				endpoints,
				boost::beast::bind_front_handler(
					&PlainClientSession::on_connect,
					std::static_pointer_cast<PlainClientSession>(shared_from_this())));
#endif
		}

#if BOOST_VERSION > 106900
		void on_connect(boost::beast::error_code ec, tcp::resolver::results_type::endpoint_type)
		{
			on_ssl_handshake(ec);
		}
#endif

	private:
#if BOOST_VERSION < 107000
		std::unique_ptr<websocket::stream<tcp::socket>> ws_;
#else
		std::unique_ptr<websocket::stream<boost::beast::tcp_stream>> ws_;
#endif
		std::size_t frameSize_;
	};
}
