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
		FETPAPI_DLL_IMPORT_OR_EXPORT std::unique_ptr<websocket::stream<tcp::socket>>& ws() { return ws_; }

		bool isTls() const final{ return false; }

		void asyncConnect()
		{
			ws_.reset(new websocket::stream<tcp::socket>(ioc));
			ws_->binary(true);
#if BOOST_VERSION < 107000
			ws_->write_buffer_size(frameSize_);
#else
			ws_->write_buffer_bytes(frameSize_);
#endif

			// Make the connection on the IP address we get from a lookup
			boost::asio::async_connect(
				ws_->next_layer(),
				endpoints.begin(),
				endpoints.end(),
				std::bind(
					&AbstractClientSessionCRTP::on_connect,
					std::static_pointer_cast<AbstractClientSessionCRTP>(shared_from_this()),
					std::placeholders::_1));
		}

	private:
		std::unique_ptr<websocket::stream<tcp::socket>> ws_;
		std::size_t frameSize_;
	};
}
