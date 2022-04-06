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
#ifndef NDEBUG
	std::cout << "*************************************************" << std::endl;
	std::cout << "Message Header received : " << std::endl;
	std::cout << "protocol : " << receivedMh.protocol << std::endl;
	std::cout << "type : " << receivedMh.messageType << std::endl;
	std::cout << "id : " << receivedMh.messageId << std::endl;
	std::cout << "correlation id : " << receivedMh.correlationId << std::endl;
	std::cout << "flags : " << receivedMh.messageFlags << std::endl;
	std::cout << "*************************************************" << std::endl;
#endif

	return receivedMh;
}

void AbstractSession::on_read(boost::system::error_code ec, std::size_t bytes_transferred)
{
	boost::ignore_unused(bytes_transferred);

	if (ec) {

		if (ec == websocket::error::closed) {
			// This indicates that the web socket (and consequently etp) session was closed
			std::cout << "The other endpoint closed the web socket (and consequently etp) connection." << std::endl;
			webSocketSessionClosed = true;
			flushReceivingBuffer();
		}
		else {
			// This indicates an unexpected error
			std::cerr << "on_read : error code number " << ec.value() << " -> " << ec.message() << std::endl;
		}

		return;
	}

	if (bytes_transferred == 0) {
		return;
	}

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
				auto specificProtocolHandlerIt = specificProtocolHandlers.find(receivedMh.correlationId);
				if (specificProtocolHandlerIt != specificProtocolHandlers.end()) {
					specificProtocolHandlers.erase(specificProtocolHandlerIt);
				}
			}
		}
		else {
			auto specificProtocolHandlerIt = specificProtocolHandlers.find(receivedMh.correlationId);
			if (specificProtocolHandlerIt != specificProtocolHandlers.end()) {
				// Receive a message which has been asked to be processed with a specific protocol handler
				specificProtocolHandlerIt->second->decodeMessageBody(receivedMh, d);
				if ((receivedMh.messageFlags & 0x02) != 0) {
					specificProtocolHandlers.erase(specificProtocolHandlerIt);
				}
			}
			else if (receivedMh.protocol < protocolHandlers.size() && protocolHandlers[receivedMh.protocol] != nullptr) {
				// Receive a message to be processed with a common protocol handler in case for example an unsollicited notification
				protocolHandlers[receivedMh.protocol]->decodeMessageBody(receivedMh, d);
			}
			else {
				flushReceivingBuffer();
				send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(4, "The agent does not support the protocol " + std::to_string(receivedMh.protocol) + " identified in a message header."), receivedMh.messageId, 0x02);
			}
		}
	}
	catch (avro::Exception& e)
	{
		flushReceivingBuffer();
		send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(19, "The agent is unable to de-serialize the body of the message id " + std::to_string(receivedMh.messageId) + " : " + std::string(e.what())), 0, 0x02);
	}

	do_read();
}

void AbstractSession::close()
{
	send(Energistics::Etp::v12::Protocol::Core::CloseSession(), 0, 0x02);
}
