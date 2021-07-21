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
#include "ServerInitializationParameters.h"

using namespace ETP_NS;

std::vector<std::string> ServerInitializationParameters::makeSupportedEncodings() const
{
	return std::vector<std::string>(1, "binary");
}

std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> ServerInitializationParameters::makeSupportedProtocols() const
{
	std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> result;
	Energistics::Etp::v12::Datatypes::Version protocolVersion;
	protocolVersion.major = 1;
	protocolVersion.minor = 2;
	protocolVersion.patch = 0;
	protocolVersion.revision = 0;

	Energistics::Etp::v12::Datatypes::SupportedProtocol protocol;
	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Core);
	protocol.protocolVersion = protocolVersion;
	protocol.role = "server";
	result.push_back(protocol);

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
	protocol.role = "store";
	result.push_back(protocol);

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Store);
	result.push_back(protocol);

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
	Energistics::Etp::v12::Datatypes::DataValue value;
	value.item.set_long((std::numeric_limits<long>::max)());
	protocol.protocolCapabilities["MaxDataArraySize"] = value;
	result.push_back(protocol);
	protocol.protocolCapabilities.clear();

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
	result.push_back(protocol);

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
	result.push_back(protocol);

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
	result.push_back(protocol);

	return result;
}
