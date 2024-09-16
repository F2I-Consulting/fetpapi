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
	class FETPAPI_DLL_IMPORT_OR_EXPORT DataspaceOSDUHandlers : public ProtocolHandlers
	{
	public:
		DataspaceOSDUHandlers(AbstractSession* mySession): ProtocolHandlers(mySession) {}
		virtual ~DataspaceOSDUHandlers() = default;

	    void decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader & mh, avro::DecoderPtr d);

		virtual void on_GetDataspaceInfo(const Energistics::Etp::v12::Protocol::DataspaceOSDU::GetDataspaceInfo& msg, int64_t correlationId);
		virtual void on_GetDataspaceInfoResponse(const Energistics::Etp::v12::Protocol::DataspaceOSDU::GetDataspaceInfoResponse& msg, int64_t correlationId);
	    virtual void on_CopyDataspacesContent(const Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyDataspacesContent& msg, int64_t correlationId);
		virtual void on_CopyDataspacesContentResponse(const Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyDataspacesContentResponse & msg, int64_t correlationId);
		virtual void on_LockDataspaces(const Energistics::Etp::v12::Protocol::DataspaceOSDU::LockDataspaces& msg, int64_t correlationId);
		virtual void on_LockDataspacesResponse(const Energistics::Etp::v12::Protocol::DataspaceOSDU::LockDataspacesResponse& msg, int64_t correlationId);
	    virtual void on_CopyToDataspace(const Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyToDataspace& msg, int64_t correlationId);
		virtual void on_CopyToDataspaceResponse(const Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyToDataspaceResponse & msg, int64_t correlationId);

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
