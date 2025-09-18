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
#include  "StoreNotificationHandlers.h"

#include "../AbstractSession.h"
#include "../EtpHelpers.h"

#include "../../tools/date.h"

using namespace ETP_NS;

void StoreNotificationHandlers::decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader & mh, avro::DecoderPtr d)
{
	if (mh.protocol != static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification)) {
		std::cerr << "Error : This message header does not belong to the protocol Store" << std::endl;
		return;
	}

	if (mh.messageType == Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotifications::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotifications msg;
		avro::decode(*d, msg);
		on_SubscribeNotifications(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotificationsResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotificationsResponse msg;
		avro::decode(*d, msg);
		on_SubscribeNotificationsResponse(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::StoreNotification::UnsubscribeNotifications::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreNotification::UnsubscribeNotifications msg;
		avro::decode(*d, msg);
		on_UnsubscribeNotifications(msg, mh.messageId, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::StoreNotification::UnsolicitedStoreNotifications::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreNotification::UnsolicitedStoreNotifications msg;
		avro::decode(*d, msg);
		on_UnsolicitedStoreNotifications(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::StoreNotification::SubscriptionEnded::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreNotification::SubscriptionEnded msg;
		avro::decode(*d, msg);
		on_SubscriptionEnded(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::StoreNotification::ObjectChanged::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreNotification::ObjectChanged msg;
		avro::decode(*d, msg);
		on_ObjectChanged(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::StoreNotification::ObjectDeleted::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreNotification::ObjectDeleted msg;
		avro::decode(*d, msg);
		on_ObjectDeleted(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::StoreNotification::ObjectAccessRevoked::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreNotification::ObjectAccessRevoked msg;
		avro::decode(*d, msg);
		on_ObjectAccessRevoked(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::StoreNotification::ObjectActiveStatusChanged::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreNotification::ObjectActiveStatusChanged msg;
		avro::decode(*d, msg);
		on_ObjectActiveStatusChanged(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::StoreNotification::Chunk::messageTypeId) {
		Energistics::Etp::v12::Protocol::StoreNotification::Chunk msg;
		avro::decode(*d, msg);
		on_Chunk(msg, mh.messageId);
	}
	else {
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(3, "The message type ID " + std::to_string(mh.messageType) + " is invalid for the store notification protocol."), mh.messageId, 0x02);
	}
}

void StoreNotificationHandlers::on_SubscribeNotifications(const Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotifications &, int64_t)
{
	session->fesapi_log("on_SubscribeNotifications");

	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The StoreHandlers::on_SubscribeNotifications method has not been overriden by the agent."), 0x02);
}

void StoreNotificationHandlers::on_SubscribeNotificationsResponse(const Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotificationsResponse&, int64_t)
{
	session->fesapi_log("Received SubscribeNotificationsResponse");
}

void StoreNotificationHandlers::on_UnsubscribeNotifications(const Energistics::Etp::v12::Protocol::StoreNotification::UnsubscribeNotifications&, int64_t, int64_t)
{
	session->fesapi_log("on_UnsubscribeNotifications");

	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The StoreHandlers::on_UnsubscribeNotifications method has not been overriden by the agent."), 0x02);
}

void StoreNotificationHandlers::on_UnsolicitedStoreNotifications(const Energistics::Etp::v12::Protocol::StoreNotification::UnsolicitedStoreNotifications&, int64_t)
{
	session->fesapi_log("Received UnsolicitedStoreNotifications");
}

void StoreNotificationHandlers::on_SubscriptionEnded(const Energistics::Etp::v12::Protocol::StoreNotification::SubscriptionEnded&, int64_t)
{
	session->fesapi_log("Received SubscriptionEnded ");
}

void StoreNotificationHandlers::on_ObjectChanged(const Energistics::Etp::v12::Protocol::StoreNotification::ObjectChanged& msg, int64_t)
{
	switch (msg.change.changeKind) {
	case Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind::authorized: session->fesapi_log("authorized"); break;
	case Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind::insert: session->fesapi_log("insert"); break;
	case Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind::update: session->fesapi_log("update"); break;
	case Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind::joined: session->fesapi_log("joined"); break;
	case Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind::unjoined: session->fesapi_log("unjoined"); break;
	case Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind::joinedSubscription: session->fesapi_log("joinedSubscription"); break;
	case Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind::unjoinedSubscription: session->fesapi_log("unjoinedSubscription"); break;
	}

	auto duration = std::chrono::microseconds(msg.change.changeTime);
	session->fesapi_log("on", date::format("%FT%TZ", date::floor<std::chrono::microseconds>(duration)));

	printDataObject(msg.change.dataObject);
}

void StoreNotificationHandlers::on_ObjectDeleted(const Energistics::Etp::v12::Protocol::StoreNotification::ObjectDeleted &, int64_t)
{
	session->fesapi_log("on_ObjectDeleted");
}

void StoreNotificationHandlers::on_ObjectAccessRevoked(const Energistics::Etp::v12::Protocol::StoreNotification::ObjectAccessRevoked &, int64_t)
{
	session->fesapi_log("on_ObjectAccessRevoked");
}

void StoreNotificationHandlers::on_ObjectActiveStatusChanged(const Energistics::Etp::v12::Protocol::StoreNotification::ObjectActiveStatusChanged&, int64_t)
{
	session->fesapi_log("Received ObjectActiveStatusChanged");
}

void StoreNotificationHandlers::on_Chunk(const Energistics::Etp::v12::Protocol::StoreNotification::Chunk&, int64_t)
{
	session->fesapi_log("Received StoreNotification Chunk");
}
