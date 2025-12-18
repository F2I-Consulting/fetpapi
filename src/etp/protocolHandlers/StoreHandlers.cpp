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
#include "StoreHandlers.h"

#include "../AbstractSession.h"
#include "../EtpHelpers.h"

using namespace ETP_NS;

void StoreHandlers::decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader& mh, avro::DecoderPtr d)
{
	if (mh.protocol != static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Store)) {
		std::cerr << "Error : This message header does not belong to the protocol Store" << std::endl;
		return;
	}

	if (mh.messageType == Energistics::Etp::v12::Protocol::Store::GetDataObjects::messageTypeId) {
		Energistics::Etp::v12::Protocol::Store::GetDataObjects getO;
		avro::decode(*d, getO);
		on_GetDataObjects(getO, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Store::GetDataObjectsResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Store::GetDataObjectsResponse obj;
		avro::decode(*d, obj);
		on_GetDataObjectsResponse(obj, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Store::PutDataObjects::messageTypeId) {
		Energistics::Etp::v12::Protocol::Store::PutDataObjects putO;
		avro::decode(*d, putO);
		on_PutDataObjects(putO, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Store::PutDataObjectsResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Store::PutDataObjectsResponse msg;
		avro::decode(*d, msg);
		on_PutDataObjectsResponse(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Store::DeleteDataObjects::messageTypeId) {
		Energistics::Etp::v12::Protocol::Store::DeleteDataObjects deleteO;
		avro::decode(*d, deleteO);
		on_DeleteDataObjects(deleteO, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Store::DeleteDataObjectsResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Store::DeleteDataObjectsResponse msg;
		avro::decode(*d, msg);
		on_DeleteDataObjectsResponse(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Store::Chunk::messageTypeId) {
		Energistics::Etp::v12::Protocol::Store::Chunk msg;
		avro::decode(*d, msg);
		on_Chunk(msg, mh.messageId);
	}
	else {
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(3, "The message type ID " + std::to_string(mh.messageType) + " is invalid for the store protocol."), mh.messageId, 0x02);
	}
}

void StoreHandlers::on_GetDataObjects(const Energistics::Etp::v12::Protocol::Store::GetDataObjects&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The StoreHandlers::on_GetDataObjects method has not been overriden by the agent."), correlationId, 0x02);
}

void StoreHandlers::on_GetDataObjectsResponse(const Energistics::Etp::v12::Protocol::Store::GetDataObjectsResponse& msg, int64_t)
{
	dataObjects.insert(msg.dataObjects.begin(), msg.dataObjects.end());
}

void StoreHandlers::on_PutDataObjects(const Energistics::Etp::v12::Protocol::Store::PutDataObjects&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The StoreHandlers::on_PutDataObject method has not been overriden by the agent."), correlationId, 0x02);
}

void StoreHandlers::on_PutDataObjectsResponse(const Energistics::Etp::v12::Protocol::Store::PutDataObjectsResponse& msg, int64_t)
{
	for (const auto& entry : msg.success) {
		successKeys.push_back(entry.first);
	}
}

void StoreHandlers::on_DeleteDataObjects(const Energistics::Etp::v12::Protocol::Store::DeleteDataObjects&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The StoreHandlers::on_DeleteDataObject method has not been overriden by the agent."), correlationId, 0x02);
}

void StoreHandlers::on_DeleteDataObjectsResponse(const Energistics::Etp::v12::Protocol::Store::DeleteDataObjectsResponse& msg, int64_t)
{
	for (const auto& entry : msg.deletedUris) {
		successKeys.push_back(entry.first);
	}
}

void StoreHandlers::on_Chunk(const Energistics::Etp::v12::Protocol::Store::Chunk&, int64_t)
{
	session->fesapi_log("Received Store Chunk");
}
