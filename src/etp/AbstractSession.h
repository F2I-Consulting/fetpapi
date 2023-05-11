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
#pragma once

#include <iostream>
#include <queue>
#include <unordered_map>
#include <utility>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/nil_generator.hpp>

#include "../nsDefinitions.h"

#include "EtpHelpers.h"
#include "ProtocolHandlers/CoreHandlers.h"
#include "ProtocolHandlers/DiscoveryHandlers.h"
#include "ProtocolHandlers/StoreHandlers.h"
#include "ProtocolHandlers/StoreNotificationHandlers.h"
#include "ProtocolHandlers/DataArrayHandlers.h"
#include "ProtocolHandlers/TransactionHandlers.h"
#include "ProtocolHandlers/DataspaceHandlers.h"

using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>

namespace {
	std::vector<std::string> tokenize(const std::string & str, char delimiter) {
		std::vector<std::string> tokens;
		std::stringstream ss(str);
		std::string token;
		while(getline(ss, token, delimiter)) {
			tokens.push_back(token);
		}

		return tokens;
	}
}

namespace ETP_NS
{
	class AbstractSession : public std::enable_shared_from_this<AbstractSession>
	{
	public:

		virtual ~AbstractSession() = default;

		/**
		* Return the identifier of the session which is an UUID.
		* If the ETP session is not set up, it returns the nil UUID.
		*/
		const boost::uuids::uuid& getIdentifier() {
			if (isEtpSessionClosed()) {
				identifier = boost::uuids::nil_uuid();
			}

			return identifier;
		}

		/**
		* Set the time out in milliseconds used when blocking waiting for message
		*/
		void setTimeOut(double timeOut) {
			_timeOut = timeOut;
		}

		/**
		* Get the time out in milliseconds used when blocking waiting for message
		*/
		double getTimeOut() const {
			return _timeOut;
		}

		/**
		* The list of subscriptions recorded by customers on this session.
		*/
		std::unordered_map<int64_t, Energistics::Etp::v12::Datatypes::Object::SubscriptionInfo> subscriptions;

		/**
		 * Set the Core protocol handlers
		 */
		FETPAPI_DLL_IMPORT_OR_EXPORT void setCoreProtocolHandlers(std::shared_ptr<CoreHandlers> coreHandlers) {
			// Verify that we don't modify handlers which could be in use on the io context thread
			if (!isWebSocketSessionClosed()) {
				throw std::logic_error("You cannot set some protocol handlers once the session is running.");
			}

	    	if (protocolHandlers.empty()) {
	    		protocolHandlers.push_back(coreHandlers);
	    	}
	    	else {
	    		protocolHandlers[static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Core)] = coreHandlers;
	    	}
	    }

	    /**
		 * Set the Discovery protocol handlers
		 */
		FETPAPI_DLL_IMPORT_OR_EXPORT void setDiscoveryProtocolHandlers(std::shared_ptr<DiscoveryHandlers> discoveryHandlers) {
			// Verify that we don't modify handlers which could be in use on the io context thread
			if (!isWebSocketSessionClosed()) {
				throw std::logic_error("You cannot set some protocol handlers once the session is running.");
			}

			while (protocolHandlers.size() < static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Discovery) + 1) {
				protocolHandlers.push_back(nullptr);
			}
			protocolHandlers[static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Discovery)] = discoveryHandlers;
		}

		/**
		 * Set the Store protocol handlers
		 */
		FETPAPI_DLL_IMPORT_OR_EXPORT void setStoreProtocolHandlers(std::shared_ptr<StoreHandlers> storeHandlers) {
			// Verify that we don't modify handlers which could be in use on the io context thread
			if (!isWebSocketSessionClosed()) {
				throw std::logic_error("You cannot set some protocol handlers once the session is running.");
			}

			while (protocolHandlers.size() < static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Store) + 1) {
				protocolHandlers.push_back(nullptr);
			}
			protocolHandlers[static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Store)] = storeHandlers;
		}

		/**
		* Set the StoreNotification protocol handlers
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void setStoreNotificationProtocolHandlers(std::shared_ptr<ETP_NS::StoreNotificationHandlers> storeNotificationHandlers) {
			// Verify that we don't modify handlers which could be in use on the io context thread
			if (!isWebSocketSessionClosed()) {
				throw std::logic_error("You cannot set some protocol handlers once the session is running.");
			}

			while (protocolHandlers.size() < static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification) + 1) {
				protocolHandlers.push_back(nullptr);
			}
			protocolHandlers[static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification)] = storeNotificationHandlers;
		}

		/**
		 * Set the Data Array protocol handlers
		 */
		FETPAPI_DLL_IMPORT_OR_EXPORT void setDataArrayProtocolHandlers(std::shared_ptr<DataArrayHandlers> dataArrayHandlers) {
			// Verify that we don't modify handlers which could be in use on the io context thread
			if (!isWebSocketSessionClosed()) {
				throw std::logic_error("You cannot set some protocol handlers once the session is running.");
			}

			while (protocolHandlers.size() < static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::DataArray) + 1) {
				protocolHandlers.push_back(nullptr);
			}
			protocolHandlers[static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::DataArray)] = dataArrayHandlers;
		}

		/**
		 * Set the Transaction protocol handlers
		 */
		FETPAPI_DLL_IMPORT_OR_EXPORT void setTransactionProtocolHandlers(std::shared_ptr<TransactionHandlers> transactionHandlers) {
			// Verify that we don't modify handlers which could be in use on the io context thread
			if (!isWebSocketSessionClosed()) {
				throw std::logic_error("You cannot set some protocol handlers once the session is running.");
			}

			while (protocolHandlers.size() < static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Transaction) + 1) {
				protocolHandlers.push_back(nullptr);
			}
			protocolHandlers[static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Transaction)] = transactionHandlers;
		}

		/**
		 * Set the Dataspace protocol handlers
		 */
		FETPAPI_DLL_IMPORT_OR_EXPORT void setDataspaceProtocolHandlers(std::shared_ptr<DataspaceHandlers> dataspaceHandlers) {
			// Verify that we don't modify handlers which could be in use on the io context thread
			if (!isWebSocketSessionClosed()) {
				throw std::logic_error("You cannot set some protocol handlers once the session is running.");
			}

			while (protocolHandlers.size() < static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace) + 1) {
				protocolHandlers.push_back(nullptr);
			}
			protocolHandlers[static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace)] = dataspaceHandlers;
		}

		/**
		 * Create a default ETP message header from the ETP message body.
		 * Encode this created default ETP message header + the ETP message body and put the result in the sending queue.
		 *
		 * @param mb The ETP message body to send
		 * @param correlationId The ID of the message which this message is answering to.
		 * @param messageFlags The message flags to be sent within the header
		 * @return The ID of the message that has been put in the sending queue.
		 */
		template<typename T> int64_t send(const T & mb, int64_t correlationId = 0, int32_t messageFlags = 0)
		{
			if (protocolHandlers.size() > mb.protocolId) {
				return sendWithSpecificHandler(mb, protocolHandlers[mb.protocolId], correlationId, messageFlags);
			}
			else {
				throw std::logic_error("The agent has no registered handler at all for the protocol " + std::to_string(mb.protocolId));
			}
		}

		/**
		* Send a message to the server and block the thread until the answer of the server has been processed by the handlers
		* Please look at setTimeOut if you want to set the default timeout value which is 10 000 ms.
		*
		* @param mb				The ETP message body to send
		* @param correlationId	The ID of the message which this message is answering to.
		* @param messageFlags	The message flags to be sent within the header
		* @return The ID of the message that has been put in the sending queue.
		*/
		template<typename T> void sendAndBlock(const T & mb, int64_t correlationId = 0, int32_t messageFlags = 0)
		{
			int64_t msgId = send(mb, correlationId, messageFlags);

			auto t_start = std::chrono::high_resolution_clock::now();
			while (isMessageStillProcessing(msgId)) {
				if (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() > _timeOut) {
					throw std::runtime_error("Time out waiting for a response of message id " + std::to_string(msgId));
				}
			}
		}

		/**
		* Send a message and register a specific handler for the response.
		*
		* @param mb The ETP message body to send
		* @param specificHandler The handlers which are going to be called for the response to this sent message
		* @param correlationId The ID of the message which this message is answering to.
		* @param messageFlags The message flags to be sent within the header
		* @return The ID of the message that has been put in the sending queue.
		*/
		template<typename T> int64_t sendWithSpecificHandler(const T & mb, std::shared_ptr<ETP_NS::ProtocolHandlers> specificHandler, int64_t correlationId = 0, int32_t messageFlags = 0)
		{
			// Encode the message into AVRO format
			auto queueItem = encode(mb, correlationId, messageFlags);

			const std::lock_guard<std::mutex> sendingQueueLock(sendingQueueMutex);
			// Set the handlers which are going to be called for the response to this sent message
			std::get<2>(queueItem) = specificHandler;
			// Push the message into the queue
			sendingQueue.push(queueItem);
			fesapi_log("*************************************************");
			fesapi_log("Message Header put in the queue : ");
			fesapi_log("protocol :", std::to_string(mb.protocolId));
			fesapi_log("type :" , std::to_string(mb.messageTypeId));
			fesapi_log("id :" , std::to_string(std::get<0>(queueItem)));
			fesapi_log("correlation id :" , std::to_string(correlationId));
			fesapi_log("flags :" , std::to_string(messageFlags));
			fesapi_log("Whole message size :" , std::to_string(std::get<1>(queueItem).size()) , "bytes.");
			fesapi_log("*************************************************");

			// Send the message directly if the sending queue was empty.
			if (sendingQueue.size() == 1) {
				do_write();
			}

			return std::get<0>(queueItem);
		}

		/**
		 * Close the web socket session (without sending any ETP message)
		 */
		virtual void do_close() = 0;

		/**
		 * Read what is on the websocket
		 */
		virtual void do_read() = 0;

		/**
		 * This method is called each time a message is received on the web socket
		 */
		FETPAPI_DLL_IMPORT_OR_EXPORT void on_read(boost::system::error_code ec, std::size_t bytes_transferred);

		void on_write(boost::system::error_code ec, std::size_t) {
			if(ec) {
				std::cerr << "on_write : " << ec.message() << std::endl;
			}

			// Remove the sent message from the queue
			const std::lock_guard<std::mutex> sendingQueueLock(sendingQueueMutex);
			sendingQueue.pop();

			do_write();
		}

		void on_close(boost::system::error_code ec) {
			if(ec) {
				std::cerr << "on_close : " << ec.message() << std::endl;
			}
			if (!etpSessionClosed) {
				std::cerr << "Websocket session is going to be closed BUT ETP SESSION HAS NOT BEEN CLOSED YET!!!" << std::endl;
			}

			// If we get here then the connection is closed gracefully
			webSocketSessionClosed = true;
		}

		/**
		* Check if the websocket session (starting after the HTTP handshake/upgrade) is not opened yet or has been closed.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT bool isWebSocketSessionClosed() const { return webSocketSessionClosed;  }

		/**
		* Check wether a particular ETP message has been responded or not by the other agent.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT bool isMessageStillProcessing(int64_t msgId) {
			const std::lock_guard<std::mutex> sendingQueueLock(sendingQueueMutex);
			const std::lock_guard<std::mutex> specificProtocolHandlersLock(specificProtocolHandlersMutex);
			return (!sendingQueue.empty() && std::get<0>(sendingQueue.front()) <= msgId) || specificProtocolHandlers.count(msgId) > 0;
		}
		//FETPAPI_DLL_IMPORT_OR_EXPORT bool isMessageStillProcessing(int64_t msgId) const { return specificProtocolHandlers.count(msgId) > 0; }

		virtual void setMaxWebSocketMessagePayloadSize(int64_t value) = 0;
		int64_t getMaxWebSocketMessagePayloadSize() const { return maxWebSocketMessagePayloadSize; }

		/****************
		***** CORE ******
		****************/

		/**
		* Send a message for closing to the server after having sent all previous messages.
		* The session would really close only after all messages have been sent and responded.
		* This method does not block.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void close() {
			isCloseRequested = true;
			sendingQueueMutex.lock();
			specificProtocolHandlersMutex.lock();
			if (specificProtocolHandlers.empty() && sendingQueue.empty()) {
				etpSessionClosed = true;
				sendingQueueMutex.unlock();
				specificProtocolHandlersMutex.unlock();
				send(Energistics::Etp::v12::Protocol::Core::CloseSession(), 0, 0x02);
			}
			else {
				sendingQueueMutex.unlock();
				specificProtocolHandlersMutex.unlock();
			}
		}

		/**
		* Send a message for closing to the server after having sent all previous messages.
		* The session would really close only after all messages have been sent and responded.
		* This method does block.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT void closeAndBlock() {
			close();
			auto t_start = std::chrono::high_resolution_clock::now();
			while (!isEtpSessionClosed()) {
				if (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() > _timeOut) {
					throw std::runtime_error("Time out waiting for closing");
				}
			}
		}

		/**
		* Check if the ETP session (starting after the Core.OpenSession or Core.RequestSession message) is not opened yet or has been closed.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT bool isEtpSessionClosed() const { return webSocketSessionClosed || etpSessionClosed; }

		void setEtpSessionClosed(bool etpSessionClosed_) { etpSessionClosed = etpSessionClosed_; }

		/****************
		*** DATASPACE ***
		****************/

		/**
		* A customer sends to a store to discover all dataspaces available on the store.
		* This function should be used with caution if Dataspace Handlers have been overidden.
		* It actually sends a message and block the current thread untill a response has been received from the store.
		*
		* @param storeLastWriteFilter	An optional filter to limit the dataspaces returned by date/time last saved to the store.
		*								The store returns a list of dataspaces whose last changed date/time is greater than the specified date/time.
		*								It must be a UTC dateTime value, serialized as a positive int 64bits, using the Avro logical type timestamp-micros (microseconds from the Unix Epoch, 1 January 1970 00:00:00.000000 UTC).
		* @param return	The available dataspaces the store could return.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::vector<Energistics::Etp::v12::Datatypes::Object::Dataspace> getDataspaces(int64_t storeLastWriteFilter = -1);

		/**
		* A customer sends to a store to create one or more dataspaces.
		* This function should be used with caution if Dataspace Handlers have been overidden.
		* It actually sends a message and block the current thread untill a response has been received from the store.
		*
		* @param dataspaces  ETP general map : One each for each dataspace the customer wants to add or update.
		* @param return	The map keys corresponding to the dataspaces which have been put successfully into the store.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::vector<std::string> putDataspaces(const std::map<std::string, Energistics::Etp::v12::Datatypes::Object::Dataspace>& dataspaces);

		/**
		* A customer sends to a store to delete one or more dataspaces.
		* This function should be used with caution if Dataspace Handlers have been overidden.
		* It actually sends a message and block the current thread untill a response has been received from the store.
		*
		* @param dataspaceUris  ETP general map where the values must be the URIs for the dataspaces the customer wants to delete.
		* @param return	The map keys corresponding to the dataspaces which have been deleted successfully.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::vector<std::string> deleteDataspaces(const std::map<std::string, std::string>& dataspaceUris);

		/****************
		*** DISCOVERY ***
		****************/

		/**
		* A Customer sends this message to a store to discover data objects in the store.
		* This function should be used with caution if Discovery Handlers have been overidden.
		* It actually sends a message and block the current thread untill a response has been received from the store.
		*
		* @param context				Includes the URI of the dataspace or data object to begin the discovery, what specific types of data objects are of interest,
		*								and how many "levels" of relationships in the model to discover, among others. 
		* @param scope					Scope is specified in reference to the URI (which is entered in the context field).
		*								It indicates which direction in the graph that the operation should proceed (targets or sources) and whether or not to include the starting point (self).
		* @param storeLastWriteFilter	An optional filter to filter the discovery on a date when the data object was last written in a particular store.
		*								The store returns resources whose storeLastWrite date/time is GREATER than the date/time specified in this filter field.
		*								It must be a UTC dateTime value, serialized as a positive int 64bits, using the Avro logical type timestamp-micros (microseconds from the Unix Epoch, 1 January 1970 00:00:00.000000 UTC).
		* @param countObjects			If true, the store provides counts of sources and targets for each resource identified by Discovery.
		* @param return					The resources corresponding to this query.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::vector<Energistics::Etp::v12::Datatypes::Object::Resource> getResources(
			const Energistics::Etp::v12::Datatypes::Object::ContextInfo& context,
			const Energistics::Etp::v12::Datatypes::Object::ContextScopeKind& scope,
			int64_t storeLastWriteFilter = -1,
			bool countObjects = false);

		/**
		* A customer sends to a store to discover data objects that have been deleted (which are sometimes called "tombstones").
		* This function should be used with caution if Discovery Handlers have been overidden.
		* It actually sends a message and block the current thread untill a response has been received from the store.
		*
		* @param dataspaceUri			The URI of the dataspace where the objects were deleted.
		* @param deleteTimeFilter		An optional filter to filter the discovery on a date when the data object was deleted in a particular store.
		*								The store returns resources for objects whose delete times are greater than this value.
		*								It must be a UTC dateTime value, serialized as a positive int 64bits, using the Avro logical type timestamp-micros (microseconds from the Unix Epoch, 1 January 1970 00:00:00.000000 UTC).
		* @param dataObjectTypes		Filter for the types of data objects you want. EXAMPLES: "witsml20.Well", "resqml20.obj_TectonicBoundaryFeature", "resqml20.*" 
		* @param return					The deleted resources corresponding to this query.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::vector<Energistics::Etp::v12::Datatypes::Object::DeletedResource> getDeletedResources(
			const std::string& dataspaceUri,
			int64_t deleteTimeFilter = -1,
			const std::vector<std::string>& dataObjectTypes = {});

		/****************
		***** STORE *****
		****************/

		/**
		* A customer sends to a store to get one or more data objects, each identified by a URI.
		* This function should be used with caution if Store Handlers have been overidden.
		* It actually sends a message and block the current thread untill a response has been received from the store.
		*
		* @param uris	ETP general map where the values MUST be the URIs of a data object to be retrieved.
		* @param return	The received dataobjects in a map where the key makes the link between the asked uris and the received dataobjects.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> getDataObjects(const std::map<std::string, std::string>& uris);

		/**
		* A customer sends to a store to add or update one or more data objects.
		* This function should be used with caution if Store Handlers have been overidden.
		* It actually sends a message and block the current thread untill a response has been received from the store.
		*
		* @param uris	ETP general map where the values MUST be the data for each data object in the request, including each one's URI.
		* @param return	The map keys corresponding to the dataObjects which have been put successfully.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::vector<std::string> putDataObjects(const std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject>& dataObjects);

		/**
		* A customer sends to a store to delete one or more data objects from the store.  
		* This function should be used with caution if Store Handlers have been overidden.
		* It actually sends a message and block the current thread untill a response has been received from the store.
		*
		* @param uris	ETP general map where the values MUST be the URIs of a data object to be deleted.
		* @param return	The map keys corresponding to the dataObjects which have been deleted successfully.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::vector<std::string> deleteDataObjects(const std::map<std::string, std::string>& uris);

		/****************
		** TRANSACTION **
		****************/

		/**
		* A customer sends to a store to begin a transaction.
		* This function should be used with caution if Transaction Handlers have been overidden.
		* It actually sends a message and block the current thread untill a response has been received from the store.
		*
		* @param dataspaceUris  Indicates the dataspaces involved in the transaction. An empty STRING means the default dataspace. An empty LIST means all dataspaces.
		* @param readOnly		Indicates that the request in the transaction is read-only (i.e., "get" messages). 
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::string startTransaction(std::vector<std::string> dataspaceUris = {}, bool readOnly = false);

		/**
		* A customer sends to a store to commit and end a transaction. This message implies that the customer 
		* has received from or sent to the store all the data required for some purpose. The customer asserts that 
		* the data sent in the scope of this transaction is a consistent unit of work.
		* It actually sends a message and block the current thread untill a response has been received from the store.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::string rollbackTransaction();

		/*
		* A customer sends to a store to cancel a transaction. The store MUST disregard any requests or data sent 
		* with that transaction. The current transaction (the one being canceled) MUST NOT change the state of 
		* the store.
		* It actually sends a message and block the current thread untill a response has been received from the store.
		*/
		FETPAPI_DLL_IMPORT_OR_EXPORT std::string commitTransaction();

		/***************************/
		// LOGGING
		/***************************/

		/**
		* Indicates if the session must be verbose or not
		*/
		void setVerbose(bool verbose) {
			_verbose = verbose;
		}

		//terminating log
		void fesapi_log() { std::cout << std::endl; }

		template<typename First, typename ...Rest>
		void fesapi_log(First && first, Rest && ...rest)
		{
			if (_verbose) {
				std::cout << std::forward<First>(first) << " ";
				fesapi_log(std::forward<Rest>(rest)...);
			}
		}

	protected:
		boost::beast::flat_buffer receivedBuffer;
		/// The default handlers for each subprotocol. Default handlers are at the index of the corresponding subprotocol id.
		std::vector<std::shared_ptr<ETP_NS::ProtocolHandlers>> protocolHandlers;
		/// A map indicating which handlers must be used for responding to which message id.
		std::unordered_map<int64_t, std::shared_ptr<ETP_NS::ProtocolHandlers>> specificProtocolHandlers;
		std::mutex specificProtocolHandlersMutex;
		/// The maximum size in bytes allowed for a complete WebSocket message payload, which is composed of one or more WebSocket frames.
		/// The limit to use during a session is the smaller of the client's and the server's value for MaxWebSocketMessagePayloadSize,
		/// which should be determined by the limits imposed by the WebSocket library used by each endpoint. 
		/// See https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/using_websocket/messages.html
		/// and https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/ref/boost__beast__websocket__stream/read_message_max/overload1.html
		int64_t maxWebSocketMessagePayloadSize = 16000000;
		/// Indicates if the websocket session is opened or not. It becomes false after the websocket handshake
		std::atomic<bool> webSocketSessionClosed = true;
		/// Indicates if the ETP1.2 session is opened or not. It becomes false after the requestSession and openSession message
		std::atomic<bool> etpSessionClosed = true;
		/// Timeout in milliseconds used when blocking waiting for message
		std::atomic<double> _timeOut = 10000;
		/// Indicates if the session must be verbose or not
		std::atomic<bool> _verbose = false;
		/// The queue of messages to be sent where the tuple respectively define message id, message and protocol handlers for responding to this message.
		std::queue< std::tuple<int64_t, std::vector<uint8_t>, std::shared_ptr<ETP_NS::ProtocolHandlers>> > sendingQueue;
		std::mutex sendingQueueMutex;
		/// The next available message id.
		std::atomic<int64_t> messageId;
		/// The identifier of the session
		boost::uuids::uuid identifier;
		/// Indicates that the endpoint request to close the websocket session 
		bool isCloseRequested = false;

		AbstractSession() = default;

		virtual boost::asio::io_context& getIoContext() = 0;

		void flushReceivingBuffer() {
			receivedBuffer.consume(receivedBuffer.size());
		}

		/**
		 * Write the current buffer on the web socket
		 */
		virtual void do_write() = 0;

		/**
		 * Reads the message header currently stored in the decoder.
		 * @param decoder	Must be initialized with stream containing a coded message header.
		 */
		Energistics::Etp::v12::Datatypes::MessageHeader decodeMessageHeader(avro::DecoderPtr decoder);

		/**
		* Return the current message id and increment it for next call.
		*/
		int64_t incrementMessageId() {
			const int64_t result = messageId;
			messageId += 2;
			return  result;
		}

		template<typename T> std::tuple<int64_t, std::vector<uint8_t>, std::shared_ptr<ETP_NS::ProtocolHandlers>> encode(const T & mb, int64_t correlationId = 0, int32_t messageFlags = 0)
		{
			// Build message header
			Energistics::Etp::v12::Datatypes::MessageHeader mh;
			mh.protocol = mb.protocolId;
			mh.messageType = mb.messageTypeId;
			mh.correlationId = correlationId;
			mh.messageId = incrementMessageId();
			mh.messageFlags = messageFlags;

			avro::OutputStreamPtr out = avro::memoryOutputStream();
			avro::EncoderPtr e = avro::binaryEncoder();
			e->init(*out);
			avro::encode(*e, mh);
			avro::encode(*e, mb);
			e->flush();
			const int64_t byteCount = e->byteCount();

			if (byteCount < maxWebSocketMessagePayloadSize) {
				return std::make_tuple(mh.messageId, *avro::snapshot(*out).get(), nullptr);
			}
			else {
				messageId -= 2;
				if (correlationId != 0) {
					return encode(EtpHelpers::buildSingleMessageProtocolException(17, "I try to send you a too big message response of protocol "
						+ std::to_string(mb.protocolId) + " and type id " + std::to_string(mb.messageTypeId) + " and size " + std::to_string(byteCount)
						+ " bytes according to our negotiated size capability which is " + std::to_string(maxWebSocketMessagePayloadSize) + " bytes."), correlationId, 0x02);
				}
				else {
					return std::make_tuple(-1, std::vector<uint8_t>{}, nullptr); // You cannot send a message which is too big. Please use message part or chunk or whatever else.
				}
			}
		}

		std::shared_ptr<ETP_NS::CoreHandlers> getCoreProtocolHandlers() {
			const size_t protocolId = static_cast<size_t>(Energistics::Etp::v12::Datatypes::Protocol::Core);
			return protocolHandlers.size() > protocolId
				? std::dynamic_pointer_cast<CoreHandlers>(protocolHandlers[protocolId])
				: nullptr;
		}

		std::shared_ptr<ETP_NS::DiscoveryHandlers> getDiscoveryProtocolHandlers() {
			const size_t protocolId = static_cast<size_t>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
			return protocolHandlers.size() > protocolId
				? std::dynamic_pointer_cast<DiscoveryHandlers>(protocolHandlers[protocolId])
				: nullptr;
		}

		std::shared_ptr<ETP_NS::StoreHandlers> getStoreProtocolHandlers() {
			const size_t protocolId = static_cast<size_t>(Energistics::Etp::v12::Datatypes::Protocol::Store);
			return protocolHandlers.size() > protocolId
				? std::dynamic_pointer_cast<StoreHandlers>(protocolHandlers[protocolId])
				: nullptr;
		}

		std::shared_ptr<ETP_NS::StoreNotificationHandlers> getStoreNotificationProtocolHandlers() {
			const size_t protocolId = static_cast<size_t>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
			return protocolHandlers.size() > protocolId
				? std::dynamic_pointer_cast<StoreNotificationHandlers>(protocolHandlers[protocolId])
				: nullptr;
		}

		std::shared_ptr<ETP_NS::DataArrayHandlers> getDataArrayProtocolHandlers() {
			const size_t protocolId = static_cast<size_t>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
			return protocolHandlers.size() > protocolId
				? std::dynamic_pointer_cast<DataArrayHandlers>(protocolHandlers[protocolId])
				: nullptr;
		}

		std::shared_ptr<ETP_NS::TransactionHandlers> getTransactionProtocolHandlers() {
			const size_t protocolId = static_cast<size_t>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
			return protocolHandlers.size() > protocolId
				? std::dynamic_pointer_cast<TransactionHandlers>(protocolHandlers[protocolId])
				: nullptr;
		}

		std::shared_ptr<ETP_NS::DataspaceHandlers> getDataspaceProtocolHandlers() {
			const size_t protocolId = static_cast<size_t>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
			return protocolHandlers.size() > protocolId
				? std::dynamic_pointer_cast<DataspaceHandlers>(protocolHandlers[protocolId])
				: nullptr;
		}
	};
}
