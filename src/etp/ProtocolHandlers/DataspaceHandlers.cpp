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
#include "DataspaceHandlers.h"

#include "../AbstractSession.h"
#include "../EtpHelpers.h"

using namespace ETP_NS;

void DataspaceHandlers::decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader& mh, avro::DecoderPtr d)
{
	if (mh.protocol != static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace)) {
		std::cerr << "Error : This message header does not belong to the protocol Dataspace" << std::endl;
		return;
	}

	if (mh.messageType == Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces::messageTypeId) {
		Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces getO;
		avro::decode(*d, getO);
		session->flushReceivingBuffer();
		on_GetDataspaces(getO, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Dataspace::GetDataspacesResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Dataspace::GetDataspacesResponse obj;
		avro::decode(*d, obj);
		session->flushReceivingBuffer();
		on_GetDataspacesResponse(obj, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces::messageTypeId) {
		Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces putO;
		avro::decode(*d, putO);
		session->flushReceivingBuffer();
		on_PutDataspaces(putO, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Dataspace::PutDataspacesResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Dataspace::PutDataspacesResponse msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_PutDataspacesResponse(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces::messageTypeId) {
		Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces deleteO;
		avro::decode(*d, deleteO);
		session->flushReceivingBuffer();
		on_DeleteDataspaces(deleteO, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspacesResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspacesResponse msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_DeleteDataspacesResponse(msg, mh.messageId);
	}
	else {
		session->flushReceivingBuffer();
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(3, "The message type ID " + std::to_string(mh.messageType) + " is invalid for the Dataspace protocol."), mh.messageId, 0x02);
	}
}

void DataspaceHandlers::on_GetDataspaces(const Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataspaceHandlers::on_GetDataspaces method has not been overriden by the agent."), correlationId, 0x02);
}

void DataspaceHandlers::on_GetDataspacesResponse(const Energistics::Etp::v12::Protocol::Dataspace::GetDataspacesResponse& msg, int64_t)
{
	for (const auto& ds : msg.dataspaces) {
		std::cout << "dataspace uri received : " << ds.uri << std::endl;
		std::cout << "dataspace title received : " << ds.path << std::endl;
	}
}

void DataspaceHandlers::on_PutDataspaces(const Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataspaceHandlers::on_PutDataObject method has not been overriden by the agent."), correlationId, 0x02);
}

void DataspaceHandlers::on_PutDataspacesResponse(const Energistics::Etp::v12::Protocol::Dataspace::PutDataspacesResponse&, int64_t)
{
	std::cout << "on_PutDataspacesResponse" << std::endl;
}

void DataspaceHandlers::on_DeleteDataspaces(const Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataspaceHandlers::on_DeleteDataObject method has not been overriden by the agent."), correlationId, 0x02);
}

void DataspaceHandlers::on_DeleteDataspacesResponse(const Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspacesResponse&, int64_t)
{
	std::cout << "on_DeleteDataspacesResponse" << std::endl;
}
