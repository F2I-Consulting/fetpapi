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
	class FETPAPI_DLL_IMPORT_OR_EXPORT DiscoveryHandlers : public ProtocolHandlers
	{
	public:
		DiscoveryHandlers(AbstractSession* mySession): ProtocolHandlers(mySession) {}
		virtual ~DiscoveryHandlers() = default;

	    void decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader & mh, avro::DecoderPtr d);

		virtual void on_GetResources(const Energistics::Etp::v12::Protocol::Discovery::GetResources & msg, int64_t correlationId);
		virtual void on_GetResourcesResponse(const Energistics::Etp::v12::Protocol::Discovery::GetResourcesResponse & msg, int64_t correlationId);
		virtual void on_GetResourcesEdgesResponse(const Energistics::Etp::v12::Protocol::Discovery::GetResourcesEdgesResponse & msg, int64_t correlationId);

		virtual void on_GetDeletedResources(const Energistics::Etp::v12::Protocol::Discovery::GetDeletedResources & msg, int64_t correlationId);
		virtual void on_GetDeletedResourcesResponse(const Energistics::Etp::v12::Protocol::Discovery::GetDeletedResourcesResponse & msg, int64_t correlationId);

		std::vector<Energistics::Etp::v12::Datatypes::Object::Resource> getResources() const {
			return resources;
		}
		void clearResources() { resources.clear(); }

		std::vector<Energistics::Etp::v12::Datatypes::Object::DeletedResource> getDeletedResources() const {
			return deletedResources;
		}
		void clearDeletedResources() { deletedResources.clear(); }

	private:
		std::vector<Energistics::Etp::v12::Datatypes::Object::Resource> resources;
		std::vector<Energistics::Etp::v12::Datatypes::Object::DeletedResource> deletedResources;
	};
}
