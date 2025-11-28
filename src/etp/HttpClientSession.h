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

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

namespace ETP_NS
{
	// Performs an HTTP GET and stores the response
	class HttpClientSession : public std::enable_shared_from_this<HttpClientSession>
	{
		tcp::resolver resolver_;
		tcp::socket socket_;
		boost::beast::flat_buffer buffer_; // (Must persist between reads)
		http::request<http::empty_body> req_;
		http::response<http::string_body> res_;

	public:
		// Resolver and socket require an io_context
		explicit
			HttpClientSession(boost::asio::io_context& ioc)
			: resolver_(ioc)
			, socket_(ioc)
		{
		}

		// Start the asynchronous operation
		void
			run(
				const std::string& etpServerHost,
				uint16_t etpServerPort,
				const std::string& etpServerTarget,
				uint32_t version,
				const std::string& etpServerAuthorization = "",
				const std::string& proxyHost = "",
				uint16_t proxyPort = 80,
				const std::string& proxyAuthorization = "")
		{
			// Set up an HTTP GET request message
			req_.version(version);
			req_.method(http::verb::get);
			req_.target(etpServerTarget);
			req_.set(http::field::host, etpServerHost + ':' + std::to_string(etpServerPort));
			req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
			if (!etpServerAuthorization.empty()) {
				req_.set(http::field::authorization, etpServerAuthorization);
			}
			if (!proxyAuthorization.empty()) {
				req_.set(http::field::proxy_authorization, proxyAuthorization);
			}

			// Look up the domain name
			resolver_.async_resolve(
				proxyHost.empty() ? etpServerHost : proxyHost,
				std::to_string(proxyHost.empty() ? etpServerPort : proxyPort),
				std::bind(
					&HttpClientSession::on_resolve,
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

			// Reality check: IPv6 is unlikely to be available yet
			std::vector<tcp::endpoint> endpoints(results.begin(), results.end());
			std::stable_partition(endpoints.begin(), endpoints.end(), [](auto entry) {return entry.protocol() == tcp::v4(); });

			// Make the connection on the IP address we get from a lookup
			boost::asio::async_connect(
				socket_,
				endpoints.begin(),
				endpoints.end(),
				std::bind(
					&HttpClientSession::on_connect,
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

			// Send the HTTP request to the remote host
			http::async_write(socket_, req_,
				std::bind(
					&HttpClientSession::on_write,
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
			http::async_read(socket_, buffer_, res_,
				std::bind(
					&HttpClientSession::on_read,
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

			// Gracefully close the socket
			socket_.shutdown(tcp::socket::shutdown_both, ec);

			// not_connected happens sometimes so don't bother reporting it.
			if (ec && ec != boost::system::errc::not_connected) {
				std::cerr << "shutdown : " << ec.message() << std::endl;
				return;
			}

			// If we get here then the connection is closed gracefully
		}

		const http::response<http::string_body>& getResponse() const {
			return res_;
		}
	};
}
