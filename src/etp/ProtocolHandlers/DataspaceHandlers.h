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

#include "ProtocolHandlers.h"

namespace ETP_NS
{
	class FETPAPI_DLL_IMPORT_OR_EXPORT DataspaceHandlers : public ProtocolHandlers
	{
	public:
		DataspaceHandlers(AbstractSession* mySession): ProtocolHandlers(mySession) {}
		virtual ~DataspaceHandlers() = default;

	    void decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader & mh, avro::DecoderPtr d);

	    virtual void on_GetDataspaces(const Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces & msg, int64_t correlationId);
		virtual void on_GetDataspacesResponse(const Energistics::Etp::v12::Protocol::Dataspace::GetDataspacesResponse & msg, int64_t correlationId);
	    virtual void on_PutDataspaces(const Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces & msg, int64_t correlationId);
		virtual void on_PutDataspacesResponse(const Energistics::Etp::v12::Protocol::Dataspace::PutDataspacesResponse & msg, int64_t correlationId);
	    virtual void on_DeleteDataspaces(const Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces & msg, int64_t correlationId);
		virtual void on_DeleteDataspacesResponse(const Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspacesResponse & msg, int64_t correlationId);

		std::vector<Energistics::Etp::v12::Datatypes::Object::Dataspace> getDataspaces() const {
			return dataspaces;
		}
		void clearDataspaces() { dataspaces.clear(); }

		std::vector<std::string> getSuccessKeys() const {
			return successKeys;
		}
		void clearSuccessKeys() { successKeys.clear(); }

	private:
		std::vector<Energistics::Etp::v12::Datatypes::Object::Dataspace> dataspaces;
		std::vector<std::string> successKeys;
	};
}
