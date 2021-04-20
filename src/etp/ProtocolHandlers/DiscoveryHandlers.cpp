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
#include  "DiscoveryHandlers.h"

#include "../AbstractSession.h"
#include "../EtpHelpers.h"

using namespace ETP_NS;

void DiscoveryHandlers::decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader & mh, avro::DecoderPtr d)
{
	if (mh.protocol != Energistics::Etp::v12::Datatypes::Protocol::Discovery) {
		std::cerr << "Error : This message header does not belong to the protocol Discovery" << std::endl;
		return;
	}
	if (mh.messageType == Energistics::Etp::v12::Protocol::Discovery::GetResources::messageTypeId) {
		Energistics::Etp::v12::Protocol::Discovery::GetResources msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_GetResources(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Discovery::GetResourcesResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Discovery::GetResourcesResponse msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_GetResourcesResponse(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Discovery::GetResourcesEdgesResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Discovery::GetResourcesEdgesResponse msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_GetResourcesEdgesResponse(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Discovery::GetDeletedResources::messageTypeId) {
		Energistics::Etp::v12::Protocol::Discovery::GetDeletedResources msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_GetDeletedResources(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Discovery::GetDeletedResourcesResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Discovery::GetDeletedResourcesResponse msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_GetDeletedResourcesResponse(msg, mh.correlationId);
	}
	else {
		session->flushReceivingBuffer();
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(3, "The message type ID " + std::to_string(mh.messageType) + " is invalid for the discovery protocol."), mh.messageId, 0x02);
	}
}

void DiscoveryHandlers::on_GetResources(const Energistics::Etp::v12::Protocol::Discovery::GetResources &, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The Discovery::on_GetResources method has not been overriden by the agent."), correlationId, 0x02);
}

void DiscoveryHandlers::on_GetResourcesResponse(const Energistics::Etp::v12::Protocol::Discovery::GetResourcesResponse & msg, int64_t)
{
	for (const auto & resource : msg.resources) {
		std::cout << "DISCOVERED RESOURCE (" << resource.name << ", " << resource.dataObjectType << ')' << std::endl;
	}
}

void DiscoveryHandlers::on_GetResourcesEdgesResponse(const Energistics::Etp::v12::Protocol::Discovery::GetResourcesEdgesResponse & msg, int64_t)
{
	for (const auto & edge : msg.edges) {
		std::cout << "DISCOVERED EDGE (" << edge.sourceUri << ", " << edge.relationshipKind << ", " << edge.targetUri << ')' << std::endl;
	}
}

void DiscoveryHandlers::on_GetDeletedResources(const Energistics::Etp::v12::Protocol::Discovery::GetDeletedResources &, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The Discovery::on_GetDeletedResources method has not been overriden by the agent."), correlationId, 0x02);
}

void DiscoveryHandlers::on_GetDeletedResourcesResponse(const Energistics::Etp::v12::Protocol::Discovery::GetDeletedResourcesResponse & msg, int64_t)
{
	for (const auto & resource : msg.deletedResources) {
		std::cout << "DISCOVERED DELETED RESOURCE (" << resource.uri << " deleted at " << resource.deletedTime << ')' << std::endl;
	}
}
