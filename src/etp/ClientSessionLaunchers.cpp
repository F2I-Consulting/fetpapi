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
#include "ClientSessionLaunchers.h"

#include <sstream>

#include "HttpClientSession.h"

namespace
{
	std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> getRequestedProtocols() {
		Energistics::Etp::v12::Datatypes::Version protocolVersion;
		protocolVersion.major = 1;
		protocolVersion.minor = 2;
		protocolVersion.patch = 0;
		protocolVersion.revision = 0;

		std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> requestedProtocols;
		Energistics::Etp::v12::Datatypes::SupportedProtocol protocol;

		protocol.protocol = Energistics::Etp::v12::Datatypes::Protocol::Core;
		protocol.protocolVersion = protocolVersion;
		protocol.role = "server";
		requestedProtocols.push_back(protocol);

		protocol.protocol = Energistics::Etp::v12::Datatypes::Protocol::Discovery;
		protocol.protocolVersion = protocolVersion;
		protocol.role = "store";
		requestedProtocols.push_back(protocol);

		protocol.protocol = Energistics::Etp::v12::Datatypes::Protocol::Store;
		protocol.protocolVersion = protocolVersion;
		protocol.role = "store";
		requestedProtocols.push_back(protocol);

		protocol.protocol = Energistics::Etp::v12::Datatypes::Protocol::DataArray;
		protocol.protocolVersion = protocolVersion;
		protocol.role = "store";
		requestedProtocols.push_back(protocol);

		protocol.protocol = Energistics::Etp::v12::Datatypes::Protocol::StoreNotification;
		protocol.protocolVersion = protocolVersion;
		protocol.role = "store";
		requestedProtocols.push_back(protocol);


		return requestedProtocols;
	}

	std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> getSupportedDataObjects() {
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

	std::size_t getNegotiatedMaxWebSocketFramePayloadSize(const std::string & responseBody, std::size_t preferredMaxFrameSize) {
		const auto maxWebSocketFramePayloadSizePos = responseBody.find("MaxWebSocketFramePayloadSize");
		if (maxWebSocketFramePayloadSizePos != std::string::npos) {
			std::istringstream iss(responseBody);
			iss.seekg(maxWebSocketFramePayloadSizePos);

			std::string temp;
			std::size_t serverMaxWebSocketFramePayloadSize;
			while (!iss.eof()) {
				/* extracting word by word from stream */
				iss >> temp;

				/* Checking the given word is integer or not */
				if (std::istringstream(temp) >> serverMaxWebSocketFramePayloadSize) {
					return std::min(serverMaxWebSocketFramePayloadSize, preferredMaxFrameSize);
				}
			}
		}

		return preferredMaxFrameSize;
	}
}

std::shared_ptr<ETP_NS::PlainClientSession> ETP_NS::ClientSessionLaunchers::createWsClientSession(InitializationParameters* initializationParams, const std::string & target, const std::string & authorization,
	std::size_t preferredMaxFrameSize)
{
	boost::asio::io_context ioc;
	auto httpClientSession = std::make_shared<HttpClientSession>(ioc);
	std::string etpServerCapTarget = target.empty() ? "/" : target;
	if (etpServerCapTarget[etpServerCapTarget.size() - 1] != '/') {
		etpServerCapTarget += '/';
	}
	etpServerCapTarget += ".well-known/etp-server-capabilities?GetVersion=etp12.energistics.org";
	httpClientSession->run(initializationParams->getHost().c_str(), initializationParams->getPort(), etpServerCapTarget.c_str(), 11, authorization);
	// Run the I/O service. The call will return when the get operation is complete.
	ioc.run();

	preferredMaxFrameSize = getNegotiatedMaxWebSocketFramePayloadSize(httpClientSession->getResponse().body(), preferredMaxFrameSize);

	auto result = std::make_shared<PlainClientSession>(initializationParams, target.empty() ? "/" : target, authorization, preferredMaxFrameSize);
	initializationParams->postSessionCreationOperation(result.get());
	return result;
}

#ifdef WITH_ETP_SSL

namespace ssl = boost::asio::ssl;               // from <boost/asio/ssl.hpp>

std::shared_ptr<ETP_NS::SslClientSession> ETP_NS::ClientSessionLaunchers::createWssClientSession(InitializationParameters* initializationParams, const std::string & target, const std::string & authorization,
	const std::string & additionalCertificates)
{
	// The SSL context is required, and holds certificates
	ssl::context ctx{ ssl::context::sslv23_client };

	if (!additionalCertificates.empty()) {
		boost::system::error_code ec;
		ctx.add_certificate_authority(
			boost::asio::buffer(additionalCertificates.data(), additionalCertificates.size()), ec);
		if (ec) {
			std::cerr << "Cannot add certificates : " << additionalCertificates << std::endl;
			return nullptr;
		}
	}

	boost::asio::io_context ioc;
	auto httpsClientSession = std::make_shared<HttpsClientSession>(ioc, ctx);
	std::string etpServerCapTarget = target.empty() ? "/" : target;
	if (etpServerCapTarget[etpServerCapTarget.size() - 1] != '/') {
		etpServerCapTarget += '/';
	}
	etpServerCapTarget += ".well-known/etp-server-capabilities?GetVersion=etp12.energistics.org";
	httpClientSession->run(initializationParams->getHost().c_str(), initializationParams->getPort().c_str(), etpServerCapTarget.c_str(), 11, authorization);
	// Run the I/O service. The call will return when the get operation is complete.
	ioc.run();

	preferredMaxFrameSize = getNegotiatedMaxWebSocketFramePayloadSize(httpsClientSession->getResponse().body(), preferredMaxFrameSize);

	auto result = std::make_shared<SslClientSession>(ctx, initializationParams, target.empty() ? "/" : target, authorization, preferredMaxFrameSize);
	initializationParams->postSessionCreationOperation(result.get());
	return result;
}
#endif
