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
	class FETPAPI_DLL_IMPORT_OR_EXPORT DataArrayHandlers : public ProtocolHandlers
	{
	public:
		DataArrayHandlers(AbstractSession* mySession): ProtocolHandlers(mySession) {}
		virtual ~DataArrayHandlers() = default;

	    void decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader & mh, avro::DecoderPtr d);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
	    virtual void on_GetDataArrays(const Energistics::Etp::v12::Protocol::DataArray::GetDataArrays& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_GetDataArraysResponse(const Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
	    virtual void on_PutDataArrays(const Energistics::Etp::v12::Protocol::DataArray::PutDataArrays& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_PutDataArraysResponse(const Energistics::Etp::v12::Protocol::DataArray::PutDataArraysResponse& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
	    virtual void on_GetDataSubarrays(const Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_GetDataSubarraysResponse(const Energistics::Etp::v12::Protocol::DataArray::GetDataSubarraysResponse& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
	    virtual void on_PutDataSubarrays(const Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_PutDataSubarraysResponse(const Energistics::Etp::v12::Protocol::DataArray::PutDataSubarraysResponse& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_GetDataArrayMetadata(const Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_GetDataArrayMetadataResponse(const Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadataResponse& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_PutUninitializedDataArrays(const Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArrays& msg, int64_t correlationId);

		/**
		* @param msg			The ETP message body which has been received and which is to be processed.
		* @param correlationId	It is the correlation ID to use if a response is needed to this message. It corresponds to the message ID of the received ETP message.
		*/
		virtual void on_PutUninitializedDataArraysResponse(const Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArraysResponse& msg, int64_t correlationId);
	};
}
