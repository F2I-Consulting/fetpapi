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

#include "ProtocolHandlers.h"

namespace ETP_NS
{
	class FETPAPI_DLL_IMPORT_OR_EXPORT CoreHandlers : public ProtocolHandlers
	{
	public:
		CoreHandlers(AbstractSession* mySession):
			ProtocolHandlers(mySession) {}
		virtual ~CoreHandlers() = default;

	    void decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader & mh, avro::DecoderPtr d) override;

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
	    virtual void on_RequestSession(const Energistics::Etp::v12::Protocol::Core::RequestSession& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
	    virtual void on_OpenSession(const Energistics::Etp::v12::Protocol::Core::OpenSession& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body whic hhas been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
	    virtual void on_CloseSession(const Energistics::Etp::v12::Protocol::Core::CloseSession& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID (not message ID) of the received exception message.
		*/
	    virtual void on_ProtocolException(const Energistics::Etp::v12::Protocol::Core::ProtocolException& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID (not message ID) of the received ackknowledged message.
		*/
		virtual void on_Acknowledge(const Energistics::Etp::v12::Protocol::Core::Acknowledge& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_Ping(const Energistics::Etp::v12::Protocol::Core::Ping& msg, int64_t correlationId);
		
		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_Pong(const Energistics::Etp::v12::Protocol::Core::Pong& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_Authorize(const Energistics::Etp::v12::Protocol::Core::Authorize& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_AuthorizeResponse(const Energistics::Etp::v12::Protocol::Core::AuthorizeResponse& msg, int64_t correlationId);
	};
}
