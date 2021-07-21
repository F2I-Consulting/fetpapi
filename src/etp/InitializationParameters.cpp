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
#include "InitializationParameters.h"

#include "AbstractSession.h"

using namespace ETP_NS;

std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> InitializationParameters::makeEndpointCapabilities() const
{
	std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> result;

	Energistics::Etp::v12::Datatypes::DataValue value;

	if (maxWebSocketMessagePayloadSize_ > 0) {
		value.item.set_long(maxWebSocketMessagePayloadSize_);
		result["MaxWebSocketFramePayloadSize"] = value;
		result["MaxWebSocketMessagePayloadSize"] = value;
	}

	value.item.set_boolean(false);
	result["SupportsAlternateRequestUris"] = value;
	result["SupportsMessageHeaderExtension"] = value;

	return result;
}

std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> InitializationParameters::makeSupportedDataObjects() const
{
	std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> result;

	Energistics::Etp::v12::Datatypes::SupportedDataObject supportedDataObject;
	supportedDataObject.qualifiedType = "resqml20.*";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "resqml22.*";
	result.push_back(supportedDataObject);

	supportedDataObject.qualifiedType = "eml20.EpcExternalPartReference";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "eml21.PropertyKind";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "eml23.Activity";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "eml23.ActivityTemplate";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "eml23.GraphicalInformationSet";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "eml23.PropertyKind";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "eml23.TimeSeries";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "eml23.EpcExternalPartReference";
	result.push_back(supportedDataObject);

	supportedDataObject.qualifiedType = "witsml20.Channel";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml20.Trajectory";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml20.Well";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml20.Wellbore";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml20.WellboreCompletion";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml20.WellboreGeometry";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml20.WellCompletion";
	result.push_back(supportedDataObject);

	supportedDataObject.qualifiedType = "prodml21.FluidCharacterization";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "prodml21.FluidSystem";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "prodml21.TimeSeriesData";
	result.push_back(supportedDataObject);

	return result;
}

std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> InitializationParameters::makeSupportedProtocols() const
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
	result.push_back(protocol);

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
	result.push_back(protocol);

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
	result.push_back(protocol);

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
	result.push_back(protocol);

	return result;
}

void InitializationParameters::postSessionCreationOperation(AbstractSession* session) const {
	session->setCoreProtocolHandlers(std::make_shared<CoreHandlers>(session));
}
