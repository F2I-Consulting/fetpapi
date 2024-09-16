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

#include "InitializationParameters.h"

namespace ETP_NS
{
	namespace ClientSessionLaunchers
	{
		/**
		* Create a client session based on some initialization parameters and authorization information.
		* It does not run the created client session.
		* @param initializationParams	The initialization parameters
		* @param etpServerAuthorization The etp server authorization. It generally starts by "Bearer" followed by a token i.e. "Bearer myToken"
		* @param proxyAuthorization		The proxy authorization. Necessary if the session connects to the ETP server through a protected proxy.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::shared_ptr<ETP_NS::ClientSession> createClientSession(InitializationParameters const* initializationParams,
			const std::string& etpServerAuthorization, const std::string& proxyAuthorization = "");
	}
}
