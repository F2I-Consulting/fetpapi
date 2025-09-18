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

#include "../AbstractSession.h"
#include "../EtpHelpers.h"

using namespace ETP_NS;

void CoreHandlers::decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader & mh, avro::DecoderPtr d)
{
	if (mh.messageType != Energistics::Etp::v12::Protocol::Core::ProtocolException::messageTypeId &&
		mh.messageType != Energistics::Etp::v12::Protocol::Core::Acknowledge::messageTypeId &&
		mh.protocol != static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Core)) {
		std::cerr << "Error : This message header does not belong to the protocol Core" << std::endl;
		return;
	}

	if (mh.messageType == Energistics::Etp::v12::Protocol::Core::RequestSession::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::RequestSession rs;
		avro::decode(*d, rs);
		session->setEtpSessionClosed(false);
		on_RequestSession(rs, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::OpenSession::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::OpenSession os;
		avro::decode(*d, os);

		// Check MaxWebSocketMessagePayloadSize capability
		auto search = os.endpointCapabilities.find("MaxWebSocketMessagePayloadSize");
		if (search != os.endpointCapabilities.end() &&
			search->second.item.idx() == 3) {
			const uint64_t maxWebSocketMessagePayloadSize = search->second.item.get_long();
			if (maxWebSocketMessagePayloadSize > 0 &&
				maxWebSocketMessagePayloadSize < session->getMaxWebSocketMessagePayloadSize()) {
				session->setMaxWebSocketMessagePayloadSize(maxWebSocketMessagePayloadSize);
			}
		}

		session->setEtpSessionClosed(false);
		{
			std::lock_guard<std::mutex> lock(session->identifierMutex);
			std::copy(os.sessionId.array.begin(), os.sessionId.array.end(), session->identifier.begin());
		}
		on_OpenSession(os, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::CloseSession::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::CloseSession cs;
		avro::decode(*d, cs);
		session->setEtpSessionClosed(true);
		on_CloseSession(cs, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::ProtocolException::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::ProtocolException pe;
		avro::decode(*d, pe);
		on_ProtocolException(pe, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::Acknowledge::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::Acknowledge ack;
		avro::decode(*d, ack);
		on_Acknowledge(ack, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::Ping::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::Ping ping;
		avro::decode(*d, ping);
		on_Ping(ping, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::Pong::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::Pong pong;
		avro::decode(*d, pong);
		on_Pong(pong, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::Authorize::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::Authorize msg;
		avro::decode(*d, msg);
		on_Authorize(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Core::AuthorizeResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Core::AuthorizeResponse msg;
		avro::decode(*d, msg);
		on_AuthorizeResponse(msg, mh.messageId);
	}
	else {
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(3, "The message type ID " + std::to_string(mh.messageType) + " is invalid for the core protocol."), mh.messageId, 0x02);
	}
}

void CoreHandlers::on_RequestSession(const Energistics::Etp::v12::Protocol::Core::RequestSession&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(1, "You cannot request a session to a client."), correlationId, 0x02);
}

void CoreHandlers::on_OpenSession(const Energistics::Etp::v12::Protocol::Core::OpenSession &, int64_t)
{
	session->fesapi_log("The session", session->getIdentifier(), "has been opened with the default core protocol handlers.");
}

void CoreHandlers::on_CloseSession(const Energistics::Etp::v12::Protocol::Core::CloseSession &, int64_t)
{
	session->fesapi_log("Close session after receiving ETP message to close session.");
	session->do_close();
}

void CoreHandlers::on_ProtocolException(const Energistics::Etp::v12::Protocol::Core::ProtocolException & pe, int64_t correlationId)
{
	std::cerr << "EXCEPTION received for message_id " << correlationId << std::endl;
	if (pe.error) {
		std::cerr << "Single error code " << pe.error.get().code << " : " << pe.error.get().message << std::endl;
	}
	else {
		std::cerr << "One or more error code : " << std::endl;
		for (const auto& error : pe.errors) {
			std::cerr << "*************************************************" << std::endl;
			std::cerr << "Resource non received : " << std::endl;
			std::cerr << "key : " << error.first << std::endl;
			std::cerr << "message : " << error.second.message << std::endl;
			std::cerr << "code : " << error.second.code << std::endl;
		}
	}
}

void CoreHandlers::on_Acknowledge(const Energistics::Etp::v12::Protocol::Core::Acknowledge &, int64_t correlationId)
{
	session->fesapi_log("Acknowledge message_id", std::to_string(correlationId));
}

void CoreHandlers::on_Ping(const Energistics::Etp::v12::Protocol::Core::Ping &, int64_t correlationId)
{
	Energistics::Etp::v12::Protocol::Core::Pong pong;
	pong.currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	session->send(pong, correlationId, 0x02);
}

void CoreHandlers::on_Pong(const Energistics::Etp::v12::Protocol::Core::Pong & pong, int64_t correlationId)
{
	session->fesapi_log("Received Pong at", std::to_string(pong.currentDateTime), "with message id", std::to_string(correlationId));
}

void CoreHandlers::on_Authorize(const Energistics::Etp::v12::Protocol::Core::Authorize &, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The Core::on_Authorize method has not been overriden by the agent."), correlationId, 0x02);
}

void CoreHandlers::on_AuthorizeResponse(const Energistics::Etp::v12::Protocol::Core::AuthorizeResponse &, int64_t)
{
	session->fesapi_log("Renewed token");
}
