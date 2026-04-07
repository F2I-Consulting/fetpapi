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
#include "CoreOSDUHandlers.h"

#include "../AbstractSession.h"

using namespace ETP_NS;

void CoreOSDUHandlers::decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader& mh, avro::DecoderPtr d)
{
	if (mh.protocol != static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::CoreOSDU)) {
		std::cerr << "Error : This message header does not belong to the protocol CoreOSDU" << std::endl;
		return;
	}

	if (mh.messageType == Energistics::Etp::v12::Protocol::CoreOSDU::ResumeSession::messageTypeId) {
		Energistics::Etp::v12::Protocol::CoreOSDU::ResumeSession msg;
		msg.decode(*d);
		on_ResumeSession(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::CoreOSDU::ResumeSessionResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::CoreOSDU::ResumeSessionResponse msg;
		msg.decode(*d);
		session->setEtpSessionClosed(false);
		{
			std::lock_guard<std::mutex> lock(session->identifierMutex);
			std::copy(msg.sessionId.array.begin(), msg.sessionId.array.end(), session->identifier.begin());
		}
		on_ResumeSessionResponse(msg, mh.messageId);
	}
	else {
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(3, "The message type ID " + std::to_string(mh.messageType) + " is invalid for the CoreOSDU protocol."), mh.messageId, 0x02);
	}
}

void CoreOSDUHandlers::on_ResumeSession(const Energistics::Etp::v12::Protocol::CoreOSDU::ResumeSession&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(1, "You cannot resume a session to a client."), correlationId, 0x02);
}

void CoreOSDUHandlers::on_ResumeSessionResponse(const Energistics::Etp::v12::Protocol::CoreOSDU::ResumeSessionResponse&, int64_t)
{
	session->fesapi_log("The session", session->getIdentifier(), "has been resumed.");
}
