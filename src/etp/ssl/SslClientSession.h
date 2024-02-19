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

#include "../AbstractClientSessionCRTP.h"

#include <boost/version.hpp>
#if BOOST_VERSION < 106800
#include "ssl_stream.h"
#elif BOOST_VERSION < 107000
#include <boost/beast/experimental/core/ssl_stream.hpp>
#else
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket/ssl.hpp>
#endif

namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

namespace ETP_NS
{
	class SslClientSession : public AbstractClientSessionCRTP<SslClientSession>
	{
	private:
		websocket::stream<boost::beast::ssl_stream<tcp::socket>> ws_;
		http::request<http::empty_body> proxyHandshake;
		http::response<http::empty_body> proxyHandshakeResponse;
		// use own response parser
		// NOTE: 200 response to a CONNECT request from a tunneling proxy do not carry a body
		http::response_parser<http::empty_body> http_proxy_handshake_parser;

	public:
		/*
		* @param frameSize				Sets the size of the write buffer used by the implementation to send frames : https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/ref/boost__beast__websocket__stream/write_buffer_bytes/overload1.html.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT SslClientSession(boost::asio::ssl::context& ctx,
			InitializationParameters* initializationParams, const std::string& target, const std::string& authorization, const std::string& proxyAuthorization = "",
			const std::map<std::string, std::string>& additionalHandshakeHeaderFields = {}, std::size_t frameSize = 4096);

		virtual ~SslClientSession() {}

		// Called by the base class
		FETPAPI_DLL_IMPORT_OR_EXPORT websocket::stream<boost::beast::ssl_stream<tcp::socket>>& ws() { return ws_; }

		void on_resolve(boost::system::error_code ec, tcp::resolver::results_type results)
		{
			if (ec) {
				std::cerr << "on_resolve : " << ec.message() << std::endl;
			}

			// Make the connection on the IP address we get from a lookup
			boost::asio::async_connect(
				ws_.next_layer().next_layer(),
				results.begin(),
				results.end(),
				std::bind(
					&SslClientSession::on_ssl_connect,
					std::static_pointer_cast<SslClientSession>(shared_from_this()),
					std::placeholders::_1));
		}

		void on_ssl_connect(boost::system::error_code ec) {
			if (ec) {
				std::cerr << "on_ssl_connect : " << ec.message() << std::endl;
			}

			if (!proxyHost.empty()) {
				proxyHandshake.version(11);
				proxyHandshake.method(http::verb::connect);
				proxyHandshake.target(etpServerHost + ':' + etpServerPort);
				proxyHandshake.set(http::field::host, etpServerHost + ':' + etpServerPort);
				if (!etpServerAuthorization.empty()) {
					proxyHandshake.set(http::field::authorization, etpServerAuthorization);
				}
				if (!proxyAuthorization.empty()) {
					proxyHandshake.set(http::field::proxy_authorization, proxyAuthorization);
				}

				// Send the handshake to the proxy
				http::async_write(ws_.next_layer().next_layer(), proxyHandshake,
					std::bind(
						&SslClientSession::on_proxy_handshake_write,
						std::static_pointer_cast<SslClientSession>(shared_from_this()),
						std::placeholders::_1,
						std::placeholders::_2));
			}
			else {
				// Perform the SSL handshake
				ws_.next_layer().async_handshake(
					boost::asio::ssl::stream_base::client,
					std::bind(
						&AbstractClientSessionCRTP::on_connect,
						std::static_pointer_cast<AbstractClientSessionCRTP>(shared_from_this()),
						std::placeholders::_1));
			}
		}

		void
			on_proxy_handshake_write(
				boost::system::error_code ec,
				std::size_t bytes_transferred)
		{
			boost::ignore_unused(bytes_transferred);

			if (ec) {
				std::cerr << "Proxy handshake write : " << ec.message() << std::endl;
				return;
			}

			/** Set the skip parse option.
			This option controls whether or not the parser expects to see an HTTP
			body, regardless of the presence or absence of certain fields such as
			Content-Length or a chunked Transfer-Encoding. Depending on the request,
			some responses do not carry a body. For example, a 200 response to a
			CONNECT request from a tunneling proxy, or a response to a HEAD request.
			In these cases, callers may use this function inform the parser that
			no body is expected. The parser will consider the message complete
			after the header has been received.
			@param v `true` to set the skip body option or `false` to disable it.
			@note This function must called before any bytes are processed.
			*/
			http_proxy_handshake_parser.skip(true); // see https://stackoverflow.com/a/49837467/10904212

			// Receive the HTTP response
			http::async_read(ws_.next_layer().next_layer(), receivedBuffer, http_proxy_handshake_parser,
				std::bind(
					&SslClientSession::on_proxy_handshake_read,
					std::static_pointer_cast<SslClientSession>(shared_from_this()),
					std::placeholders::_1,
					std::placeholders::_2));
		}

		void
			on_proxy_handshake_read(
				boost::system::error_code ec,
				std::size_t bytes_transferred)
		{
			boost::ignore_unused(bytes_transferred);
			if (ec) {
				std::cerr << "read : " << ec.message() << std::endl;
				return;
			}

			// Perform the SSL handshake
			ws_.next_layer().async_handshake(
				boost::asio::ssl::stream_base::client,
				std::bind(
					&AbstractClientSessionCRTP::on_connect,
					std::static_pointer_cast<AbstractClientSessionCRTP>(shared_from_this()),
					std::placeholders::_1));
		}
	};
}
