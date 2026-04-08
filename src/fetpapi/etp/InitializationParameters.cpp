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

#include <stdexcept>
#include <tuple>

#include <boost/version.hpp>
#if BOOST_VERSION >= 108100
#include <boost/url.hpp>
#endif

#include "AbstractSession.h"

using namespace ETP_NS;

namespace {

#if BOOST_VERSION < 108100
	std::tuple<std::string, uint16_t, std::string> extractHostPortPathFromUrl(const std::string& url) {
		if (url.empty()) {
			throw std::invalid_argument("URL cannot be empty");
		}

		std::tuple<std::string, uint16_t, std::string> result;
		const size_t schemeSeparatorPos = url.find("://");

		const size_t hostStart = schemeSeparatorPos == std::string::npos ? 0 : schemeSeparatorPos + 3;
		size_t hostEnd;
		size_t portStart = url.find(":", hostStart);
		size_t portEnd;

		if (portStart == std::string::npos) {
			hostEnd = url.find("/", hostStart + 3);
			portEnd = hostEnd;
			std::get<1>(result) = url.find("wss://") == 0 || url.find("https://") == 0 ? 443 : 80;
		}
		else {
			hostEnd = portStart++;
			portEnd = url.find("/", portStart);
			uint16_t readPort = static_cast<uint16_t>(stoi(url.substr(portStart, portEnd - portStart)));
			if (readPort < 1 || readPort > (std::numeric_limits<uint16_t>::max)()) {
				throw std::out_of_range("The port " + std::to_string(readPort) + " is out of the allowed range for TCP ports (0,2^16)");
			}
			std::get<1>(result) = static_cast<uint16_t>(readPort);
		}

		if (hostEnd == std::string::npos) {
			std::get<2>(result) = "";
			std::get<0>(result) = url.substr(hostStart);
		}
		else {
			std::get<2>(result) = portEnd < url.size() - 1 ? url.substr(portEnd + 1) : "";
			std::get<0>(result) = url.substr(hostStart, hostEnd - hostStart);
		}

		return result;
	}
#else
	std::tuple<std::string, uint16_t, std::string> extractHostPortPathFromUrl(const std::string& urlStr) {
		// Parse the URL
		boost::urls::result<boost::urls::url_view> result = boost::urls::parse_uri(urlStr);
		if (!result) {
			throw std::invalid_argument("Invalid URL: " + urlStr);
		}
		boost::urls::url_view url = *result;

		// Extract host (handles IPv4, IPv6, and domain names)
		std::string host = url.host();
		if (host.empty()) {
			throw std::invalid_argument("Host cannot be empty");
		}

		// Extract port (falls back to default if not specified)
		uint16_t port = url.port_number();
		if (port == 0) {
			port = boost::urls::default_port(url.scheme_id());
			if (port == 0) {
				throw std::invalid_argument("The default port is unknown for scheme " + std::string(url.scheme()));
			}
		}

		return { host, port, url.path() };
	}
#endif
}

void InitializationParameters::initFromUrl(const std::string& etpUrl, const std::string& proxyUrl)
{
	if (!etpUrl.empty()) {
		std::tuple<std::string, uint16_t, std::string> serverInfo = extractHostPortPathFromUrl(etpUrl);
		etpServerHost = std::get<0>(serverInfo);
		etpServerPort = std::get<1>(serverInfo);
		etpServerUrlPath = std::get<2>(serverInfo);
	}

	if (!proxyUrl.empty()) {
		std::tuple<std::string, uint16_t, std::string> serverInfo = extractHostPortPathFromUrl(proxyUrl);
		proxyHost = std::get<0>(serverInfo);
		proxyPort = std::get<1>(serverInfo);
	}
}

std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> InitializationParameters::makeEndpointCapabilities() const
{
	std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> result;

	Energistics::Etp::v12::Datatypes::DataValue value;

	if (maxWebSocketMessagePayloadSize > 0) {
		value.item.set_long(static_cast<int64_t>(maxWebSocketMessagePayloadSize));
		result["MaxWebSocketFramePayloadSize"] = value;
		result["MaxWebSocketMessagePayloadSize"] = value;
	}

	value.item.set_boolean(false);
	result["SupportsAlternateRequestUris"] = value;
	result["SupportsMessageHeaderExtensions"] = value;

	return result;
}

std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> InitializationParameters::makeSupportedDataObjects() const
{
	std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> dataobjectCaps;
	Energistics::Etp::v12::Datatypes::DataValue dataVal;
	dataVal.item.set_boolean(true);
	dataobjectCaps["SupportsDelete"] = dataVal;
	dataobjectCaps["SupportsGet"] = dataVal;
	dataobjectCaps["SupportsPut"] = dataVal;

	std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> result;

	Energistics::Etp::v12::Datatypes::SupportedDataObject supportedDataObject;
	supportedDataObject.qualifiedType = "resqml20.*";
	supportedDataObject.dataObjectCapabilities = dataobjectCaps;
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "resqml22.*";
	result.push_back(supportedDataObject);

	supportedDataObject.qualifiedType = "eml20.obj_EpcExternalPartReference";
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

	supportedDataObject.qualifiedType = "witsml21.Channel";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml21.Trajectory";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml21.Well";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml21.Wellbore";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml21.WellboreCompletion";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml21.WellboreGeometry";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "witsml21.WellCompletion";
	result.push_back(supportedDataObject);

	supportedDataObject.qualifiedType = "prodml22.FluidCharacterization";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "prodml22.FluidSystem";
	result.push_back(supportedDataObject);
	supportedDataObject.qualifiedType = "prodml22.TimeSeriesData";
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
	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
	protocol.protocolVersion = protocolVersion;
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

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::CoreOSDU);
	result.push_back(protocol);

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::StoreOSDU);
	result.push_back(protocol);

	protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::DataspaceOSDU);
	result.push_back(protocol);

	return result;
}

void InitializationParameters::postSessionCreationOperation(AbstractSession* session) const {
	session->setCoreProtocolHandlers(std::make_shared<CoreHandlers>(session));
	session->setDiscoveryProtocolHandlers(std::make_shared<DiscoveryHandlers>(session));
	session->setStoreProtocolHandlers(std::make_shared<StoreHandlers>(session));
	session->setStoreNotificationProtocolHandlers(std::make_shared<StoreNotificationHandlers>(session));
	session->setDataArrayProtocolHandlers(std::make_shared<DataArrayHandlers>(session));
	session->setTransactionProtocolHandlers(std::make_shared<TransactionHandlers>(session));
	session->setDataspaceProtocolHandlers(std::make_shared<DataspaceHandlers>(session));
	session->setCoreOSDUProtocolHandlers(std::make_shared<CoreOSDUHandlers>(session));
	session->setStoreOSDUProtocolHandlers(std::make_shared<StoreOSDUHandlers>(session));
	session->setDataspaceOSDUProtocolHandlers(std::make_shared<DataspaceOSDUHandlers>(session));
}
