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

#include <avro/Decoder.hh>
#include <avro/Encoder.hh>
#include <avro/Specific.hh>

namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct MessageHeader {
					int32_t protocol = 0;
					int32_t messageType = 0;
					int64_t correlationId = 0;
					int64_t messageId = 0;
					int32_t messageFlags = 0;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::MessageHeader> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::MessageHeader& v) {
			avro::encode(e, v.protocol);
			avro::encode(e, v.messageType);
			avro::encode(e, v.correlationId);
			avro::encode(e, v.messageId);
			avro::encode(e, v.messageFlags);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::MessageHeader& v) {
			avro::decode(e, v.protocol);
			avro::decode(e, v.messageType);
			avro::decode(e, v.correlationId);
			avro::decode(e, v.messageId);
			avro::decode(e, v.messageFlags);
		}
	};
}

namespace ETP_NS
{
	class EtpMessage
	{
	public:

		virtual ~EtpMessage() = default;

		std::shared_ptr<const std::vector<uint8_t>> encodeHeaderAndBody() {
			auto out = avro::memoryOutputStream();
			auto encoder = avro::binaryEncoder();
			encoder->init(*out);

			avro::encode(*encoder, messageHeader);
			encode(*encoder);
			encoder->flush();

			return avro::snapshot(*out);
		}

		virtual void encode(avro::Encoder&) const = 0;
		virtual void decode(avro::Decoder&) = 0;

		std::string to_string() const {
			std::ostringstream oss;
			oss << "*************************************************\n"
				<< "Message Header put in the queue :\n"
				<< "protocol : " << messageHeader.protocol << '\n'
				<< "type : " << messageHeader.messageType << '\n'
				<< "id : " << messageHeader.messageId << '\n'
				<< "correlation id : " << messageHeader.correlationId << '\n'
				<< "flags : " << messageHeader.messageFlags << '\n'
				<< "*************************************************" << '\n'
				<< body_to_string();
			return oss.str();
		}

		Energistics::Etp::v12::Datatypes::MessageHeader messageHeader;

	protected :
		virtual std::string body_to_string() const { return ""; }
	};
}
