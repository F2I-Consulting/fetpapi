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

#include <iostream>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

namespace ETP_NS
{
	// Performs an HTTP GET and stores the response
	class HttpsClientSession : public std::enable_shared_from_this<HttpsClientSession>
	{
		tcp::resolver resolver_;
		ssl::stream<tcp::socket> stream_;
		boost::beast::flat_buffer buffer_; // (Must persist between reads)
		http::request<http::empty_body> req_;
		http::request<http::empty_body> proxyHandshake;
		http::response<http::string_body> res_;
		http::response<http::empty_body> proxyHandshakeResponse;
		// use own response parser
		// NOTE: 200 response to a CONNECT request from a tunneling proxy do not carry a body
		http::response_parser<http::empty_body> http_proxy_handshake_parser;

	public:
		// Resolver and stream require an io_context
		explicit
			HttpsClientSession(boost::asio::io_context& ioc, ssl::context& ctx)
			: resolver_(ioc)
			, stream_(ioc, ctx)
			, http_proxy_handshake_parser(proxyHandshakeResponse)
		{
		}

		// Start the asynchronous operation
		void
			run(
				const std::string& etpServerHost,
				uint16_t etpServerPort,
				const std::string& etpServerTarget,
				int version,
				std::string authorization = "",
				const std::string& proxyHost = "",
				uint16_t proxyPort = 80,
				const std::string& proxyAuthorization = "")
		{
			// Set SNI Hostname (many hosts need this to handshake successfully)
			if (!SSL_set_tlsext_host_name(stream_.native_handle(), etpServerHost.data()))
			{
				boost::system::error_code ec{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
				std::cerr << ec.message() << "\n";
				return;
			}

			// Set up an HTTP GET request message
			req_.version(version);
			req_.method(http::verb::get);
			req_.target(etpServerTarget);
			req_.set(http::field::host, etpServerHost + ':' + std::to_string(etpServerPort));
			req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
			if (!authorization.empty()) {
				req_.set(http::field::authorization, authorization);
			}

			if (!proxyHost.empty()) {
				proxyHandshake.version(version);
				proxyHandshake.method(http::verb::connect);
				proxyHandshake.target(etpServerHost + ':' + std::to_string(etpServerPort));
				proxyHandshake.set(http::field::host, etpServerHost + ':' + std::to_string(etpServerPort));
				if (!authorization.empty()) {
					proxyHandshake.set(http::field::authorization, authorization);
				}
				if (!proxyAuthorization.empty()) {
					proxyHandshake.set(http::field::proxy_authorization, proxyAuthorization);
				}
			}

			// Look up the domain name
			resolver_.async_resolve(
				proxyHost.empty() ? etpServerHost : proxyHost,
				std::to_string(proxyHost.empty() ? etpServerPort : proxyPort),
				std::bind(
					&HttpsClientSession::on_resolve,
					shared_from_this(),
					std::placeholders::_1,
					std::placeholders::_2));
		}

		void
			on_resolve(
				boost::system::error_code ec,
				tcp::resolver::results_type results)
		{
			if (ec) {
				std::cerr << "resolve : " << ec.message() << std::endl;
				return;
			}

			// Make the connection on the IP address we get from a lookup
			boost::asio::async_connect(
				stream_.next_layer(),
				results.begin(),
				results.end(),
				std::bind(
					&HttpsClientSession::on_connect,
					shared_from_this(),
					std::placeholders::_1));
		}

		void
			on_connect(boost::system::error_code ec)
		{
			if (ec) {
				std::cerr << "connect : " << ec.message() << std::endl;
				return;
			}

			if (proxyHandshake.count(http::field::host) > 0) {
				// Send the handshake to the proxy
				http::async_write(stream_.next_layer(), proxyHandshake,
					std::bind(
						&HttpsClientSession::on_proxy_handshake_write,
						shared_from_this(),
						std::placeholders::_1,
						std::placeholders::_2));
			}
			else {
				// Perform the SSL handshake
				stream_.async_handshake(
					ssl::stream_base::client,
					std::bind(
						&HttpsClientSession::on_handshake,
						shared_from_this(),
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
			http::async_read(stream_.next_layer(), buffer_, http_proxy_handshake_parser,
				std::bind(
					&HttpsClientSession::on_proxy_handshake_read,
					shared_from_this(),
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
			stream_.async_handshake(
				ssl::stream_base::client,
				std::bind(
					&HttpsClientSession::on_handshake,
					shared_from_this(),
					std::placeholders::_1));
		}

		void
			on_handshake(boost::system::error_code ec)
		{
			if (ec) {
				std::cerr << "handshake : " << ec.message() << std::endl;
				return;
			}

			// Send the HTTP request to the remote host
			http::async_write(stream_, req_,
				std::bind(
					&HttpsClientSession::on_write,
					shared_from_this(),
					std::placeholders::_1,
					std::placeholders::_2));
		}

		void
			on_write(
				boost::system::error_code ec,
				std::size_t bytes_transferred)
		{
			boost::ignore_unused(bytes_transferred);

			if (ec) {
				std::cerr << "write : " << ec.message() << std::endl;
				return;
			}

			// Receive the HTTP response
			http::async_read(stream_, buffer_, res_,
				std::bind(
					&HttpsClientSession::on_read,
					shared_from_this(),
					std::placeholders::_1,
					std::placeholders::_2));
		}

		void
			on_read(
				boost::system::error_code ec,
				std::size_t bytes_transferred)
		{
			boost::ignore_unused(bytes_transferred);

			if (ec) {
				std::cerr << "read : " << ec.message() << std::endl;
				return;
			}

			// Gracefully close the stream_
			stream_.async_shutdown(
				std::bind(
					&HttpsClientSession::on_shutdown,
					shared_from_this(),
					std::placeholders::_1));
		}

		void
			on_shutdown(boost::system::error_code ec)
		{
			if (ec == boost::asio::error::eof)
			{
				// Rationale:
				// http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
				ec.assign(0, ec.category());
			}
			if (ec) {
				// Do nothing : https://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
				//std::cerr << "shutdown : " << ec.message() << std::endl;
				return;
			}

			// If we get here then the connection is closed gracefully
		}

		const http::response<http::string_body>& getResponse() const {
			return res_;
		}
	};
}
