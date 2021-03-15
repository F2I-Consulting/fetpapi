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

namespace ETP_NS
{
	class ServerInitializationParameters
	{
	protected:
		boost::uuids::uuid identifier;

	public:
		/**
		* Set the identifier of the server a default value.
		* You should set the identifer as wanted in your own derived class.
		*/
		ServerInitializationParameters() {}
		/**
		* Mainly for use with SWIG i.e. boost uuid structure is not easily portable whereas strings are.
		*/
		ServerInitializationParameters(const std::string & serverUuid) {
			std::stringstream ss(serverUuid);
			ss >> identifier;
		}
		virtual ~ServerInitializationParameters() = default;

		FETPAPI_DLL_IMPORT_OR_EXPORT virtual const boost::uuids::uuid& getServerInstanceId() const { return identifier; }
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::string getApplicationName() const { return "F2I-CONSULTING ETP SERVER"; }
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::string getApplicationVersion() const { return "0.0"; }
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::string getContactEmail() const { return "philippe.verney@f2i-consulting.com"; }
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::string getContactName() const { return "Philippe Verney"; }
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::string getContactPhone() const { return "Please use Zoom or Slack"; }
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::string getOrganizationName() const { return "F2I-CONSULTING"; }

		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::vector<std::string> makeSupportedEncodings() const;
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> makeEndpointCapabilities() const;
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> makeSupportedDataObjects() const;
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> makeSupportedProtocols() const;

		/**
		* Override this method in order to register some dedicated protocol handlers for a session.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT virtual void postSessionCreationOperation (AbstractSession*) const {}
	};
}
