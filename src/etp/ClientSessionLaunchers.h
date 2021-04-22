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

#include "PlainClientSession.h"
#ifdef WITH_ETP_SSL
#include "ssl/SslClientSession.h"
#endif

#include "InitializationParameters.h"

namespace ETP_NS
{
	namespace ClientSessionLaunchers
	{
		/**
		* @param preferredFrameSize The preferred websocket frame payload to use by this client.
		*							If the ETP websocket server on the other end has a lower related MaxWebSocketFramePayloadSize capability, then the used websocket frame payload will be the server one.
		*							Default value corresponds to the default Boost.Beast value : https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/ref/boost__beast__websocket__stream/write_buffer_bytes/overload1.html.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::shared_ptr<ETP_NS::PlainClientSession> createWsClientSession(InitializationParameters* initializationParams, const std::string & target, const std::string & authorization,
			std::size_t preferredMaxFrameSize = 4096);

#ifdef WITH_ETP_SSL
		FETPAPI_DLL_IMPORT_OR_EXPORT std::shared_ptr<ETP_NS::SslClientSession> createWssClientSession(InitializationParameters* initializationParams, const std::string & target, const std::string & authorization,
			const std::string & additionalCertificates = "");
#endif

	}
}
