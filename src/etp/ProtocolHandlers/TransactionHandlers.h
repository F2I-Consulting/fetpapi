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
	class FETPAPI_DLL_IMPORT_OR_EXPORT TransactionHandlers : public ProtocolHandlers
	{
	public:
		TransactionHandlers(AbstractSession* mySession): ProtocolHandlers(mySession) {}
		virtual ~TransactionHandlers() = default;

	    void decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader & mh, avro::DecoderPtr d);

	    virtual void on_StartTransaction(const Energistics::Etp::v12::Protocol::Transaction::StartTransaction & msg, int64_t correlationId);
		virtual void on_StartTransactionResponse(const Energistics::Etp::v12::Protocol::Transaction::StartTransactionResponse & msg, int64_t correlationId);
	    virtual void on_CommitTransaction(const Energistics::Etp::v12::Protocol::Transaction::CommitTransaction & msg, int64_t correlationId);
		virtual void on_CommitTransactionResponse(const Energistics::Etp::v12::Protocol::Transaction::CommitTransactionResponse & msg, int64_t correlationId);
	    virtual void on_RollbackTransaction(const Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction & msg, int64_t correlationId);
		virtual void on_RollbackTransactionResponse(const Energistics::Etp::v12::Protocol::Transaction::RollbackTransactionResponse & msg, int64_t correlationId);
	};
}
