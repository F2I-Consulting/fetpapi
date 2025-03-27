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
#include "AbstractSession.h"

#include "EtpHelpers.h"

using namespace ETP_NS;

Energistics::Etp::v12::Datatypes::MessageHeader AbstractSession::decodeMessageHeader(avro::DecoderPtr decoder)
{
	Energistics::Etp::v12::Datatypes::MessageHeader receivedMh;
	avro::decode(*decoder, receivedMh);

	fesapi_log("Message Header received :");
	fesapi_log("protocol :", std::to_string(receivedMh.protocol));
	fesapi_log("type :", std::to_string(receivedMh.messageType));
	fesapi_log("id :", std::to_string(receivedMh.messageId));
	fesapi_log("correlation id :", std::to_string(receivedMh.correlationId));
	fesapi_log("flags :", std::to_string(receivedMh.messageFlags));

	return receivedMh;
}

void AbstractSession::on_read(boost::system::error_code ec, std::size_t bytes_transferred)
{
	boost::ignore_unused(bytes_transferred);

	if (ec) {

		if (ec == websocket::error::closed) {
			// This indicates that the web socket (and consequently etp) session was closed
			fesapi_log("The other endpoint closed the web socket (and consequently etp) connection.");
			webSocketSessionClosed = true;
			flushReceivingBuffer();
		}
		else {
			// This indicates an unexpected error
			fesapi_log("on_read : error code number", ec.value(), "->", ec.message());
			// This error may be a common one to ignore in case of SSL short read : https://github.com/boostorg/beast/issues/824
			if (etpSessionClosed) {
				fesapi_log("It looks that the other endpoint has closed the websocket session in a non graceful way.");
				webSocketSessionClosed = true;
				flushReceivingBuffer();
			}
		}

		return;
	}

	if (bytes_transferred == 0) {
		return;
	}

	fesapi_log("Receiving", std::to_string(bytes_transferred), "bytes");
	avro::InputStreamPtr in = avro::memoryInputStream(static_cast<const uint8_t*>(receivedBuffer.data().data()), bytes_transferred);
	avro::DecoderPtr d = avro::binaryDecoder();
	d->init(*in);

	Energistics::Etp::v12::Datatypes::MessageHeader receivedMh;
	try {
		receivedMh = decodeMessageHeader(d);
	}
	catch (avro::Exception& e)
	{
		flushReceivingBuffer();
		send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(19, "The agent is unable to de-serialize the header of the received message : " + std::string(e.what())), 0, 0x02);
		do_read();
		return;
	}

	// Request for Acknowledge
	if ((receivedMh.messageFlags & 0x10) != 0) {
		Energistics::Etp::v12::Protocol::Core::Acknowledge acknowledge;
		acknowledge.protocolId = receivedMh.protocol;
		send(acknowledge, receivedMh.messageId, 0x02);
	}

	try {
		if (receivedMh.messageType == Energistics::Etp::v12::Protocol::Core::Acknowledge::messageTypeId) {
			// Receive Acknowledge
			protocolHandlers[static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Core)]->decodeMessageBody(receivedMh, d);
		}
		else if (receivedMh.messageType == Energistics::Etp::v12::Protocol::Core::ProtocolException::messageTypeId) {
			// Receive Protocol Exception
			protocolHandlers[static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Core)]->decodeMessageBody(receivedMh, d);
			if ((receivedMh.messageFlags & 0x02) != 0) {
				const std::lock_guard<std::mutex> specificProtocolHandlersLock(specificProtocolHandlersMutex);
				auto specificProtocolHandlerIt = specificProtocolHandlers.find(receivedMh.correlationId);
				if (specificProtocolHandlerIt != specificProtocolHandlers.end()) {
					specificProtocolHandlers.erase(specificProtocolHandlerIt);
				}
			}
		}
		else {
			std::shared_ptr<ETP_NS::ProtocolHandlers> specificProtocolHandler;
			{
				const std::lock_guard<std::mutex> specificProtocolHandlersLock(specificProtocolHandlersMutex);
				auto specificProtocolHandlerIt = specificProtocolHandlers.find(receivedMh.correlationId);
				if (specificProtocolHandlerIt != specificProtocolHandlers.end()) {
					specificProtocolHandler = specificProtocolHandlerIt->second;
				}
			} // Scope for specificProtocolHandlersLock

			size_t receivedMhProtocol = static_cast<size_t>(receivedMh.protocol);
			if (specificProtocolHandler) {
				// Receive a message which has been asked to be processed with a specific protocol handler
				specificProtocolHandler->decodeMessageBody(receivedMh, d);
				if ((receivedMh.messageFlags & 0x02) != 0) {
					const std::lock_guard<std::mutex> specificProtocolHandlersLock(specificProtocolHandlersMutex);
					auto specificProtocolHandlerIt = specificProtocolHandlers.find(receivedMh.correlationId);
					specificProtocolHandlers.erase(specificProtocolHandlerIt);
				}
			}
			else {
				auto normalProtocolHandlerIt = protocolHandlers.find(receivedMhProtocol);
				if (normalProtocolHandlerIt != protocolHandlers.end()) {
					// Receive a message to be processed with a common protocol handler in case for example an unsollicited notification
					normalProtocolHandlerIt->second->decodeMessageBody(receivedMh, d);
				}
				else {
					std::cerr << "Received a message with id " << receivedMh.messageId << " for which non protocol handlers is associated. Protocol " << receivedMhProtocol << std::endl;
					send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(4, "The agent does not support the protocol " + std::to_string(receivedMhProtocol) + " identified in a message header."), receivedMh.messageId, 0x02);
				}
			}
		}
		flushReceivingBuffer();
	}
	catch (avro::Exception& e)
	{
		flushReceivingBuffer();
		send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(19, "The agent is unable to de-serialize the body of the message id " + std::to_string(receivedMh.messageId) + " : " + std::string(e.what())), 0, 0x02);
	}

	if (specificProtocolHandlers.empty() && isCloseRequested)
	{
		etpSessionClosed = true;
		send(Energistics::Etp::v12::Protocol::Core::CloseSession(), 0, 0x02);
	}

	do_read();
}

/****************
*** DATASPACE ***
****************/

std::vector<Energistics::Etp::v12::Datatypes::Object::Dataspace> AbstractSession::getDataspaces(int64_t storeLastWriteFilter)
{
	std::shared_ptr<DataspaceHandlers> handlers = getDataspaceProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any dataspace protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces msg;
	if (storeLastWriteFilter >= 0) {
		msg.storeLastWriteFilter = storeLastWriteFilter;
	}
	sendAndBlock(msg, 0, 0x02);
	std::vector<Energistics::Etp::v12::Datatypes::Object::Dataspace> result = handlers->getDataspaces();
	handlers->clearDataspaces();
	return result;
}

std::vector<std::string> AbstractSession::putDataspaces(const std::map<std::string, Energistics::Etp::v12::Datatypes::Object::Dataspace>& dataspaces)
{
	std::shared_ptr<DataspaceHandlers> handlers = getDataspaceProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any dataspace protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces msg;
	msg.dataspaces = dataspaces;
	sendAndBlock(msg, 0, 0x02);
	std::vector<std::string> result = handlers->getSuccessKeys();
	handlers->clearSuccessKeys();
	return result;
}

std::vector<std::string> AbstractSession::deleteDataspaces(const std::map<std::string, std::string>& dataspaceUris)
{
	std::shared_ptr<DataspaceHandlers> handlers = getDataspaceProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any dataspace protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces msg;
	msg.uris = dataspaceUris;
	sendAndBlock(msg, 0, 0x02);
	std::vector<std::string> result = handlers->getSuccessKeys();
	handlers->clearSuccessKeys();
	return result;
}



/*********************
*** DATASPACE OSDU ***
**********************/

std::vector<Energistics::Etp::v12::Datatypes::Object::Dataspace> AbstractSession::getDataspaceInfo(const std::map<std::string, std::string>& dataspaceUris)
{
	std::shared_ptr<DataspaceOSDUHandlers> handlers = getDataspaceOSDUProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any dataspace OSDU protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::DataspaceOSDU::GetDataspaceInfo msg;
	msg.uris = dataspaceUris;
	sendAndBlock(msg, 0, 0x02);
	std::vector<Energistics::Etp::v12::Datatypes::Object::Dataspace> result = handlers->getDataspaces();
	handlers->clearDataspaces();
	handlers->clearSuccessKeys();
	return result;
}

std::vector<std::string> AbstractSession::copyDataspacesContent(const std::map<std::string, std::string>& sourceDataspaceUris, const std::string& targetDataspaceUri)
{
	std::shared_ptr<DataspaceOSDUHandlers> handlers = getDataspaceOSDUProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any dataspace OSDU protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyDataspacesContent msg;
	msg.dataspaces = sourceDataspaceUris;
	msg.targetDataspace = targetDataspaceUri;
	sendAndBlock(msg, 0, 0x02);
	std::vector<std::string> result = handlers->getSuccessKeys();
	handlers->clearSuccessKeys();
	return result;
}

std::vector<std::string> AbstractSession::lockDataspaces(const std::map<std::string, std::string>& dataspaceUris, bool lock)
{
	std::shared_ptr<DataspaceOSDUHandlers> handlers = getDataspaceOSDUProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any dataspace OSDU protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::DataspaceOSDU::LockDataspaces msg;
	msg.uris = dataspaceUris;
	msg.lock = lock;
	sendAndBlock(msg, 0, 0x02);
	std::vector<std::string> result = handlers->getSuccessKeys();
	handlers->clearSuccessKeys();
	return result;
}

std::vector<std::string> AbstractSession::copyToDataspace(const std::map<std::string, std::string>& sourceUris, const std::string& targetDataspaceUri)
{
	std::shared_ptr<DataspaceOSDUHandlers> handlers = getDataspaceOSDUProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any dataspace OSDU protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::DataspaceOSDU::CopyToDataspace msg;
	msg.uris = sourceUris;
	msg.dataspaceUri = targetDataspaceUri;
	sendAndBlock(msg, 0, 0x02);
	std::vector<std::string> result = handlers->getSuccessKeys();
	handlers->clearSuccessKeys();
	return result;
}

/****************
*** DISCOVERY ***
****************/

std::vector<Energistics::Etp::v12::Datatypes::Object::Resource> AbstractSession::getResources(
	const Energistics::Etp::v12::Datatypes::Object::ContextInfo& context,
	const Energistics::Etp::v12::Datatypes::Object::ContextScopeKind& scope,
	int64_t storeLastWriteFilter,
	bool countObjects)
{
	std::shared_ptr<DiscoveryHandlers> handlers = getDiscoveryProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any discovery protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::Discovery::GetResources msg;
	msg.context = context;
	msg.scope = scope;
	if (storeLastWriteFilter >= 0) {
		msg.storeLastWriteFilter = storeLastWriteFilter;
	}
	msg.countObjects = countObjects;
	sendAndBlock(msg, 0, 0x02);
	std::vector<Energistics::Etp::v12::Datatypes::Object::Resource> result = handlers->getResources();
	handlers->clearResources();
	return result;
}

std::vector<Energistics::Etp::v12::Datatypes::Object::DeletedResource> AbstractSession::getDeletedResources(
	const std::string& dataspaceUri,
	int64_t deleteTimeFilter,
	const std::vector<std::string>& dataObjectTypes)
{
	std::shared_ptr<DiscoveryHandlers> handlers = getDiscoveryProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any discovery protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::Discovery::GetDeletedResources msg;
	msg.dataspaceUri = dataspaceUri;
	if (deleteTimeFilter >= 0) {
		msg.deleteTimeFilter = deleteTimeFilter;
	}
	msg.dataObjectTypes = dataObjectTypes;
	sendAndBlock(msg, 0, 0x02);
	std::vector<Energistics::Etp::v12::Datatypes::Object::DeletedResource> result = handlers->getDeletedResources();
	handlers->clearDeletedResources();
	return result;
}

/****************
***** STORE *****
****************/

std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> AbstractSession::getDataObjects(const std::map<std::string, std::string>& uris)
{
	std::shared_ptr<StoreHandlers> handlers = getStoreProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any store protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::Store::GetDataObjects msg;
	msg.uris = uris;
	msg.format = "xml";
	sendAndBlock(msg, 0, 0x02);
	std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> result = handlers->getDataObjects();
	handlers->clearDataObjects();
	return result;
}

std::vector<std::string> AbstractSession::putDataObjects(const std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject>& dataObjects)
{
	std::shared_ptr<StoreHandlers> handlers = getStoreProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any store protocol handlers.");
	}

	// 50 bytes is the maximum (10 bytes by variable length integer, see https://protobuf.dev/programming-guides/encoding/#varints) size of a message header
	// +10 is the size (variable length integer) of the map
	// TODO : optimize by taking into account the real size and not the maximum size of a message header (and map size)
	// +1 is the end (0 as a variable length integer) of the map
	// +1 is the size of the pruneContainedObjects boolean
	size_t messageSize = 62;

	std::vector<std::string> result;
	Energistics::Etp::v12::Protocol::Store::PutDataObjects msg;
	msg.pruneContainedObjects = false;

	auto it = dataObjects.cbegin();
	int64_t correlationId = 0;
	while (it != dataObjects.cend()) {
		// TODO: optimize by taking into account the real size and not the maximum size of variable length integer
		// Add key size
		size_t dataObjectSize = 10 + it->first.size();
		// Add value size
		dataObjectSize += 10 + it->second.resource.uri.size();
			// ignore alternateUris for now
		dataObjectSize += 10 + it->second.resource.name.size();
		dataObjectSize += 10 + it->second.resource.has_sourceCount() ? 10 : 0;
		dataObjectSize += 10 + it->second.resource.has_targetCount() ? 10 : 0;
		dataObjectSize += 40; //  lastChanged, storeLastWrite, storeCreated, activeStatus
			// ignore customData for now
		dataObjectSize += 10 + it->second.format.size();
		dataObjectSize += 10 + it->second.has_blobId() ? 100 : 0;
		dataObjectSize += 10 + it->second.data.size();

		if (messageSize + dataObjectSize > maxWebSocketMessagePayloadSize) {
			const int64_t sentMessageId = send(msg, correlationId);
			auto successKeys = handlers->getSuccessKeys();
			result.insert(result.end(), successKeys.begin(), successKeys.end());
			handlers->clearSuccessKeys();

			msg.dataObjects.clear();
			messageSize = 62;
			if (correlationId == 0) {
				correlationId = sentMessageId;
			}
		}
		msg.dataObjects.emplace(it->first, it->second);
		messageSize += dataObjectSize;
		++it;
	}
	sendAndBlock(msg, correlationId, 0x02);
	auto successKeys = handlers->getSuccessKeys();
	result.insert(result.end(), successKeys.begin(), successKeys.end());
	handlers->clearSuccessKeys();
	return result;
}

std::vector<std::string> AbstractSession::deleteDataObjects(const std::map<std::string, std::string>& uris)
{
	std::shared_ptr<StoreHandlers> handlers = getStoreProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any store protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::Store::DeleteDataObjects msg;
	msg.uris = uris;
	msg.pruneContainedObjects = false;
	sendAndBlock(msg, 0, 0x02);
	std::vector<std::string> result = handlers->getSuccessKeys();
	handlers->clearSuccessKeys();
	return result;
}

/********************
***** STORE OSDU ****
*********************/

std::vector<std::string> AbstractSession::copyDataObjectsByValue(const std::string& uri, int32_t sourcesDepth, const std::vector<std::string>& dataObjectTypes)
{
	std::shared_ptr<StoreOSDUHandlers> handlers = getStoreOSDUProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any store OSDU protocol handlers.");
	}

	Energistics::Etp::v12::Protocol::StoreOSDU::CopyDataObjectsByValue msg;
	msg.uri = uri;
	msg.sourcesDepth = sourcesDepth;
	msg.dataObjectTypes = dataObjectTypes;
	sendAndBlock(msg, 0, 0x02);
	std::vector<std::string> result = handlers->getCopiedDataobjects();
	handlers->clearCopiedDataobjects();
	return result;
}

/****************
** TRANSACTION **
****************/

std::string AbstractSession::startTransaction(std::vector<std::string> dataspaceUris, bool readOnly)
{
	std::shared_ptr<TransactionHandlers> handlers = getTransactionProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any transaction protocol handlers.");
	}
	if (handlers->isInAnActiveTransaction()) {
		throw std::logic_error("You cannot start a transaction before the current transaction is rolled back or committed. ETP1.2 intentionally supports a single opened transaction on a session.");
	}

	Energistics::Etp::v12::Protocol::Transaction::StartTransaction startTransactionMsg;
	startTransactionMsg.dataspaceUris = dataspaceUris;
	startTransactionMsg.readOnly = readOnly;
	sendAndBlock(startTransactionMsg, 0, 0x02);
	return handlers->isInAnActiveTransaction()
		? ""
		: handlers->getLastTransactionFailure();
}

std::string AbstractSession::rollbackTransaction()
{
	std::shared_ptr<TransactionHandlers> handlers = getTransactionProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any transaction protocol handlers.");
	}
	if (!handlers->isInAnActiveTransaction()) {
		throw std::logic_error("You cannot roll back a transaction which has not been started.");
	}

	Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction rollbackTransactionMsg;
	rollbackTransactionMsg.transactionUuid = handlers->getTransactionUuid();
	sendAndBlock(rollbackTransactionMsg, 0, 0x02);

	return !handlers->isInAnActiveTransaction()
		? ""
		: handlers->getLastTransactionFailure();
}

std::string AbstractSession::commitTransaction()
{
	std::shared_ptr<TransactionHandlers> handlers = getTransactionProtocolHandlers();
	if (handlers == nullptr) {
		throw std::logic_error("You did not register any transaction protocol handlers.");
	}
	if (!handlers->isInAnActiveTransaction()) {
		throw std::logic_error("You cannot commit a transaction which has not been started.");
	}

	Energistics::Etp::v12::Protocol::Transaction::CommitTransaction commitTransactionMsg;
	commitTransactionMsg.transactionUuid = handlers->getTransactionUuid();
	sendAndBlock(commitTransactionMsg, 0, 0x02);

	return !handlers->isInAnActiveTransaction()
		? ""
		: handlers->getLastTransactionFailure();
}
