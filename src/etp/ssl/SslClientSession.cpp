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
#include "SslClientSession.h"

using namespace ETP_NS;

SslClientSession::SslClientSession(boost::asio::ssl::context& ctx,
	InitializationParameters* initializationParams, const std::string& target, const std::string& authorization, const std::string& proxyAuthorization,
	const std::map<std::string, std::string>& additionalHandshakeHeaderFields, std::size_t frameSize)
	: AbstractClientSessionCRTP<SslClientSession>(initializationParams, target, authorization, proxyAuthorization),
		ws_(ioc, ctx)
{
	ws_.binary(true);
#if BOOST_VERSION < 107000
	ws_.write_buffer_size(frameSize);
#else
	ws_.write_buffer_bytes(frameSize);
#endif

	additionalHandshakeHeaderFields_ = additionalHandshakeHeaderFields;
}
