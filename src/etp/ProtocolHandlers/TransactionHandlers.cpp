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
#include "TransactionHandlers.h"

#include "../AbstractSession.h"
#include "../EtpHelpers.h"

using namespace ETP_NS;

void TransactionHandlers::decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader& mh, avro::DecoderPtr d)
{
	if (mh.protocol != static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Transaction)) {
		std::cerr << "Error : This message header does not belong to the protocol Transaction" << std::endl;
		return;
	}

	if (mh.messageType == Energistics::Etp::v12::Protocol::Transaction::StartTransaction::messageTypeId) {
		Energistics::Etp::v12::Protocol::Transaction::StartTransaction getO;
		avro::decode(*d, getO);
		session->flushReceivingBuffer();
		on_StartTransaction(getO, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Transaction::StartTransactionResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Transaction::StartTransactionResponse obj;
		avro::decode(*d, obj);
		session->flushReceivingBuffer();
		on_StartTransactionResponse(obj, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Transaction::CommitTransaction::messageTypeId) {
		Energistics::Etp::v12::Protocol::Transaction::CommitTransaction putO;
		avro::decode(*d, putO);
		session->flushReceivingBuffer();
		on_CommitTransaction(putO, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Transaction::CommitTransactionResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Transaction::CommitTransactionResponse msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_CommitTransactionResponse(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction::messageTypeId) {
		Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction deleteO;
		avro::decode(*d, deleteO);
		session->flushReceivingBuffer();
		on_RollbackTransaction(deleteO, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::Transaction::RollbackTransactionResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::Transaction::RollbackTransactionResponse msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_RollbackTransactionResponse(msg, mh.messageId);
	}
	else {
		session->flushReceivingBuffer();
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(3, "The message type ID " + std::to_string(mh.messageType) + " is invalid for the Transaction protocol."), mh.messageId, 0x02);
	}
}

void TransactionHandlers::on_StartTransaction(const Energistics::Etp::v12::Protocol::Transaction::StartTransaction&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The TransactionHandlers::on_StartTransaction method has not been overriden by the agent."), correlationId, 0x02);
}

void TransactionHandlers::on_StartTransactionResponse(const Energistics::Etp::v12::Protocol::Transaction::StartTransactionResponse& msg, int64_t)
{
	if (isInAnActiveTransaction()) {
		throw std::logic_error("ETP1.2 intentionally supports a single open transaction on a session");
	}

	if (msg.successful) {
		transactionUuid = msg.transactionUuid;
	}
	else {
		lastTransactionFailure = msg.failureReason;
	}
}

void TransactionHandlers::on_CommitTransaction(const Energistics::Etp::v12::Protocol::Transaction::CommitTransaction&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The TransactionHandlers::on_PutDataObject method has not been overriden by the agent."), correlationId, 0x02);
}

void TransactionHandlers::on_CommitTransactionResponse(const Energistics::Etp::v12::Protocol::Transaction::CommitTransactionResponse& msg, int64_t)
{
	if (msg.successful) {
		std::fill(transactionUuid.array.begin(), transactionUuid.array.end(), 0);
	}
	else {
		lastTransactionFailure = msg.failureReason;
	}
}

void TransactionHandlers::on_RollbackTransaction(const Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The TransactionHandlers::on_DeleteDataObject method has not been overriden by the agent."), correlationId, 0x02);
}

void TransactionHandlers::on_RollbackTransactionResponse(const Energistics::Etp::v12::Protocol::Transaction::RollbackTransactionResponse& msg, int64_t)
{
	if (msg.successful) {
		std::fill(transactionUuid.array.begin(), transactionUuid.array.end(), 0);
	}
	else {
		lastTransactionFailure = msg.failureReason;
	}
}
