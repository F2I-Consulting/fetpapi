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
#include "StoreOSDUHandlers.h"

#include "../AbstractSession.h"

using namespace ETP_NS;

void StoreOSDUHandlers::decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader& mh, avro::DecoderPtr d)
{
	if (mh.protocol != static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::StoreOSDU)) {
		std::cerr << "Error : This message header does not belong to the protocol StoreOSDU" << std::endl;
		return;
	}

	if (mh.messageType == Energistics::Etp::v12::Protocol::StoreOSDU::CopyDataObjectsByValue::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreOSDU::CopyDataObjectsByValue obj;
		avro::decode(*d, obj);
		on_CopyDataObjectsByValue(obj, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::StoreOSDU::CopyDataObjectsByValueResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreOSDU::CopyDataObjectsByValueResponse obj;
		avro::decode(*d, obj);
		on_CopyDataObjectsByValueResponse(obj, mh.messageId);
	}
	else {
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(3, "The message type ID " + std::to_string(mh.messageType) + " is invalid for the Dataspace protocol."), mh.messageId, 0x02);
	}
}

void StoreOSDUHandlers::on_CopyDataObjectsByValue(const Energistics::Etp::v12::Protocol::StoreOSDU::CopyDataObjectsByValue&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataspaceHandlers::on_CopyDataObjectsByValue method has not been overriden by the agent."), correlationId, 0x02);
}

void StoreOSDUHandlers::on_CopyDataObjectsByValueResponse(const Energistics::Etp::v12::Protocol::StoreOSDU::CopyDataObjectsByValueResponse& msg, int64_t)
{
	for (const auto& copiedDataObject : msg.copiedDataObjects) {
		copiedDataobjects.emplace_back(copiedDataObject);
	}
}
