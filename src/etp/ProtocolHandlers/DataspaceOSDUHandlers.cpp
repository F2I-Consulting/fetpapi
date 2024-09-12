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
#include "DataspaceOSDUHandlers.h"

#include "../AbstractSession.h"

using namespace ETP_NS;

void DataspaceOSDUHandlers::decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader& mh, avro::DecoderPtr d)
{
	if (mh.protocol != static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::DataspaceOSDU)) {
		std::cerr << "Error : This message header does not belong to the protocol DataspaceOSDU" << std::endl;
		return;
	}

	if (mh.messageType == Energistics::Etp::v12::Protocol::DataspaceOSDU::GetDataspaceInfo::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataspaceOSDU::GetDataspaceInfo obj;
		avro::decode(*d, obj);
		on_GetDataspaceInfo(obj, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataspaceOSDU::GetDataspaceInfoResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataspaceOSDU::GetDataspaceInfoResponse obj;
		avro::decode(*d, obj);
		on_GetDataspaceInfoResponse(obj, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyToDataspace::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyToDataspace obj;
		avro::decode(*d, obj);
		on_CopyToDataspace(obj, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyToDataspaceResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyToDataspaceResponse obj;
		avro::decode(*d, obj);
		on_CopyToDataspaceResponse(obj, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataspaceOSDU::LockDataspaces::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataspaceOSDU::LockDataspaces obj;
		avro::decode(*d, obj);
		on_LockDataspaces(obj, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataspaceOSDU::LockDataspacesResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataspaceOSDU::LockDataspacesResponse obj;
		avro::decode(*d, obj);
		on_LockDataspacesResponse(obj, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyDataspacesContent::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyDataspacesContent obj;
		avro::decode(*d, obj);
		on_CopyDataspacesContent(obj, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyDataspacesContentResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyDataspacesContentResponse obj;
		avro::decode(*d, obj);
		on_CopyDataspacesContentResponse(obj, mh.messageId);
	}
	else {
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(3, "The message type ID " + std::to_string(mh.messageType) + " is invalid for the Dataspace protocol."), mh.messageId, 0x02);
	}
}

void DataspaceOSDUHandlers::on_GetDataspaceInfo(const Energistics::Etp::v12::Protocol::DataspaceOSDU::GetDataspaceInfo&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataspaceHandlers::on_GetDataspaceInfo method has not been overriden by the agent."), correlationId, 0x02);
}

void DataspaceOSDUHandlers::on_GetDataspaceInfoResponse(const Energistics::Etp::v12::Protocol::DataspaceOSDU::GetDataspaceInfoResponse& msg, int64_t)
{
	for (const auto& entry : msg.dataspaces) {
		successKeys.emplace_back(entry.first);
		dataspaces.emplace_back(entry.second);
	}
}

void DataspaceOSDUHandlers::on_CopyToDataspace(const Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyToDataspace&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataspaceHandlers::on_GetDataspaces method has not been overriden by the agent."), correlationId, 0x02);
}

void DataspaceOSDUHandlers::on_CopyToDataspaceResponse(const Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyToDataspaceResponse& msg, int64_t)
{
	for (const auto& entry : msg.success) {
		successKeys.emplace_back(entry.first);
	}
}

void DataspaceOSDUHandlers::on_LockDataspaces(const Energistics::Etp::v12::Protocol::DataspaceOSDU::LockDataspaces&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataspaceHandlers::on_LockDataspaces method has not been overriden by the agent."), correlationId, 0x02);
}

void DataspaceOSDUHandlers::on_LockDataspacesResponse(const Energistics::Etp::v12::Protocol::DataspaceOSDU::LockDataspacesResponse& msg, int64_t)
{
	for (const auto& entry : msg.success) {
		successKeys.emplace_back(entry.first);
	}
}

void DataspaceOSDUHandlers::on_CopyDataspacesContent(const Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyDataspacesContent&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataspaceHandlers::on_CopyDataspacesContent method has not been overriden by the agent."), correlationId, 0x02);
}

void DataspaceOSDUHandlers::on_CopyDataspacesContentResponse(const Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyDataspacesContentResponse& msg, int64_t)
{
	for (const auto& entry : msg.success) {
		successKeys.emplace_back(entry.first);
	}
}
