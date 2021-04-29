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
#include  "CoreHandlers.h"

#include "../PlainServerSession.h"
#ifdef WITH_ETP_SSL
#include "../ssl/SslServerSession.h"
#endif
#include "../EtpHelpers.h"
#include "../ServerInitializationParameters.h"

using namespace ETP_NS;

void CoreHandlers::decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader & mh, avro::DecoderPtr d)
{
	if (mh.messageType != Energistics::Etp::v12::Protocol::Core::ProtocolException::messageTypeId &&
		mh.messageType != Energistics::Etp::v12::Protocol::Core::Acknowledge::messageTypeId &&
		mh.protocol != Energistics::Etp::v12::Datatypes::Protocol::Core) {
		std::cerr << "Error : This message header does not belong to the protocol Core" << std::endl;
		return;
	}

	if (mh.messageType == Energistics::Etp::v12::Protocol::Core::RequestSession::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::RequestSession rs;
		avro::decode(*d, rs);
		session->flushReceivingBuffer();
		session->setEtpSessionClosed(false);
		on_RequestSession(rs, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::OpenSession::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::OpenSession os;
		avro::decode(*d, os);
		session->flushReceivingBuffer();
		session->setEtpSessionClosed(false);

		// Check MaxWebSocketMessagePayloadSize capability
		auto search = os.endpointCapabilities.find("MaxWebSocketMessagePayloadSize");
		if (search != os.endpointCapabilities.end() &&
			search->second.item.idx() == 3) {
			const int64_t maxWebSocketMessagePayloadSize = search->second.item.get_long();
			if (maxWebSocketMessagePayloadSize > 0 &&
				maxWebSocketMessagePayloadSize < session->getMaxWebSocketMessagePayloadSize()) {
				session->setMaxWebSocketMessagePayloadSize(maxWebSocketMessagePayloadSize);
			}
		}

		on_OpenSession(os, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::CloseSession::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::CloseSession cs;
		avro::decode(*d, cs);
		session->flushReceivingBuffer();
		on_CloseSession(cs, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::ProtocolException::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::ProtocolException pe;
		avro::decode(*d, pe);
		session->flushReceivingBuffer();
		on_ProtocolException(pe, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::Acknowledge::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::Acknowledge ack;
		avro::decode(*d, ack);
		session->flushReceivingBuffer();
		on_Acknowledge(ack, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::Ping::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::Ping ping;
		avro::decode(*d, ping);
		session->flushReceivingBuffer();
		on_Ping(ping, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::Pong::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::Pong pong;
		avro::decode(*d, pong);
		session->flushReceivingBuffer();
		on_Pong(pong, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::RenewSecurityToken::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::RenewSecurityToken msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_RenewSecurityToken(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::RenewSecurityTokenResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::RenewSecurityTokenResponse msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_RenewSecurityTokenResponse(msg, mh.messageId);
	}
	else {
		session->flushReceivingBuffer();
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(3, "The message type ID " + std::to_string(mh.messageType) + " is invalid for the core protocol."), mh.messageId, 0x02);
	}
}

void CoreHandlers::on_RequestSession(const Energistics::Etp::v12::Protocol::Core::RequestSession & rs, int64_t correlationId)
{
	ServerInitializationParameters const* serverInitializationParams = nullptr;

	PlainServerSession* pss = dynamic_cast<PlainServerSession*>(session);
	if (pss != nullptr) {
		serverInitializationParams = pss->getServerInitializationParameters();
	}
#ifdef WITH_ETP_SSL
	else {
		SslServerSession* sss = dynamic_cast<SslServerSession*>(session);
		if (sss != nullptr) {
			serverInitializationParams = sss->getServerInitializationParameters();
		}
	}
#endif

	if (serverInitializationParams == nullptr) {
		std::cerr << "Request Session message must be received on a server session." << std::endl;
	}

	// Check format
	if (std::find(rs.supportedFormats.begin(), rs.supportedFormats.end(), "xml") == rs.supportedFormats.end()) {
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(21, "Per the ETP1.2 official specification, \"xml\" format MUST BE supported."), correlationId, 0x02);
		session->close();
		return;
	}

	// Check requested protocols
	auto supportedProtocols = serverInitializationParams->makeSupportedProtocols();
	std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> requestedAndSupportedProtocols;
	for (auto& rp : rs.requestedProtocols) {
		const auto validatedProtocol = std::find_if(supportedProtocols.begin(), supportedProtocols.end(),
			[rp](const Energistics::Etp::v12::Datatypes::SupportedProtocol & sp) -> bool {
				return sp.protocol == rp.protocol &&
					sp.role == rp.role &&
					sp.protocolVersion.major == rp.protocolVersion.major &&
					sp.protocolVersion.minor == rp.protocolVersion.minor &&
					sp.protocolVersion.patch == rp.protocolVersion.patch &&
					sp.protocolVersion.revision == rp.protocolVersion.revision;
			}
		);
		if (validatedProtocol != std::end(supportedProtocols)) {
			requestedAndSupportedProtocols.push_back(*validatedProtocol);
		}
	}

	if (requestedAndSupportedProtocols.empty()) {
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(2, "The server does not support any of the requested protocols."), correlationId, 0x02);
		session->close();
		return;
	}

	// Check requested dataobjects
	auto supportedDataobjects = serverInitializationParams->makeSupportedDataObjects();
	std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> requestedAndSupportedDataObjects;
	for (auto& rd : rs.supportedDataObjects) {
		const auto validatedDataObject = std::find_if(supportedDataobjects.begin(), supportedDataobjects.end(),
			[rd](const Energistics::Etp::v12::Datatypes::SupportedDataObject & sd) -> bool {
				return sd.qualifiedType == rd.qualifiedType;
			}
		);
		if (validatedDataObject != std::end(supportedDataobjects)) {
			requestedAndSupportedDataObjects.push_back(*validatedDataObject);
		}
	}

	// Check MaxWebSocketMessagePayloadSize endpoint capability
	auto supportedEndPointCaps = serverInitializationParams->makeEndpointCapabilities();
	const auto requestedMaxWebSocketMessagePayloadSizeIt = rs.endpointCapabilities.find("MaxWebSocketMessagePayloadSize");
	if (requestedMaxWebSocketMessagePayloadSizeIt != rs.endpointCapabilities.end() && requestedMaxWebSocketMessagePayloadSizeIt->second.item.idx() == 3) {
		const int64_t requestedMaxWebSocketMessagePayloadSize = requestedMaxWebSocketMessagePayloadSizeIt->second.item.get_long();
		if (requestedMaxWebSocketMessagePayloadSize > 0 && requestedMaxWebSocketMessagePayloadSize != session->getMaxWebSocketMessagePayloadSize()) {
			session->setMaxWebSocketMessagePayloadSize(requestedMaxWebSocketMessagePayloadSize);

			Energistics::Etp::v12::Datatypes::DataValue value;
			value.item.set_long(requestedMaxWebSocketMessagePayloadSize);
			supportedEndPointCaps["MaxWebSocketFramePayloadSize"] = value;
			supportedEndPointCaps["MaxWebSocketMessagePayloadSize"] = value;
		}
	}

	// Build Open Session message
	Energistics::Etp::v12::Protocol::Core::OpenSession openSession;
	openSession.applicationName = serverInitializationParams->getApplicationName();
	openSession.applicationVersion = serverInitializationParams->getApplicationVersion();
	std::copy(std::begin(session->getIdentifier().data), std::end(session->getIdentifier().data), openSession.sessionId.array.begin());
	std::copy(std::begin(serverInitializationParams->getInstanceId().data), std::end(serverInitializationParams->getInstanceId().data), openSession.serverInstanceId.array.begin());
	openSession.supportedFormats.push_back("xml");
	openSession.supportedProtocols = requestedAndSupportedProtocols;
	openSession.endpointCapabilities = supportedEndPointCaps;
	openSession.supportedDataObjects = requestedAndSupportedDataObjects;
	openSession.currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	session->send(openSession, correlationId, 0x02);

	std::cout << "A new websocket session " + to_string(session->getIdentifier()) + " has been opened";
}

void CoreHandlers::on_OpenSession(const Energistics::Etp::v12::Protocol::Core::OpenSession &, int64_t)
{
	std::cout << "The session has been opened with the default core protocol handlers. You probably want to use your own." << std::endl;
}

void CoreHandlers::on_CloseSession(const Energistics::Etp::v12::Protocol::Core::CloseSession &, int64_t)
{
	std::cout << "Close session after received request." << std::endl;
	session->sendCloseFrame();
}

void CoreHandlers::on_ProtocolException(const Energistics::Etp::v12::Protocol::Core::ProtocolException & pe, int64_t correlationId)
{
	if (pe.error) {
		std::cout << "EXCEPTION received for message_id " << correlationId << " with error code " << pe.error.get().code << " : " << pe.error.get().message << std::endl;
	}

	for (const auto& error : pe.errors) {
		std::cout << "*************************************************" << std::endl;
		std::cout << "Resource non received : " << std::endl;
		std::cout << "key : " << error.first << std::endl;
		std::cout << "message : " << error.second.message << std::endl;
		std::cout << "code : " << error.second.code << std::endl;
	}
}

void CoreHandlers::on_Acknowledge(const Energistics::Etp::v12::Protocol::Core::Acknowledge &, int64_t correlationId)
{
	std::cout << "Acknowledge message_id " << correlationId << std::endl;
}

void CoreHandlers::on_Ping(const Energistics::Etp::v12::Protocol::Core::Ping &, int64_t correlationId)
{
	Energistics::Etp::v12::Protocol::Core::Pong pong;
	pong.currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	session->send(pong, correlationId, 0x02);
}

void CoreHandlers::on_Pong(const Energistics::Etp::v12::Protocol::Core::Pong & pong, int64_t correlationId)
{
	std::cout << "Received Pong at " << pong.currentDateTime << " with message id " << correlationId << std::endl;
}

void CoreHandlers::on_RenewSecurityToken(const Energistics::Etp::v12::Protocol::Core::RenewSecurityToken &, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The Core::on_RenewSecurityToken method has not been overriden by the agent."), correlationId, 0x02);
}

void CoreHandlers::on_RenewSecurityTokenResponse(const Energistics::Etp::v12::Protocol::Core::RenewSecurityTokenResponse &, int64_t)
{
	std::cout << "Renewed token" << std::endl;
}
