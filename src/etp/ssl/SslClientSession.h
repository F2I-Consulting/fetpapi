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
#elif BOOST_VERSION < 108600
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket/ssl.hpp>
#else
#include <boost/asio/ssl.hpp>
#include <boost/beast/websocket/ssl.hpp>
#endif

namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

namespace ETP_NS
{
	class SslClientSession : public AbstractClientSessionCRTP<SslClientSession>
	{
	public:
		/*
		* @param frameSize				Sets the size of the write buffer used by the implementation to send frames : https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/ref/boost__beast__websocket__stream/write_buffer_bytes/overload1.html.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT SslClientSession(boost::asio::ssl::context&& ctx,
			InitializationParameters const* initializationParams, const std::string& target, const std::string& authorization, const std::string& proxyAuthorization = "",
			const std::map<std::string, std::string>& additionalHandshakeHeaderFields = {}, std::size_t frameSize = 4096);

		virtual ~SslClientSession() {}

		// Called by the base class
#if BOOST_VERSION < 107000
		FETPAPI_DLL_IMPORT_OR_EXPORT std::unique_ptr<websocket::stream<boost::beast::ssl_stream<tcp::socket>>>& ws() { return ws_; }
#elif BOOST_VERSION < 108600
		FETPAPI_DLL_IMPORT_OR_EXPORT std::unique_ptr<websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream>>>& ws() { return ws_; }
#else
		FETPAPI_DLL_IMPORT_OR_EXPORT std::unique_ptr< websocket::stream<boost::asio::ssl::stream<boost::beast::tcp_stream>>>& ws() { return ws_; }
#endif

		bool isTls() const final { return true; }

		void asyncConnect(const tcp::resolver::results_type& results)
		{
#if BOOST_VERSION < 107000
			ws_.reset(new websocket::stream<boost::beast::ssl_stream<tcp::socket>>(ioc, sslContext_));
#elif BOOST_VERSION < 108600
			ws_.reset(new websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream>>(ioc, sslContext_));
#else
			ws_.reset(new websocket::stream<boost::asio::ssl::stream<boost::beast::tcp_stream>>(ioc, sslContext_));
#endif

			ws_->binary(true);
#if BOOST_VERSION < 107000
			ws_->write_buffer_size(frameSize_);
#else
			ws_->write_buffer_bytes(frameSize_);
#endif

			// Set SNI Hostname (many hosts need this to handshake successfully)
			if (!SSL_set_tlsext_host_name(ws_->next_layer().native_handle(), etpServerHost.c_str()))
			{
				boost::system::error_code ecSNI{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
				std::cerr << "Websocket on connect (SNI): " << ecSNI.message() << std::endl;
			}

			// Reality check: IPv6 is unlikely to be available yet
			std::vector<tcp::endpoint> endpoints = std::vector<tcp::endpoint>(results.begin(), results.end());
			std::stable_partition(endpoints.begin(), endpoints.end(), [](auto entry) {return entry.protocol() == tcp::v4(); });

			// Make the connection on the IP address we get from a lookup
#if BOOST_VERSION < 107000
			boost::asio::async_connect(
				ws_->next_layer().next_layer(),
				endpoints.begin(),
				endpoints.end(),
				std::bind(
					&SslClientSession::on_ssl_connect,
					std::static_pointer_cast<SslClientSession>(shared_from_this()),
					std::placeholders::_1));
#else
			boost::beast::get_lowest_layer(*ws_).async_connect(
				endpoints,
				boost::beast::bind_front_handler(
					&SslClientSession::on_ssl_connect,
					std::static_pointer_cast<SslClientSession>(shared_from_this())));
#endif
		}

#if BOOST_VERSION < 107000
		void on_ssl_connect(boost::system::error_code ec) {
#else
		void on_ssl_connect(boost::beast::error_code ec, tcp::resolver::results_type::endpoint_type) {
#endif
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
				http::async_write(ws_->next_layer().next_layer(), proxyHandshake,
					std::bind(
						&SslClientSession::on_proxy_handshake_write,
						std::static_pointer_cast<SslClientSession>(shared_from_this()),
						std::placeholders::_1,
						std::placeholders::_2));
			}
			else {
				// Perform the SSL handshake
				ws_->next_layer().async_handshake(
					boost::asio::ssl::stream_base::client,
					std::bind(
						&AbstractClientSessionCRTP::on_ssl_handshake,
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
			http::async_read(ws_->next_layer().next_layer(), receivedBuffer, http_proxy_handshake_parser,
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
			ws_->next_layer().async_handshake(
				boost::asio::ssl::stream_base::client,
				std::bind(
					&AbstractClientSessionCRTP::on_ssl_handshake,
					std::static_pointer_cast<AbstractClientSessionCRTP>(shared_from_this()),
					std::placeholders::_1));
		}

	private:
		boost::asio::ssl::context sslContext_;
#if BOOST_VERSION < 107000
		std::unique_ptr<websocket::stream<boost::beast::ssl_stream<tcp::socket>>> ws_;
#elif BOOST_VERSION < 108600
		std::unique_ptr<websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream>>> ws_;
#else
		std::unique_ptr<websocket::stream<boost::asio::ssl::stream<boost::beast::tcp_stream>>> ws_;
#endif
		http::request<http::empty_body> proxyHandshake;
		http::response<http::empty_body> proxyHandshakeResponse;
		// use own response parser
		// NOTE: 200 response to a CONNECT request from a tunneling proxy do not carry a body
		http::response_parser<http::empty_body> http_proxy_handshake_parser;
		std::size_t frameSize_;
	};
}
