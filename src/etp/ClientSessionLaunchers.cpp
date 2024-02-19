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
#include "PlainClientSession.h"

#ifdef WITH_ETP_SSL
#include "ssl/HttpsClientSession.h"
#include "ssl/SslClientSession.h"
#endif

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

		protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Core);
		protocol.protocolVersion = protocolVersion;
		protocol.role = "server";
		requestedProtocols.push_back(protocol);

		protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
		protocol.protocolVersion = protocolVersion;
		protocol.role = "store";
		requestedProtocols.push_back(protocol);

		protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Store);
		protocol.protocolVersion = protocolVersion;
		protocol.role = "store";
		requestedProtocols.push_back(protocol);

		protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
		protocol.protocolVersion = protocolVersion;
		protocol.role = "store";
		requestedProtocols.push_back(protocol);

		protocol.protocol = static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
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

		supportedDataObject.qualifiedType = "eml20.obj_EpcExternalPartReference";
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

	std::size_t getNegotiatedMaxWebSocketFramePayloadSize(const std::string& responseBody, std::size_t preferredMaxFrameSize) {
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

std::shared_ptr<ETP_NS::ClientSession> ETP_NS::ClientSessionLaunchers::createClientSession(InitializationParameters* initializationParams,
	const std::string& authorization, const std::string& proxyAuthorization)
{
	boost::asio::io_context ioc;

	std::string etpServerCapTarget = "/" + initializationParams->getEtpServerUrlPath();
	if (etpServerCapTarget.back() != '/') {
		etpServerCapTarget += '/';
	}
	etpServerCapTarget += ".well-known/etp-server-capabilities?GetVersion=etp12.energistics.org";

#ifdef WITH_ETP_SSL
	if (initializationParams->getEtpServerPort() == 443 || initializationParams->isTlsForced()) {
		// The SSL context is required, and holds certificates
		boost::asio::ssl::context ctx{ boost::asio::ssl::context::sslv23_client };
		ctx.set_default_verify_paths();
		ctx.set_options(
			boost::asio::ssl::context::default_workarounds
			| boost::asio::ssl::context::no_sslv2
			| boost::asio::ssl::context::no_sslv3
			| boost::asio::ssl::context::single_dh_use
		);

		const std::string& additionalCertificates = initializationParams->getAdditionalCertificates();
		if (!additionalCertificates.empty()) {
			boost::system::error_code ec;
			ctx.add_certificate_authority(
				boost::asio::buffer(additionalCertificates.data(), additionalCertificates.size()), ec);
			if (ec) {
				std::cerr << "Cannot add certificates : " << additionalCertificates << std::endl;
				return nullptr;
			}
		}
		auto restClientSession = std::make_shared<HttpsClientSession>(ioc, ctx);
		restClientSession->run(
			initializationParams->getEtpServerHost(), initializationParams->getEtpServerPort(), etpServerCapTarget, 11, authorization,
			initializationParams->getProxyHost(), initializationParams->getProxyPort(), proxyAuthorization);
		// Run the I/O service. The call will return when the get operation is complete.
		ioc.run();

		std::size_t preferredMaxFrameSize = getNegotiatedMaxWebSocketFramePayloadSize(restClientSession->getResponse().body(), initializationParams->getPreferredMaxFrameSize());

		auto result = std::make_shared<SslClientSession>(ctx, initializationParams, "/" + initializationParams->getEtpServerUrlPath(),
			authorization, proxyAuthorization,
			initializationParams->getAdditionalHandshakeHeaderFields(), preferredMaxFrameSize);
		initializationParams->postSessionCreationOperation(result.get());
		return result;
	}
	else {
#endif
		auto restClientSession = std::make_shared<HttpClientSession>(ioc);
		restClientSession->run(
			initializationParams->getEtpServerHost(), initializationParams->getEtpServerPort(), etpServerCapTarget, 11, authorization,
			initializationParams->getProxyHost(), initializationParams->getProxyPort(), proxyAuthorization);
		// Run the I/O service. The call will return when the get operation is complete.
		ioc.run();

		std::size_t preferredMaxFrameSize = getNegotiatedMaxWebSocketFramePayloadSize(restClientSession->getResponse().body(), initializationParams->getPreferredMaxFrameSize());

		auto result = std::make_shared<PlainClientSession>(initializationParams, "/" + initializationParams->getEtpServerUrlPath(),
			authorization, proxyAuthorization,
			initializationParams->getAdditionalHandshakeHeaderFields(), preferredMaxFrameSize);
		initializationParams->postSessionCreationOperation(result.get());
		return result;
#ifdef WITH_ETP_SSL
	}
#endif
}
