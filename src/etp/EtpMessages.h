#ifndef ETP_MESSAGES__
#define ETP_MESSAGES__

#include <chrono>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <array>
#include <boost/optional.hpp>
#include <boost/any.hpp>
#include <avro/Specific.hh>
#include <avro/Encoder.hh>
#include <avro/Decoder.hh>

namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class Protocol : uint16_t {
					Core = 0,
					ChannelStreaming = 1,
					ChannelDataFrame = 2,
					Discovery = 3,
					Store = 4,
					StoreNotification = 5,
					GrowingObject = 6,
					GrowingObjectNotification = 7,
					DEPRECATED_8 = 8,
					DataArray = 9,
					RESERVED_10 = 10,
					RESERVED_11 = 11,
					RESERVED_12 = 12,
					DiscoveryQuery = 13,
					StoreQuery = 14,
					RESERVED_15 = 15,
					GrowingObjectQuery = 16,
					RESERVED_17 = 17,
					Transaction = 18,
					RESERVED_19 = 19,
					RESERVED_20 = 20,
					ChannelSubscribe = 21,
					ChannelDataLoad = 22,
					RESERVED_23 = 23,
					Dataspace = 24,
					SupportedTypes = 25
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Protocol> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Protocol& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Protocol& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::Protocol>(e.decodeEnum());
		}
	};
}

namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct Acknowledge {
						static constexpr int messageTypeId=1001;
						int protocolId;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Core::Acknowledge> {
		static void encode(Encoder&, const Energistics::Etp::v12::Protocol::Core::Acknowledge&) {
		}
		static void decode(Decoder&, Energistics::Etp::v12::Protocol::Core::Acknowledge&) {
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct Authorize {
						std::string authorization;
						std::map<std::string, std::string> supplementalAuthorization;
						static constexpr int messageTypeId=6;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Core);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Core::Authorize> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Core::Authorize& v) {
			avro::encode(e, v.authorization);
			avro::encode(e, v.supplementalAuthorization);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Core::Authorize& v) {
			avro::decode(e, v.authorization);
			avro::decode(e, v.supplementalAuthorization);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct AuthorizeResponse {
						bool success=false;
						std::vector<std::string> challenges;
						static constexpr int messageTypeId=7;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Core);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Core::AuthorizeResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Core::AuthorizeResponse& v) {
			avro::encode(e, v.success);
			avro::encode(e, v.challenges);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Core::AuthorizeResponse& v) {
			avro::decode(e, v.success);
			avro::decode(e, v.challenges);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct CloseSession {
						std::string reason;
						static constexpr int messageTypeId=5;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Core);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Core::CloseSession> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Core::CloseSession& v) {
			avro::encode(e, v.reason);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Core::CloseSession& v) {
			avro::decode(e, v.reason);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct Pong {
						int64_t currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						static constexpr int messageTypeId=9;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Core);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Core::Pong> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Core::Pong& v) {
			avro::encode(e, v.currentDateTime);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Core::Pong& v) {
			avro::decode(e, v.currentDateTime);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct Ping {
						int64_t currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						static constexpr int messageTypeId=8;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Core);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Core::Ping> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Core::Ping& v) {
			avro::encode(e, v.currentDateTime);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Core::Ping& v) {
			avro::decode(e, v.currentDateTime);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutDataArraysResponse {
						std::map<std::string, std::string> success;
						static constexpr int messageTypeId=10;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::PutDataArraysResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::PutDataArraysResponse& v) {
			avro::encode(e, v.success);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::PutDataArraysResponse& v) {
			avro::decode(e, v.success);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutDataSubarraysResponse {
						std::map<std::string, std::string> success;
						static constexpr int messageTypeId=11;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::PutDataSubarraysResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::PutDataSubarraysResponse& v) {
			avro::encode(e, v.success);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::PutDataSubarraysResponse& v) {
			avro::decode(e, v.success);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutUninitializedDataArraysResponse {
						std::map<std::string, std::string> success;
						static constexpr int messageTypeId=12;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArraysResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArraysResponse& v) {
			avro::encode(e, v.success);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArraysResponse& v) {
			avro::decode(e, v.success);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct DeleteDataspaces {
						std::map<std::string, std::string> uris;
						static constexpr int messageTypeId=4;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces& v) {
			avro::encode(e, v.uris);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspaces& v) {
			avro::decode(e, v.uris);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct DeleteDataspacesResponse {
						std::map<std::string, std::string> success;
						static constexpr int messageTypeId=5;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspacesResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspacesResponse& v) {
			avro::encode(e, v.success);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Dataspace::DeleteDataspacesResponse& v) {
			avro::decode(e, v.success);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct GetDataspaces {
						boost::optional<int64_t> storeLastWriteFilter;
						bool has_storeLastWriteFilter() const { return storeLastWriteFilter.is_initialized(); }
						int64_t get_storeLastWriteFilter() const { return storeLastWriteFilter.get(); }
						static constexpr int messageTypeId=1;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces& v) {
			avro::encode(e, v.storeLastWriteFilter);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Dataspace::GetDataspaces& v) {
			avro::decode(e, v.storeLastWriteFilter);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct PutDataspacesResponse {
						std::map<std::string, std::string> success;
						static constexpr int messageTypeId=6;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Dataspace::PutDataspacesResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Dataspace::PutDataspacesResponse& v) {
			avro::encode(e, v.success);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Dataspace::PutDataspacesResponse& v) {
			avro::decode(e, v.success);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Discovery {
					struct GetDeletedResources {
						std::string dataspaceUri;
						boost::optional<int64_t> deleteTimeFilter;
						bool has_deleteTimeFilter() const { return deleteTimeFilter.is_initialized(); }
						int64_t get_deleteTimeFilter() const { return deleteTimeFilter.get(); }
						std::vector<std::string> dataObjectTypes;
						static constexpr int messageTypeId=5;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Discovery::GetDeletedResources> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Discovery::GetDeletedResources& v) {
			avro::encode(e, v.dataspaceUri);
			avro::encode(e, v.deleteTimeFilter);
			avro::encode(e, v.dataObjectTypes);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Discovery::GetDeletedResources& v) {
			avro::decode(e, v.dataspaceUri);
			avro::decode(e, v.deleteTimeFilter);
			avro::decode(e, v.dataObjectTypes);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct SubscribeNotificationsResponse {
						std::map<std::string, std::string> success;
						static constexpr int messageTypeId=10;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotificationsResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotificationsResponse& v) {
			avro::encode(e, v.success);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotificationsResponse& v) {
			avro::decode(e, v.success);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct DeleteDataObjects {
						std::map<std::string, std::string> uris;
						bool pruneContainedObjects=false;
						static constexpr int messageTypeId=3;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Store);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Store::DeleteDataObjects> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Store::DeleteDataObjects& v) {
			avro::encode(e, v.uris);
			avro::encode(e, v.pruneContainedObjects);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Store::DeleteDataObjects& v) {
			avro::decode(e, v.uris);
			avro::decode(e, v.pruneContainedObjects);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct GetDataObjects {
						std::map<std::string, std::string> uris;
						std::string format = "xml";
						static constexpr int messageTypeId=1;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Store);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Store::GetDataObjects> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Store::GetDataObjects& v) {
			avro::encode(e, v.uris);
			avro::encode(e, v.format);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Store::GetDataObjects& v) {
			avro::decode(e, v.uris);
			avro::decode(e, v.format);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct StartTransaction {
						bool readOnly=false;
						std::string message;
						std::vector<std::string> dataspaceUris;
						static constexpr int messageTypeId=1;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Transaction::StartTransaction> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Transaction::StartTransaction& v) {
			avro::encode(e, v.readOnly);
			avro::encode(e, v.message);
			avro::encode(e, v.dataspaceUris);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Transaction::StartTransaction& v) {
			avro::decode(e, v.readOnly);
			avro::decode(e, v.message);
			avro::decode(e, v.dataspaceUris);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class AnyArrayType {
					arrayOfBoolean=0,
					arrayOfInt=1,
					arrayOfLong=2,
					arrayOfFloat=3,
					arrayOfDouble=4,
					arrayOfString=5,
					bytes=6
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::AnyArrayType> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::AnyArrayType& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::AnyArrayType>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::AnyArrayType& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::AnyArrayType>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class AnyLogicalArrayType {
					arrayOfBoolean=0,
					arrayOfInt8=1,
					arrayOfUInt8=2,
					arrayOfInt16LE=3,
					arrayOfInt32LE=4,
					arrayOfInt64LE=5,
					arrayOfUInt16LE=6,
					arrayOfUInt32LE=7,
					arrayOfUInt64LE=8,
					arrayOfFloat32LE=9,
					arrayOfDouble64LE=10,
					arrayOfInt16BE=11,
					arrayOfInt32BE=12,
					arrayOfInt64BE=13,
					arrayOfUInt16BE=14,
					arrayOfUInt32BE=15,
					arrayOfUInt64BE=16,
					arrayOfFloat32BE=17,
					arrayOfDouble64BE=18,
					arrayOfString=19,
					arrayOfCustom=20
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::AnyLogicalArrayType> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::AnyLogicalArrayType& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::AnyLogicalArrayType>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::AnyLogicalArrayType& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::AnyLogicalArrayType>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfBoolean {
					std::vector<bool> values;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ArrayOfBoolean> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ArrayOfBoolean& v) {
			avro::encode(e, v.values);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ArrayOfBoolean& v) {
			avro::decode(e, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfBytes {
					std::vector<std::string> values;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ArrayOfBytes> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ArrayOfBytes& v) {
			avro::encode(e, v.values);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ArrayOfBytes& v) {
			avro::decode(e, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfDouble {
					std::vector<double> values;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ArrayOfDouble> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ArrayOfDouble& v) {
			avro::encode(e, v.values);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ArrayOfDouble& v) {
			avro::decode(e, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfFloat {
					std::vector<float> values;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ArrayOfFloat> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ArrayOfFloat& v) {
			avro::encode(e, v.values);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ArrayOfFloat& v) {
			avro::decode(e, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfInt {
					std::vector<int32_t> values;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ArrayOfInt> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ArrayOfInt& v) {
			avro::encode(e, v.values);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ArrayOfInt& v) {
			avro::decode(e, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfLong {
					std::vector<int64_t> values;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ArrayOfLong> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ArrayOfLong& v) {
			avro::encode(e, v.values);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ArrayOfLong& v) {
			avro::decode(e, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfNullableBoolean {
					std::vector<boost::optional<bool>> values;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean& v) {
			avro::encode(e, v.values);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean& v) {
			avro::decode(e, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfNullableInt {
					std::vector<boost::optional<int32_t>> values;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ArrayOfNullableInt> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ArrayOfNullableInt& v) {
			avro::encode(e, v.values);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ArrayOfNullableInt& v) {
			avro::decode(e, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfNullableLong {
					std::vector<boost::optional<int64_t>> values;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ArrayOfNullableLong> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ArrayOfNullableLong& v) {
			avro::encode(e, v.values);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ArrayOfNullableLong& v) {
			avro::decode(e, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfString {
					std::vector<std::string> values;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ArrayOfString> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ArrayOfString& v) {
			avro::encode(e, v.values);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ArrayOfString& v) {
			avro::decode(e, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct DeleteDataObjectsResponse {
						std::map<std::string, Energistics::Etp::v12::Datatypes::ArrayOfString> deletedUris;
						static constexpr int messageTypeId=10;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Store);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Store::DeleteDataObjectsResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Store::DeleteDataObjectsResponse& v) {
			avro::encode(e, v.deletedUris);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Store::DeleteDataObjectsResponse& v) {
			avro::decode(e, v.deletedUris);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct AnyArrayitem_t {
				private:
					size_t idx_=0;
					boost::any value_;

				public:
					size_t idx() const { return idx_; }
					Energistics::Etp::v12::Datatypes::ArrayOfBoolean const & get_ArrayOfBoolean() const {
						if (idx_ != 0) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfBoolean const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfBoolean& get_ArrayOfBoolean()  {
						if (idx_ != 0) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfBoolean& >(value_);
					}
					void set_ArrayOfBoolean(const Energistics::Etp::v12::Datatypes::ArrayOfBoolean& v) {
						idx_ = 0;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfInt const & get_ArrayOfInt() const {
						if (idx_ != 1) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfInt const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfInt& get_ArrayOfInt()  {
						if (idx_ != 1) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfInt& >(value_);
					}
					void set_ArrayOfInt(const Energistics::Etp::v12::Datatypes::ArrayOfInt& v) {
						idx_ = 1;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfLong const & get_ArrayOfLong() const {
						if (idx_ != 2) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfLong const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfLong& get_ArrayOfLong()  {
						if (idx_ != 2) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfLong& >(value_);
					}
					void set_ArrayOfLong(const Energistics::Etp::v12::Datatypes::ArrayOfLong& v) {
						idx_ = 2;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfFloat const & get_ArrayOfFloat() const {
						if (idx_ != 3) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfFloat const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfFloat& get_ArrayOfFloat()  {
						if (idx_ != 3) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfFloat& >(value_);
					}
					void set_ArrayOfFloat(const Energistics::Etp::v12::Datatypes::ArrayOfFloat& v) {
						idx_ = 3;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfDouble const & get_ArrayOfDouble() const {
						if (idx_ != 4) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfDouble const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfDouble& get_ArrayOfDouble()  {
						if (idx_ != 4) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfDouble& >(value_);
					}
					void set_ArrayOfDouble(const Energistics::Etp::v12::Datatypes::ArrayOfDouble& v) {
						idx_ = 4;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfString const & get_ArrayOfString() const {
						if (idx_ != 5) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfString const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfString& get_ArrayOfString()  {
						if (idx_ != 5) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfString& >(value_);
					}
					void set_ArrayOfString(const Energistics::Etp::v12::Datatypes::ArrayOfString& v) {
						idx_ = 5;
						value_ = v;
					}
					std::string const & get_bytes() const {
						if (idx_ != 6) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< std::string const & >(value_);
					}
					std::string& get_bytes()  {
						if (idx_ != 6) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< std::string& >(value_);
					}
					void set_bytes(const std::string& v) {
						idx_ = 6;
						value_ = v;
					}
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::AnyArrayitem_t> {

		static void encode(Encoder& e, Energistics::Etp::v12::Datatypes::AnyArrayitem_t v) {

			e.encodeUnionIndex(v.idx());
			switch (v.idx()) {
				case 0:
					avro::encode(e, v.get_ArrayOfBoolean());
					break;
				case 1:
					avro::encode(e, v.get_ArrayOfInt());
					break;
				case 2:
					avro::encode(e, v.get_ArrayOfLong());
					break;
				case 3:
					avro::encode(e, v.get_ArrayOfFloat());
					break;
				case 4:
					avro::encode(e, v.get_ArrayOfDouble());
					break;
				case 5:
					avro::encode(e, v.get_ArrayOfString());
					break;
				case 6:
					avro::encode(e, v.get_bytes());
					break;
			}
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::AnyArrayitem_t& v) {

			size_t n = d.decodeUnionIndex();
			if (n >= 7) { throw avro::Exception("Union index too big"); }
			switch (n) {
				case 0:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfBoolean vv;
						avro::decode(d, vv);
						v.set_ArrayOfBoolean(vv);
					}
					break;
				case 1:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfInt vv;
						avro::decode(d, vv);
						v.set_ArrayOfInt(vv);
					}
					break;
				case 2:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfLong vv;
						avro::decode(d, vv);
						v.set_ArrayOfLong(vv);
					}
					break;
				case 3:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfFloat vv;
						avro::decode(d, vv);
						v.set_ArrayOfFloat(vv);
					}
					break;
				case 4:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfDouble vv;
						avro::decode(d, vv);
						v.set_ArrayOfDouble(vv);
					}
					break;
				case 5:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfString vv;
						avro::decode(d, vv);
						v.set_ArrayOfString(vv);
					}
					break;
				case 6:
					{
						std::string vv;
						avro::decode(d, vv);
						v.set_bytes(vv);
					}
					break;
			}
		}
	};
}

namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct AnyArray {
					Energistics::Etp::v12::Datatypes::AnyArrayitem_t item;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::AnyArray> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::AnyArray& v) {
			avro::encode(e, v.item);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::AnyArray& v) {
			avro::decode(e, v.item);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct AnySubarray {
					int64_t start = 0;
					Energistics::Etp::v12::Datatypes::AnyArray slice;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::AnySubarray> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::AnySubarray& v) {
			avro::encode(e, v.start);
			avro::encode(e, v.slice);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::AnySubarray& v) {
			avro::decode(e, v.start);
			avro::decode(e, v.slice);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct AnySparseArray {
					std::vector<Energistics::Etp::v12::Datatypes::AnySubarray> slices;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::AnySparseArray> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::AnySparseArray& v) {
			avro::encode(e, v.slices);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::AnySparseArray& v) {
			avro::decode(e, v.slices);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct Contact {
					std::string organizationName;
					std::string contactName;
					std::string contactPhone;
					std::string contactEmail;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Contact> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Contact& v) {
			avro::encode(e, v.organizationName);
			avro::encode(e, v.contactName);
			avro::encode(e, v.contactPhone);
			avro::encode(e, v.contactEmail);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Contact& v) {
			avro::decode(e, v.organizationName);
			avro::decode(e, v.contactName);
			avro::decode(e, v.contactPhone);
			avro::decode(e, v.contactEmail);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class DataObjectCapabilityKind {
					ActiveTimeoutPeriod=0,
					MaxContainedDataObjectCount=1,
					MaxDataObjectSize=2,
					OrphanedChildrenPrunedOnDelete=3,
					SupportsGet=4,
					SupportsPut=5,
					SupportsDelete=6,
					MaxSecondaryIndexCount=7
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataObjectCapabilityKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::DataObjectCapabilityKind& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::DataObjectCapabilityKind>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::DataObjectCapabilityKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::DataObjectCapabilityKind>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct DataValueitem_t {
				private:
					size_t idx_=0;
					boost::any value_;

				public:
					size_t idx() const { return idx_; }
					bool is_null() const { return idx_==0; }
					void set_null() { idx_=0; value_ = boost::any(); }
					bool const & get_boolean() const {
						if (idx_ != 1) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< bool const & >(value_);
					}
					bool& get_boolean()  {
						if (idx_ != 1) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< bool& >(value_);
					}
					void set_boolean(const bool& v) {
						idx_ = 1;
						value_ = v;
					}
					int32_t const & get_int() const {
						if (idx_ != 2) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< int32_t const & >(value_);
					}
					int32_t& get_int()  {
						if (idx_ != 2) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< int32_t& >(value_);
					}
					void set_int(const int32_t& v) {
						idx_ = 2;
						value_ = v;
					}
					int64_t const & get_long() const {
						if (idx_ != 3) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< int64_t const & >(value_);
					}
					int64_t& get_long()  {
						if (idx_ != 3) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< int64_t& >(value_);
					}
					void set_long(const int64_t& v) {
						idx_ = 3;
						value_ = v;
					}
					float const & get_float() const {
						if (idx_ != 4) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< float const & >(value_);
					}
					float& get_float()  {
						if (idx_ != 4) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< float& >(value_);
					}
					void set_float(const float& v) {
						idx_ = 4;
						value_ = v;
					}
					double const & get_double() const {
						if (idx_ != 5) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< double const & >(value_);
					}
					double& get_double()  {
						if (idx_ != 5) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< double& >(value_);
					}
					void set_double(const double& v) {
						idx_ = 5;
						value_ = v;
					}
					std::string const & get_string() const {
						if (idx_ != 6) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< std::string const & >(value_);
					}
					std::string& get_string()  {
						if (idx_ != 6) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< std::string& >(value_);
					}
					void set_string(const std::string& v) {
						idx_ = 6;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfBoolean const & get_ArrayOfBoolean() const {
						if (idx_ != 7) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfBoolean const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfBoolean& get_ArrayOfBoolean()  {
						if (idx_ != 7) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfBoolean& >(value_);
					}
					void set_ArrayOfBoolean(const Energistics::Etp::v12::Datatypes::ArrayOfBoolean& v) {
						idx_ = 7;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean const & get_ArrayOfNullableBoolean() const {
						if (idx_ != 8) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean& get_ArrayOfNullableBoolean()  {
						if (idx_ != 8) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean& >(value_);
					}
					void set_ArrayOfNullableBoolean(const Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean& v) {
						idx_ = 8;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfInt const & get_ArrayOfInt() const {
						if (idx_ != 9) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfInt const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfInt& get_ArrayOfInt()  {
						if (idx_ != 9) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfInt& >(value_);
					}
					void set_ArrayOfInt(const Energistics::Etp::v12::Datatypes::ArrayOfInt& v) {
						idx_ = 9;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfNullableInt const & get_ArrayOfNullableInt() const {
						if (idx_ != 10) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfNullableInt const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfNullableInt& get_ArrayOfNullableInt()  {
						if (idx_ != 10) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfNullableInt& >(value_);
					}
					void set_ArrayOfNullableInt(const Energistics::Etp::v12::Datatypes::ArrayOfNullableInt& v) {
						idx_ = 10;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfLong const & get_ArrayOfLong() const {
						if (idx_ != 11) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfLong const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfLong& get_ArrayOfLong()  {
						if (idx_ != 11) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfLong& >(value_);
					}
					void set_ArrayOfLong(const Energistics::Etp::v12::Datatypes::ArrayOfLong& v) {
						idx_ = 11;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfNullableLong const & get_ArrayOfNullableLong() const {
						if (idx_ != 12) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfNullableLong const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfNullableLong& get_ArrayOfNullableLong()  {
						if (idx_ != 12) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfNullableLong& >(value_);
					}
					void set_ArrayOfNullableLong(const Energistics::Etp::v12::Datatypes::ArrayOfNullableLong& v) {
						idx_ = 12;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfFloat const & get_ArrayOfFloat() const {
						if (idx_ != 13) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfFloat const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfFloat& get_ArrayOfFloat()  {
						if (idx_ != 13) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfFloat& >(value_);
					}
					void set_ArrayOfFloat(const Energistics::Etp::v12::Datatypes::ArrayOfFloat& v) {
						idx_ = 13;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfDouble const & get_ArrayOfDouble() const {
						if (idx_ != 14) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfDouble const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfDouble& get_ArrayOfDouble()  {
						if (idx_ != 14) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfDouble& >(value_);
					}
					void set_ArrayOfDouble(const Energistics::Etp::v12::Datatypes::ArrayOfDouble& v) {
						idx_ = 14;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfString const & get_ArrayOfString() const {
						if (idx_ != 15) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfString const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfString& get_ArrayOfString()  {
						if (idx_ != 15) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfString& >(value_);
					}
					void set_ArrayOfString(const Energistics::Etp::v12::Datatypes::ArrayOfString& v) {
						idx_ = 15;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfBytes const & get_ArrayOfBytes() const {
						if (idx_ != 16) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfBytes const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ArrayOfBytes& get_ArrayOfBytes()  {
						if (idx_ != 16) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ArrayOfBytes& >(value_);
					}
					void set_ArrayOfBytes(const Energistics::Etp::v12::Datatypes::ArrayOfBytes& v) {
						idx_ = 16;
						value_ = v;
					}
					std::string const & get_bytes() const {
						if (idx_ != 17) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< std::string const & >(value_);
					}
					std::string& get_bytes()  {
						if (idx_ != 17) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< std::string& >(value_);
					}
					void set_bytes(const std::string& v) {
						idx_ = 17;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::AnySparseArray const & get_AnySparseArray() const {
						if (idx_ != 18) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::AnySparseArray const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::AnySparseArray& get_AnySparseArray()  {
						if (idx_ != 18) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::AnySparseArray& >(value_);
					}
					void set_AnySparseArray(const Energistics::Etp::v12::Datatypes::AnySparseArray& v) {
						idx_ = 18;
						value_ = v;
					}
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataValueitem_t> {

		static void encode(Encoder& e, Energistics::Etp::v12::Datatypes::DataValueitem_t v) {

			e.encodeUnionIndex(v.idx());
			switch (v.idx()) {
				case 0:
					e.encodeNull();
					break;
				case 1:
					avro::encode(e, v.get_boolean());
					break;
				case 2:
					avro::encode(e, v.get_int());
					break;
				case 3:
					avro::encode(e, v.get_long());
					break;
				case 4:
					avro::encode(e, v.get_float());
					break;
				case 5:
					avro::encode(e, v.get_double());
					break;
				case 6:
					avro::encode(e, v.get_string());
					break;
				case 7:
					avro::encode(e, v.get_ArrayOfBoolean());
					break;
				case 8:
					avro::encode(e, v.get_ArrayOfNullableBoolean());
					break;
				case 9:
					avro::encode(e, v.get_ArrayOfInt());
					break;
				case 10:
					avro::encode(e, v.get_ArrayOfNullableInt());
					break;
				case 11:
					avro::encode(e, v.get_ArrayOfLong());
					break;
				case 12:
					avro::encode(e, v.get_ArrayOfNullableLong());
					break;
				case 13:
					avro::encode(e, v.get_ArrayOfFloat());
					break;
				case 14:
					avro::encode(e, v.get_ArrayOfDouble());
					break;
				case 15:
					avro::encode(e, v.get_ArrayOfString());
					break;
				case 16:
					avro::encode(e, v.get_ArrayOfBytes());
					break;
				case 17:
					avro::encode(e, v.get_bytes());
					break;
				case 18:
					avro::encode(e, v.get_AnySparseArray());
					break;
			}
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::DataValueitem_t& v) {

			size_t n = d.decodeUnionIndex();
			if (n >= 19) { throw avro::Exception("Union index too big"); }
			switch (n) {
				case 0:
					{
						d.decodeNull();
						v.set_null();
					}
					break;
				case 1:
					{
						bool vv;
						avro::decode(d, vv);
						v.set_boolean(vv);
					}
					break;
				case 2:
					{
						int32_t vv;
						avro::decode(d, vv);
						v.set_int(vv);
					}
					break;
				case 3:
					{
						int64_t vv;
						avro::decode(d, vv);
						v.set_long(vv);
					}
					break;
				case 4:
					{
						float vv;
						avro::decode(d, vv);
						v.set_float(vv);
					}
					break;
				case 5:
					{
						double vv;
						avro::decode(d, vv);
						v.set_double(vv);
					}
					break;
				case 6:
					{
						std::string vv;
						avro::decode(d, vv);
						v.set_string(vv);
					}
					break;
				case 7:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfBoolean vv;
						avro::decode(d, vv);
						v.set_ArrayOfBoolean(vv);
					}
					break;
				case 8:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean vv;
						avro::decode(d, vv);
						v.set_ArrayOfNullableBoolean(vv);
					}
					break;
				case 9:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfInt vv;
						avro::decode(d, vv);
						v.set_ArrayOfInt(vv);
					}
					break;
				case 10:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfNullableInt vv;
						avro::decode(d, vv);
						v.set_ArrayOfNullableInt(vv);
					}
					break;
				case 11:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfLong vv;
						avro::decode(d, vv);
						v.set_ArrayOfLong(vv);
					}
					break;
				case 12:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfNullableLong vv;
						avro::decode(d, vv);
						v.set_ArrayOfNullableLong(vv);
					}
					break;
				case 13:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfFloat vv;
						avro::decode(d, vv);
						v.set_ArrayOfFloat(vv);
					}
					break;
				case 14:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfDouble vv;
						avro::decode(d, vv);
						v.set_ArrayOfDouble(vv);
					}
					break;
				case 15:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfString vv;
						avro::decode(d, vv);
						v.set_ArrayOfString(vv);
					}
					break;
				case 16:
					{
						Energistics::Etp::v12::Datatypes::ArrayOfBytes vv;
						avro::decode(d, vv);
						v.set_ArrayOfBytes(vv);
					}
					break;
				case 17:
					{
						std::string vv;
						avro::decode(d, vv);
						v.set_bytes(vv);
					}
					break;
				case 18:
					{
						Energistics::Etp::v12::Datatypes::AnySparseArray vv;
						avro::decode(d, vv);
						v.set_AnySparseArray(vv);
					}
					break;
			}
		}
	};
}

namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct DataValue {
					Energistics::Etp::v12::Datatypes::DataValueitem_t item;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataValue> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::DataValue& v) {
			avro::encode(e, v.item);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::DataValue& v) {
			avro::decode(e, v.item);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct DataAttribute {
					int32_t attributeId = 0;
					Energistics::Etp::v12::Datatypes::DataValue attributeValue;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataAttribute> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::DataAttribute& v) {
			avro::encode(e, v.attributeId);
			avro::encode(e, v.attributeValue);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::DataAttribute& v) {
			avro::decode(e, v.attributeId);
			avro::decode(e, v.attributeValue);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class EndpointCapabilityKind {
					ActiveTimeoutPeriod=0,
					AuthorizationDetails=1,
					ChangePropagationPeriod=2,
					ChangeRetentionPeriod=3,
					MaxConcurrentMultipart=4,
					MaxDataObjectSize=5,
					MaxPartSize=6,
					MaxSessionClientCount=7,
					MaxSessionGlobalCount=8,
					MaxWebSocketFramePayloadSize=9,
					MaxWebSocketMessagePayloadSize=10,
					MultipartMessageTimeoutPeriod=11,
					ResponseTimeoutPeriod=12,
					RequestSessionTimeoutPeriod=13,
					SessionEstablishmentTimeoutPeriod=14,
					SupportsAlternateRequestUris=15,
					SupportsMessageHeaderExtensions=16
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::EndpointCapabilityKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::EndpointCapabilityKind& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::EndpointCapabilityKind>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::EndpointCapabilityKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::EndpointCapabilityKind>(e.decodeEnum());
		}
	};
}
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
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ErrorInfo {
					std::string message;
					int32_t code = 0;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ErrorInfo> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ErrorInfo& v) {
			avro::encode(e, v.message);
			avro::encode(e, v.code);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ErrorInfo& v) {
			avro::decode(e, v.message);
			avro::decode(e, v.code);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct ProtocolException {
						boost::optional<Energistics::Etp::v12::Datatypes::ErrorInfo> error;
						bool has_error() const { return error.is_initialized(); }
						Energistics::Etp::v12::Datatypes::ErrorInfo get_error() const { return error.get(); }
						std::map<std::string, Energistics::Etp::v12::Datatypes::ErrorInfo> errors;
						static constexpr int messageTypeId=1000;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Core);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Core::ProtocolException> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Core::ProtocolException& v) {
			avro::encode(e, v.error);
			avro::encode(e, v.errors);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Core::ProtocolException& v) {
			avro::decode(e, v.error);
			avro::decode(e, v.errors);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct MessageHeaderExtension {
					std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> extension;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::MessageHeaderExtension> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::MessageHeaderExtension& v) {
			avro::encode(e, v.extension);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::MessageHeaderExtension& v) {
			avro::decode(e, v.extension);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class ProtocolCapabilityKind {
					FrameChangeDetectionPeriod=0,
					MaxDataArraySize=1,
					MaxDataObjectSize=2,
					MaxFrameResponseRowCount=3,
					MaxIndexCount=4,
					MaxRangeChannelCount=5,
					MaxRangeDataItemCount=6,
					MaxResponseCount=7,
					MaxStreamingChannelsSessionCount=8,
					MaxSubscriptionSessionCount=9,
					MaxTransactionCount=10,
					SupportsSecondaryIndexFiltering=11,
					TransactionTimeoutPeriod=12
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ProtocolCapabilityKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ProtocolCapabilityKind& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::ProtocolCapabilityKind>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ProtocolCapabilityKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::ProtocolCapabilityKind>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct SupportedDataObject {
					std::string qualifiedType;
					std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> dataObjectCapabilities;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::SupportedDataObject> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::SupportedDataObject& v) {
			avro::encode(e, v.qualifiedType);
			avro::encode(e, v.dataObjectCapabilities);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::SupportedDataObject& v) {
			avro::decode(e, v.qualifiedType);
			avro::decode(e, v.dataObjectCapabilities);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct Uuid{
					std::array<uint8_t, 16> array{{}};
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Uuid> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Uuid& v) {
			avro::encode(e, v.array);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Uuid& v) {
			avro::decode(e, v.array);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct Chunk {
						Energistics::Etp::v12::Datatypes::Uuid blobId;
						std::string data;
						bool final=false;
						static constexpr int messageTypeId=9;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreNotification::Chunk> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreNotification::Chunk& v) {
			avro::encode(e, v.blobId);
			avro::encode(e, v.data);
			avro::encode(e, v.final);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreNotification::Chunk& v) {
			avro::decode(e, v.blobId);
			avro::decode(e, v.data);
			avro::decode(e, v.final);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct ObjectAccessRevoked {
						std::string uri;
						int64_t changeTime;
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;
						static constexpr int messageTypeId=5;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreNotification::ObjectAccessRevoked> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreNotification::ObjectAccessRevoked& v) {
			avro::encode(e, v.uri);
			avro::encode(e, v.changeTime);
			avro::encode(e, v.requestUuid);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreNotification::ObjectAccessRevoked& v) {
			avro::decode(e, v.uri);
			avro::decode(e, v.changeTime);
			avro::decode(e, v.requestUuid);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct ObjectDeleted {
						std::string uri;
						int64_t changeTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;
						static constexpr int messageTypeId=3;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreNotification::ObjectDeleted> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreNotification::ObjectDeleted& v) {
			avro::encode(e, v.uri);
			avro::encode(e, v.changeTime);
			avro::encode(e, v.requestUuid);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreNotification::ObjectDeleted& v) {
			avro::decode(e, v.uri);
			avro::decode(e, v.changeTime);
			avro::decode(e, v.requestUuid);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct SubscriptionEnded {
						std::string reason;
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;
						static constexpr int messageTypeId=7;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreNotification::SubscriptionEnded> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreNotification::SubscriptionEnded& v) {
			avro::encode(e, v.reason);
			avro::encode(e, v.requestUuid);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreNotification::SubscriptionEnded& v) {
			avro::decode(e, v.reason);
			avro::decode(e, v.requestUuid);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct UnsubscribeNotifications {
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;
						static constexpr int messageTypeId=4;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreNotification::UnsubscribeNotifications> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreNotification::UnsubscribeNotifications& v) {
			avro::encode(e, v.requestUuid);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreNotification::UnsubscribeNotifications& v) {
			avro::decode(e, v.requestUuid);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct Chunk {
						Energistics::Etp::v12::Datatypes::Uuid blobId;
						std::string data;
						bool final=false;
						static constexpr int messageTypeId=8;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Store);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Store::Chunk> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Store::Chunk& v) {
			avro::encode(e, v.blobId);
			avro::encode(e, v.data);
			avro::encode(e, v.final);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Store::Chunk& v) {
			avro::decode(e, v.blobId);
			avro::decode(e, v.data);
			avro::decode(e, v.final);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreQuery {
					struct Chunk {
						Energistics::Etp::v12::Datatypes::Uuid blobId;
						std::string data;
						bool final=false;
						static constexpr int messageTypeId=3;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreQuery);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreQuery::Chunk> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreQuery::Chunk& v) {
			avro::encode(e, v.blobId);
			avro::encode(e, v.data);
			avro::encode(e, v.final);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreQuery::Chunk& v) {
			avro::decode(e, v.blobId);
			avro::decode(e, v.data);
			avro::decode(e, v.final);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct CommitTransaction {
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						static constexpr int messageTypeId=3;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Transaction::CommitTransaction> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Transaction::CommitTransaction& v) {
			avro::encode(e, v.transactionUuid);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Transaction::CommitTransaction& v) {
			avro::decode(e, v.transactionUuid);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct CommitTransactionResponse {
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						bool successful=false;
						std::string failureReason;
						static constexpr int messageTypeId=5;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Transaction::CommitTransactionResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Transaction::CommitTransactionResponse& v) {
			avro::encode(e, v.transactionUuid);
			avro::encode(e, v.successful);
			avro::encode(e, v.failureReason);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Transaction::CommitTransactionResponse& v) {
			avro::decode(e, v.transactionUuid);
			avro::decode(e, v.successful);
			avro::decode(e, v.failureReason);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct RollbackTransaction {
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						static constexpr int messageTypeId=4;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction& v) {
			avro::encode(e, v.transactionUuid);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Transaction::RollbackTransaction& v) {
			avro::decode(e, v.transactionUuid);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct RollbackTransactionResponse {
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						bool successful=false;
						std::string failureReason;
						static constexpr int messageTypeId=6;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Transaction::RollbackTransactionResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Transaction::RollbackTransactionResponse& v) {
			avro::encode(e, v.transactionUuid);
			avro::encode(e, v.successful);
			avro::encode(e, v.failureReason);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Transaction::RollbackTransactionResponse& v) {
			avro::decode(e, v.transactionUuid);
			avro::decode(e, v.successful);
			avro::decode(e, v.failureReason);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct StartTransactionResponse {
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						bool successful=false;
						std::string failureReason;
						static constexpr int messageTypeId=2;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Transaction::StartTransactionResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Transaction::StartTransactionResponse& v) {
			avro::encode(e, v.transactionUuid);
			avro::encode(e, v.successful);
			avro::encode(e, v.failureReason);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Transaction::StartTransactionResponse& v) {
			avro::decode(e, v.transactionUuid);
			avro::decode(e, v.successful);
			avro::decode(e, v.failureReason);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct Version {
					int32_t major = 1;
					int32_t minor = 2;
					int32_t revision = 0;
					int32_t patch = 0;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Version> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Version& v) {
			avro::encode(e, v.major);
			avro::encode(e, v.minor);
			avro::encode(e, v.revision);
			avro::encode(e, v.patch);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Version& v) {
			avro::decode(e, v.major);
			avro::decode(e, v.minor);
			avro::decode(e, v.revision);
			avro::decode(e, v.patch);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct SupportedProtocol {
					int32_t protocol = 0;
					Energistics::Etp::v12::Datatypes::Version protocolVersion;
					std::string role;
					std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> protocolCapabilities;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::SupportedProtocol> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::SupportedProtocol& v) {
			avro::encode(e, v.protocol);
			avro::encode(e, v.protocolVersion);
			avro::encode(e, v.role);
			avro::encode(e, v.protocolCapabilities);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::SupportedProtocol& v) {
			avro::decode(e, v.protocol);
			avro::decode(e, v.protocolVersion);
			avro::decode(e, v.role);
			avro::decode(e, v.protocolCapabilities);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct OpenSession {
						std::string applicationName;
						std::string applicationVersion;
						Energistics::Etp::v12::Datatypes::Uuid serverInstanceId;
						std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> supportedProtocols;
						std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> supportedDataObjects;
						std::string supportedCompression;
						std::vector<std::string> supportedFormats;
						int64_t currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						int64_t earliestRetainedChangeTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						Energistics::Etp::v12::Datatypes::Uuid sessionId;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> endpointCapabilities;
						static constexpr int messageTypeId=2;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Core);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Core::OpenSession> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Core::OpenSession& v) {
			avro::encode(e, v.applicationName);
			avro::encode(e, v.applicationVersion);
			avro::encode(e, v.serverInstanceId);
			avro::encode(e, v.supportedProtocols);
			avro::encode(e, v.supportedDataObjects);
			avro::encode(e, v.supportedCompression);
			avro::encode(e, v.supportedFormats);
			avro::encode(e, v.currentDateTime);
			avro::encode(e, v.earliestRetainedChangeTime);
			avro::encode(e, v.sessionId);
			avro::encode(e, v.endpointCapabilities);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Core::OpenSession& v) {
			avro::decode(e, v.applicationName);
			avro::decode(e, v.applicationVersion);
			avro::decode(e, v.serverInstanceId);
			avro::decode(e, v.supportedProtocols);
			avro::decode(e, v.supportedDataObjects);
			avro::decode(e, v.supportedCompression);
			avro::decode(e, v.supportedFormats);
			avro::decode(e, v.currentDateTime);
			avro::decode(e, v.earliestRetainedChangeTime);
			avro::decode(e, v.sessionId);
			avro::decode(e, v.endpointCapabilities);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct RequestSession {
						std::string applicationName;
						std::string applicationVersion;
						Energistics::Etp::v12::Datatypes::Uuid clientInstanceId;
						std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> requestedProtocols;
						std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> supportedDataObjects;
						std::vector<std::string> supportedCompression;
						std::vector<std::string> supportedFormats = { "xml" };
						int64_t currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						int64_t earliestRetainedChangeTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						bool serverAuthorizationRequired=false;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> endpointCapabilities;
						static constexpr int messageTypeId=1;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Core);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Core::RequestSession> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Core::RequestSession& v) {
			avro::encode(e, v.applicationName);
			avro::encode(e, v.applicationVersion);
			avro::encode(e, v.clientInstanceId);
			avro::encode(e, v.requestedProtocols);
			avro::encode(e, v.supportedDataObjects);
			avro::encode(e, v.supportedCompression);
			avro::encode(e, v.supportedFormats);
			avro::encode(e, v.currentDateTime);
			avro::encode(e, v.earliestRetainedChangeTime);
			avro::encode(e, v.serverAuthorizationRequired);
			avro::encode(e, v.endpointCapabilities);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Core::RequestSession& v) {
			avro::decode(e, v.applicationName);
			avro::decode(e, v.applicationVersion);
			avro::decode(e, v.clientInstanceId);
			avro::decode(e, v.requestedProtocols);
			avro::decode(e, v.supportedDataObjects);
			avro::decode(e, v.supportedCompression);
			avro::decode(e, v.supportedFormats);
			avro::decode(e, v.currentDateTime);
			avro::decode(e, v.earliestRetainedChangeTime);
			avro::decode(e, v.serverAuthorizationRequired);
			avro::decode(e, v.endpointCapabilities);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ServerCapabilities {
					std::string applicationName;
					std::string applicationVersion;
					Energistics::Etp::v12::Datatypes::Contact contactInformation;
					std::vector<std::string> supportedCompression;
					std::vector<std::string> supportedEncodings;
					std::vector<std::string> supportedFormats;
					std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> supportedDataObjects;
					std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> supportedProtocols;
					std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> endpointCapabilities;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ServerCapabilities> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ServerCapabilities& v) {
			avro::encode(e, v.applicationName);
			avro::encode(e, v.applicationVersion);
			avro::encode(e, v.contactInformation);
			avro::encode(e, v.supportedCompression);
			avro::encode(e, v.supportedEncodings);
			avro::encode(e, v.supportedFormats);
			avro::encode(e, v.supportedDataObjects);
			avro::encode(e, v.supportedProtocols);
			avro::encode(e, v.endpointCapabilities);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ServerCapabilities& v) {
			avro::decode(e, v.applicationName);
			avro::decode(e, v.applicationVersion);
			avro::decode(e, v.contactInformation);
			avro::decode(e, v.supportedCompression);
			avro::decode(e, v.supportedEncodings);
			avro::decode(e, v.supportedFormats);
			avro::decode(e, v.supportedDataObjects);
			avro::decode(e, v.supportedProtocols);
			avro::decode(e, v.endpointCapabilities);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct ChannelChangeRequestInfo {
						int64_t sinceChangeTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						std::vector<int64_t> channelIds;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::ChannelChangeRequestInfo> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::ChannelChangeRequestInfo& v) {
			avro::encode(e, v.sinceChangeTime);
			avro::encode(e, v.channelIds);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::ChannelChangeRequestInfo& v) {
			avro::decode(e, v.sinceChangeTime);
			avro::decode(e, v.channelIds);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					enum class ChannelDataKind {
						DateTime=0,
						ElapsedTime=1,
						MeasuredDepth=2,
						PassIndexedDepth=3,
						TrueVerticalDepth=4,
						typeBoolean=5,
						typeInt=6,
						typeLong=7,
						typeFloat=8,
						typeDouble=9,
						typeString=10,
						typeBytes=11
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct AttributeMetadataRecord {
					int32_t attributeId = 0;
					std::string attributeName;
					Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind dataKind;
					std::string uom;
					std::string depthDatum;
					std::string attributePropertyKindUri;
					std::vector<int32_t> axisVectorLengths;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::AttributeMetadataRecord> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::AttributeMetadataRecord& v) {
			avro::encode(e, v.attributeId);
			avro::encode(e, v.attributeName);
			avro::encode(e, v.dataKind);
			avro::encode(e, v.uom);
			avro::encode(e, v.depthDatum);
			avro::encode(e, v.attributePropertyKindUri);
			avro::encode(e, v.axisVectorLengths);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::AttributeMetadataRecord& v) {
			avro::decode(e, v.attributeId);
			avro::decode(e, v.attributeName);
			avro::decode(e, v.dataKind);
			avro::decode(e, v.uom);
			avro::decode(e, v.depthDatum);
			avro::decode(e, v.attributePropertyKindUri);
			avro::decode(e, v.axisVectorLengths);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					enum class ChannelIndexKind {
						DateTime=0,
						ElapsedTime=1,
						MeasuredDepth=2,
						TrueVerticalDepth=3,
						PassIndexedDepth=4,
						Pressure=5,
						Temperature=6,
						Scalar=7
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::ChannelIndexKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::ChannelIndexKind& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::ChannelData::ChannelIndexKind>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::ChannelIndexKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::ChannelData::ChannelIndexKind>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct FramePoint {
						Energistics::Etp::v12::Datatypes::DataValue value;
						std::vector<Energistics::Etp::v12::Datatypes::DataAttribute> valueAttributes;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::FramePoint> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::FramePoint& v) {
			avro::encode(e, v.value);
			avro::encode(e, v.valueAttributes);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::FramePoint& v) {
			avro::decode(e, v.value);
			avro::decode(e, v.valueAttributes);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					enum class IndexDirection {
						Increasing=0,
						Decreasing=1,
						Unordered=2
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::IndexDirection> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::IndexDirection& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::ChannelData::IndexDirection>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::IndexDirection& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::ChannelData::IndexDirection>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					enum class PassDirection {
						Up=0,
						HoldingSteady=1,
						Down=2
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::PassDirection> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::PassDirection& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::ChannelData::PassDirection>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::PassDirection& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::ChannelData::PassDirection>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct PassIndexedDepth {
						int64_t pass = 0;
						Energistics::Etp::v12::Datatypes::ChannelData::PassDirection direction = Energistics::Etp::v12::Datatypes::ChannelData::PassDirection::Up;
						double depth = .0;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth& v) {
			avro::encode(e, v.pass);
			avro::encode(e, v.direction);
			avro::encode(e, v.depth);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth& v) {
			avro::decode(e, v.pass);
			avro::decode(e, v.direction);
			avro::decode(e, v.depth);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct IndexValueitem_t {
				private:
					size_t idx_=0;
					boost::any value_;

				public:
					size_t idx() const { return idx_; }
					bool is_null() const { return idx_==0; }
					void set_null() { idx_=0; value_ = boost::any(); }
					int64_t const & get_long() const {
						if (idx_ != 1) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< int64_t const & >(value_);
					}
					int64_t& get_long()  {
						if (idx_ != 1) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< int64_t& >(value_);
					}
					void set_long(const int64_t& v) {
						idx_ = 1;
						value_ = v;
					}
					double const & get_double() const {
						if (idx_ != 2) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< double const & >(value_);
					}
					double& get_double()  {
						if (idx_ != 2) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< double& >(value_);
					}
					void set_double(const double& v) {
						idx_ = 2;
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth const & get_PassIndexedDepth() const {
						if (idx_ != 3) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth const & >(value_);
					}
					Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth& get_PassIndexedDepth()  {
						if (idx_ != 3) {
							throw avro::Exception("Invalid type for union.");
						}
						return boost::any_cast< Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth& >(value_);
					}
					void set_PassIndexedDepth(const Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth& v) {
						idx_ = 3;
						value_ = v;
					}
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::IndexValueitem_t> {

		static void encode(Encoder& e, Energistics::Etp::v12::Datatypes::IndexValueitem_t v) {

			e.encodeUnionIndex(v.idx());
			switch (v.idx()) {
				case 0:
					e.encodeNull();
					break;
				case 1:
					avro::encode(e, v.get_long());
					break;
				case 2:
					avro::encode(e, v.get_double());
					break;
				case 3:
					avro::encode(e, v.get_PassIndexedDepth());
					break;
			}
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::IndexValueitem_t& v) {

			size_t n = d.decodeUnionIndex();
			if (n >= 4) { throw avro::Exception("Union index too big"); }
			switch (n) {
				case 0:
					{
						d.decodeNull();
						v.set_null();
					}
					break;
				case 1:
					{
						int64_t vv;
						avro::decode(d, vv);
						v.set_long(vv);
					}
					break;
				case 2:
					{
						double vv;
						avro::decode(d, vv);
						v.set_double(vv);
					}
					break;
				case 3:
					{
						Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth vv;
						avro::decode(d, vv);
						v.set_PassIndexedDepth(vv);
					}
					break;
			}
		}
	};
}

namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct IndexValue {
					Energistics::Etp::v12::Datatypes::IndexValueitem_t item;
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::IndexValue> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::IndexValue& v) {
			avro::encode(e, v.item);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::IndexValue& v) {
			avro::decode(e, v.item);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct ChannelSubscribeInfo {
						int64_t channelId = 0;
						Energistics::Etp::v12::Datatypes::IndexValue startIndex;
						bool dataChanges=false;
						boost::optional<int32_t> requestLatestIndexCount;
						bool has_requestLatestIndexCount() const { return requestLatestIndexCount.is_initialized(); }
						int32_t get_requestLatestIndexCount() const { return requestLatestIndexCount.get(); }
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::ChannelSubscribeInfo> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::ChannelSubscribeInfo& v) {
			avro::encode(e, v.channelId);
			avro::encode(e, v.startIndex);
			avro::encode(e, v.dataChanges);
			avro::encode(e, v.requestLatestIndexCount);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::ChannelSubscribeInfo& v) {
			avro::decode(e, v.channelId);
			avro::decode(e, v.startIndex);
			avro::decode(e, v.dataChanges);
			avro::decode(e, v.requestLatestIndexCount);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct DataItem {
						int64_t channelId = 0;
						std::vector<Energistics::Etp::v12::Datatypes::IndexValue> indexes;
						Energistics::Etp::v12::Datatypes::DataValue value;
						std::vector<Energistics::Etp::v12::Datatypes::DataAttribute> valueAttributes;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::DataItem> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::DataItem& v) {
			avro::encode(e, v.channelId);
			avro::encode(e, v.indexes);
			avro::encode(e, v.value);
			avro::encode(e, v.valueAttributes);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::DataItem& v) {
			avro::decode(e, v.channelId);
			avro::decode(e, v.indexes);
			avro::decode(e, v.value);
			avro::decode(e, v.valueAttributes);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct FrameRow {
						std::vector<Energistics::Etp::v12::Datatypes::IndexValue> indexes;
						std::vector<Energistics::Etp::v12::Datatypes::ChannelData::FramePoint> points;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::FrameRow> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::FrameRow& v) {
			avro::encode(e, v.indexes);
			avro::encode(e, v.points);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::FrameRow& v) {
			avro::decode(e, v.indexes);
			avro::decode(e, v.points);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct TruncateInfo {
						int64_t channelId = 0;
						Energistics::Etp::v12::Datatypes::IndexValue newEndIndex;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::TruncateInfo> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::TruncateInfo& v) {
			avro::encode(e, v.channelId);
			avro::encode(e, v.newEndIndex);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::TruncateInfo& v) {
			avro::decode(e, v.channelId);
			avro::decode(e, v.newEndIndex);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace DataArrayTypes {
					struct DataArray {
						std::vector<int64_t> dimensions;
						Energistics::Etp::v12::Datatypes::AnyArray data;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray& v) {
			avro::encode(e, v.dimensions);
			avro::encode(e, v.data);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray& v) {
			avro::decode(e, v.dimensions);
			avro::decode(e, v.data);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataArraysResponse {
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray> dataArrays;
						static constexpr int messageTypeId=1;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse& v) {
			avro::encode(e, v.dataArrays);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse& v) {
			avro::decode(e, v.dataArrays);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataSubarraysResponse {
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray> dataSubarrays;
						static constexpr int messageTypeId=8;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::GetDataSubarraysResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::GetDataSubarraysResponse& v) {
			avro::encode(e, v.dataSubarrays);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::GetDataSubarraysResponse& v) {
			avro::decode(e, v.dataSubarrays);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace DataArrayTypes {
					struct DataArrayIdentifier {
						std::string uri;
						std::string pathInResource;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier& v) {
			avro::encode(e, v.uri);
			avro::encode(e, v.pathInResource);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier& v) {
			avro::decode(e, v.uri);
			avro::decode(e, v.pathInResource);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataArrayMetadata {
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier> dataArrays;
						static constexpr int messageTypeId=6;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata& v) {
			avro::encode(e, v.dataArrays);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata& v) {
			avro::decode(e, v.dataArrays);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataArrays {
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier> dataArrays;
						static constexpr int messageTypeId=2;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::GetDataArrays> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::GetDataArrays& v) {
			avro::encode(e, v.dataArrays);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::GetDataArrays& v) {
			avro::decode(e, v.dataArrays);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace DataArrayTypes {
					struct GetDataSubarraysType {
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier uid;
						std::vector<int64_t> starts;
						std::vector<int64_t> counts;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataArrayTypes::GetDataSubarraysType> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::DataArrayTypes::GetDataSubarraysType& v) {
			avro::encode(e, v.uid);
			avro::encode(e, v.starts);
			avro::encode(e, v.counts);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::DataArrayTypes::GetDataSubarraysType& v) {
			avro::decode(e, v.uid);
			avro::decode(e, v.starts);
			avro::decode(e, v.counts);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataSubarrays {
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::GetDataSubarraysType> dataSubarrays;
						static constexpr int messageTypeId=3;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays& v) {
			avro::encode(e, v.dataSubarrays);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays& v) {
			avro::decode(e, v.dataSubarrays);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace DataArrayTypes {
					struct DataArrayMetadata {
						std::vector<int64_t> dimensions;
						std::vector<int64_t> preferredSubarrayDimensions;
						Energistics::Etp::v12::Datatypes::AnyArrayType transportArrayType;
						Energistics::Etp::v12::Datatypes::AnyLogicalArrayType logicalArrayType;
						int64_t storeLastWrite = 0;
						int64_t storeCreated = 0;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata& v) {
			avro::encode(e, v.dimensions);
			avro::encode(e, v.preferredSubarrayDimensions);
			avro::encode(e, v.transportArrayType);
			avro::encode(e, v.logicalArrayType);
			avro::encode(e, v.storeLastWrite);
			avro::encode(e, v.storeCreated);
			avro::encode(e, v.customData);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata& v) {
			avro::decode(e, v.dimensions);
			avro::decode(e, v.preferredSubarrayDimensions);
			avro::decode(e, v.transportArrayType);
			avro::decode(e, v.logicalArrayType);
			avro::decode(e, v.storeLastWrite);
			avro::decode(e, v.storeCreated);
			avro::decode(e, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataArrayMetadataResponse {
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata> arrayMetadata;
						static constexpr int messageTypeId=7;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadataResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadataResponse& v) {
			avro::encode(e, v.arrayMetadata);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadataResponse& v) {
			avro::decode(e, v.arrayMetadata);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace DataArrayTypes {
					struct PutDataArraysType {
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier uid;
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray array;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataArraysType> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataArraysType& v) {
			avro::encode(e, v.uid);
			avro::encode(e, v.array);
			avro::encode(e, v.customData);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataArraysType& v) {
			avro::decode(e, v.uid);
			avro::decode(e, v.array);
			avro::decode(e, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutDataArrays {
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataArraysType> dataArrays;
						static constexpr int messageTypeId=4;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::PutDataArrays> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::PutDataArrays& v) {
			avro::encode(e, v.dataArrays);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::PutDataArrays& v) {
			avro::decode(e, v.dataArrays);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace DataArrayTypes {
					struct PutDataSubarraysType {
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier uid;
						Energistics::Etp::v12::Datatypes::AnyArray data;
						std::vector<int64_t> starts;
						std::vector<int64_t> counts;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataSubarraysType> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataSubarraysType& v) {
			avro::encode(e, v.uid);
			avro::encode(e, v.data);
			avro::encode(e, v.starts);
			avro::encode(e, v.counts);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataSubarraysType& v) {
			avro::decode(e, v.uid);
			avro::decode(e, v.data);
			avro::decode(e, v.starts);
			avro::decode(e, v.counts);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutDataSubarrays {
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataSubarraysType> dataSubarrays;
						static constexpr int messageTypeId=5;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays& v) {
			avro::encode(e, v.dataSubarrays);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays& v) {
			avro::decode(e, v.dataSubarrays);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace DataArrayTypes {
					struct PutUninitializedDataArrayType {
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier uid;
						Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata metadata;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataArrayTypes::PutUninitializedDataArrayType> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::DataArrayTypes::PutUninitializedDataArrayType& v) {
			avro::encode(e, v.uid);
			avro::encode(e, v.metadata);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutUninitializedDataArrayType& v) {
			avro::decode(e, v.uid);
			avro::decode(e, v.metadata);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutUninitializedDataArrays {
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutUninitializedDataArrayType> dataArrays;
						static constexpr int messageTypeId=9;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArrays> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArrays& v) {
			avro::encode(e, v.dataArrays);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArrays& v) {
			avro::decode(e, v.dataArrays);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					enum class ActiveStatusKind {
						Active=0,
						Inactive=1
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct FrameChannelMetadataRecord {
						std::string uri;
						std::string channelName;
						Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind dataKind = Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind::DateTime;
						std::string uom;
						std::string depthDatum;
						std::string channelPropertyKindUri;
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind status = Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind::Active;
						std::string source;
						std::vector<int32_t> axisVectorLengths;
						std::vector<Energistics::Etp::v12::Datatypes::AttributeMetadataRecord> attributeMetadata;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::FrameChannelMetadataRecord> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::FrameChannelMetadataRecord& v) {
			avro::encode(e, v.uri);
			avro::encode(e, v.channelName);
			avro::encode(e, v.dataKind);
			avro::encode(e, v.uom);
			avro::encode(e, v.depthDatum);
			avro::encode(e, v.channelPropertyKindUri);
			avro::encode(e, v.status);
			avro::encode(e, v.source);
			avro::encode(e, v.axisVectorLengths);
			avro::encode(e, v.attributeMetadata);
			avro::encode(e, v.customData);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::FrameChannelMetadataRecord& v) {
			avro::decode(e, v.uri);
			avro::decode(e, v.channelName);
			avro::decode(e, v.dataKind);
			avro::decode(e, v.uom);
			avro::decode(e, v.depthDatum);
			avro::decode(e, v.channelPropertyKindUri);
			avro::decode(e, v.status);
			avro::decode(e, v.source);
			avro::decode(e, v.axisVectorLengths);
			avro::decode(e, v.attributeMetadata);
			avro::decode(e, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					enum class ContextScopeKind {
						self=0,
						sources=1,
						targets=2,
						sourcesOrSelf=3,
						targetsOrSelf=4
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::ContextScopeKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::ContextScopeKind& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Object::ContextScopeKind>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::ContextScopeKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::Object::ContextScopeKind>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct Dataspace {
						std::string uri;
						std::string path;
						int64_t storeLastWrite = 0;
						int64_t storeCreated = 0;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::Dataspace> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::Dataspace& v) {
			avro::encode(e, v.uri);
			avro::encode(e, v.path);
			avro::encode(e, v.storeLastWrite);
			avro::encode(e, v.storeCreated);
			avro::encode(e, v.customData);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::Dataspace& v) {
			avro::decode(e, v.uri);
			avro::decode(e, v.path);
			avro::decode(e, v.storeLastWrite);
			avro::decode(e, v.storeCreated);
			avro::decode(e, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct GetDataspacesResponse {
						std::vector<Energistics::Etp::v12::Datatypes::Object::Dataspace> dataspaces;
						static constexpr int messageTypeId=2;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Dataspace::GetDataspacesResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Dataspace::GetDataspacesResponse& v) {
			avro::encode(e, v.dataspaces);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Dataspace::GetDataspacesResponse& v) {
			avro::decode(e, v.dataspaces);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct PutDataspaces {
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::Dataspace> dataspaces;
						static constexpr int messageTypeId=3;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces& v) {
			avro::encode(e, v.dataspaces);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Dataspace::PutDataspaces& v) {
			avro::decode(e, v.dataspaces);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct DeletedResource {
						std::string uri;
						int64_t deletedTime = 0;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::DeletedResource> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::DeletedResource& v) {
			avro::encode(e, v.uri);
			avro::encode(e, v.deletedTime);
			avro::encode(e, v.customData);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::DeletedResource& v) {
			avro::decode(e, v.uri);
			avro::decode(e, v.deletedTime);
			avro::decode(e, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Discovery {
					struct GetDeletedResourcesResponse {
						std::vector<Energistics::Etp::v12::Datatypes::Object::DeletedResource> deletedResources;
						static constexpr int messageTypeId=6;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Discovery::GetDeletedResourcesResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Discovery::GetDeletedResourcesResponse& v) {
			avro::encode(e, v.deletedResources);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Discovery::GetDeletedResourcesResponse& v) {
			avro::decode(e, v.deletedResources);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct IndexInterval {
						Energistics::Etp::v12::Datatypes::IndexValue startIndex;
						Energistics::Etp::v12::Datatypes::IndexValue endIndex;
						std::string uom;
						std::string depthDatum;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::IndexInterval> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::IndexInterval& v) {
			avro::encode(e, v.startIndex);
			avro::encode(e, v.endIndex);
			avro::encode(e, v.uom);
			avro::encode(e, v.depthDatum);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::IndexInterval& v) {
			avro::decode(e, v.startIndex);
			avro::decode(e, v.endIndex);
			avro::decode(e, v.uom);
			avro::decode(e, v.depthDatum);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct ChannelRangeInfo {
						std::vector<int64_t> channelIds;
						Energistics::Etp::v12::Datatypes::Object::IndexInterval interval;
						std::vector<Energistics::Etp::v12::Datatypes::Object::IndexInterval> secondaryIntervals;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::ChannelRangeInfo> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::ChannelRangeInfo& v) {
			avro::encode(e, v.channelIds);
			avro::encode(e, v.interval);
			avro::encode(e, v.secondaryIntervals);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::ChannelRangeInfo& v) {
			avro::decode(e, v.channelIds);
			avro::decode(e, v.interval);
			avro::decode(e, v.secondaryIntervals);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct IndexMetadataRecord {
						Energistics::Etp::v12::Datatypes::ChannelData::ChannelIndexKind indexKind;
						Energistics::Etp::v12::Datatypes::Object::IndexInterval interval;
						Energistics::Etp::v12::Datatypes::ChannelData::IndexDirection direction;
						std::string name;
						std::string uom;
						std::string depthDatum;
						std::string indexPropertyKindUri;
						bool filterable=false;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::IndexMetadataRecord> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::IndexMetadataRecord& v) {
			avro::encode(e, v.indexKind);
			avro::encode(e, v.interval);
			avro::encode(e, v.direction);
			avro::encode(e, v.name);
			avro::encode(e, v.uom);
			avro::encode(e, v.depthDatum);
			avro::encode(e, v.indexPropertyKindUri);
			avro::encode(e, v.filterable);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::IndexMetadataRecord& v) {
			avro::decode(e, v.indexKind);
			avro::decode(e, v.interval);
			avro::decode(e, v.direction);
			avro::decode(e, v.name);
			avro::decode(e, v.uom);
			avro::decode(e, v.depthDatum);
			avro::decode(e, v.indexPropertyKindUri);
			avro::decode(e, v.filterable);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct ChannelMetadataRecord {
						std::string uri;
						int64_t id = 0;
						std::vector<Energistics::Etp::v12::Datatypes::ChannelData::IndexMetadataRecord> indexes;
						std::string channelName;
						Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind dataKind = Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind::DateTime;
						std::string uom;
						std::string depthDatum;
						std::string channelClassUri;
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind status = Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind::Active;
						std::string source;
						std::vector<int32_t> axisVectorLengths;
						std::vector<Energistics::Etp::v12::Datatypes::AttributeMetadataRecord> attributeMetadata;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::ChannelMetadataRecord> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::ChannelMetadataRecord& v) {
			avro::encode(e, v.uri);
			avro::encode(e, v.id);
			avro::encode(e, v.indexes);
			avro::encode(e, v.channelName);
			avro::encode(e, v.dataKind);
			avro::encode(e, v.uom);
			avro::encode(e, v.depthDatum);
			avro::encode(e, v.channelClassUri);
			avro::encode(e, v.status);
			avro::encode(e, v.source);
			avro::encode(e, v.axisVectorLengths);
			avro::encode(e, v.attributeMetadata);
			avro::encode(e, v.customData);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::ChannelMetadataRecord& v) {
			avro::decode(e, v.uri);
			avro::decode(e, v.id);
			avro::decode(e, v.indexes);
			avro::decode(e, v.channelName);
			avro::decode(e, v.dataKind);
			avro::decode(e, v.uom);
			avro::decode(e, v.depthDatum);
			avro::decode(e, v.channelClassUri);
			avro::decode(e, v.status);
			avro::decode(e, v.source);
			avro::decode(e, v.axisVectorLengths);
			avro::decode(e, v.attributeMetadata);
			avro::decode(e, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					struct OpenChannelInfo {
						Energistics::Etp::v12::Datatypes::ChannelData::ChannelMetadataRecord metadata;
						bool preferRealtime=false;
						bool dataChanges=false;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::OpenChannelInfo> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::OpenChannelInfo& v) {
			avro::encode(e, v.metadata);
			avro::encode(e, v.preferRealtime);
			avro::encode(e, v.dataChanges);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::ChannelData::OpenChannelInfo& v) {
			avro::decode(e, v.metadata);
			avro::decode(e, v.preferRealtime);
			avro::decode(e, v.dataChanges);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct ChangeAnnotation {
						int64_t changeTime = 0;
						Energistics::Etp::v12::Datatypes::Object::IndexInterval interval;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::ChangeAnnotation> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::ChangeAnnotation& v) {
			avro::encode(e, v.changeTime);
			avro::encode(e, v.interval);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::ChangeAnnotation& v) {
			avro::decode(e, v.changeTime);
			avro::decode(e, v.interval);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct ChangeResponseInfo {
						int64_t responseTimestamp = 0;
						std::map<std::string, std::vector<Energistics::Etp::v12::Datatypes::Object::ChangeAnnotation> > changes;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::ChangeResponseInfo> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::ChangeResponseInfo& v) {
			avro::encode(e, v.responseTimestamp);
			avro::encode(e, v.changes);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::ChangeResponseInfo& v) {
			avro::decode(e, v.responseTimestamp);
			avro::decode(e, v.changes);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					enum class ObjectChangeKind {
						insert=0,
						update=1,
						authorized=2,
						joined=3,
						unjoined=4,
						joinedSubscription=5,
						unjoinedSubscription=6
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct ObjectPart {
						std::string uid;
						std::string data;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::ObjectPart> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::ObjectPart& v) {
			avro::encode(e, v.uid);
			avro::encode(e, v.data);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::ObjectPart& v) {
			avro::decode(e, v.uid);
			avro::decode(e, v.data);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct PartsMetadataInfo {
						std::string uri;
						std::string name;
						Energistics::Etp::v12::Datatypes::ChannelData::IndexMetadataRecord index;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::PartsMetadataInfo> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::PartsMetadataInfo& v) {
			avro::encode(e, v.uri);
			avro::encode(e, v.name);
			avro::encode(e, v.index);
			avro::encode(e, v.customData);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::PartsMetadataInfo& v) {
			avro::decode(e, v.uri);
			avro::decode(e, v.name);
			avro::decode(e, v.index);
			avro::decode(e, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct PutResponse {
						std::vector<std::string> createdContainedObjectUris;
						std::vector<std::string> deletedContainedObjectUris;
						std::vector<std::string> joinedContainedObjectUris;
						std::vector<std::string> unjoinedContainedObjectUris;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::PutResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::PutResponse& v) {
			avro::encode(e, v.createdContainedObjectUris);
			avro::encode(e, v.deletedContainedObjectUris);
			avro::encode(e, v.joinedContainedObjectUris);
			avro::encode(e, v.unjoinedContainedObjectUris);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::PutResponse& v) {
			avro::decode(e, v.createdContainedObjectUris);
			avro::decode(e, v.deletedContainedObjectUris);
			avro::decode(e, v.joinedContainedObjectUris);
			avro::decode(e, v.unjoinedContainedObjectUris);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct PutDataObjectsResponse {
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::PutResponse> success;
						static constexpr int messageTypeId=9;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Store);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Store::PutDataObjectsResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Store::PutDataObjectsResponse& v) {
			avro::encode(e, v.success);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Store::PutDataObjectsResponse& v) {
			avro::decode(e, v.success);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					enum class RelationshipKind {
						Primary=0,
						Secondary=1,
						Both=2
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::RelationshipKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::RelationshipKind& v) {
			e.encodeEnum(static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Object::RelationshipKind>::type>(v));
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::RelationshipKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::Object::RelationshipKind>(e.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct ContextInfo {
						std::string uri;
						int32_t depth = 1;
						std::vector<std::string> dataObjectTypes;
						Energistics::Etp::v12::Datatypes::Object::RelationshipKind navigableEdges = Energistics::Etp::v12::Datatypes::Object::RelationshipKind::Primary;
						bool includeSecondaryTargets=false;
						bool includeSecondarySources=false;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::ContextInfo> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::ContextInfo& v) {
			avro::encode(e, v.uri);
			avro::encode(e, v.depth);
			avro::encode(e, v.dataObjectTypes);
			avro::encode(e, v.navigableEdges);
			avro::encode(e, v.includeSecondaryTargets);
			avro::encode(e, v.includeSecondarySources);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::ContextInfo& v) {
			avro::decode(e, v.uri);
			avro::decode(e, v.depth);
			avro::decode(e, v.dataObjectTypes);
			avro::decode(e, v.navigableEdges);
			avro::decode(e, v.includeSecondaryTargets);
			avro::decode(e, v.includeSecondarySources);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Discovery {
					struct GetResources {
						Energistics::Etp::v12::Datatypes::Object::ContextInfo context;
						Energistics::Etp::v12::Datatypes::Object::ContextScopeKind scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets;
						bool countObjects=false;
						boost::optional<int64_t> storeLastWriteFilter;
						bool has_storeLastWriteFilter() const { return storeLastWriteFilter.is_initialized(); }
						int64_t get_storeLastWriteFilter() const { return storeLastWriteFilter.get(); }
						boost::optional<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind> activeStatusFilter;
						bool has_activeStatusFilter() const { return activeStatusFilter.is_initialized(); }
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind get_activeStatusFilter() const { return activeStatusFilter.get(); }
						bool includeEdges=false;
						static constexpr int messageTypeId=1;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Discovery::GetResources> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Discovery::GetResources& v) {
			avro::encode(e, v.context);
			avro::encode(e, v.scope);
			avro::encode(e, v.countObjects);
			avro::encode(e, v.storeLastWriteFilter);
			avro::encode(e, v.activeStatusFilter);
			avro::encode(e, v.includeEdges);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Discovery::GetResources& v) {
			avro::decode(e, v.context);
			avro::decode(e, v.scope);
			avro::decode(e, v.countObjects);
			avro::decode(e, v.storeLastWriteFilter);
			avro::decode(e, v.activeStatusFilter);
			avro::decode(e, v.includeEdges);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DiscoveryQuery {
					struct FindResources {
						Energistics::Etp::v12::Datatypes::Object::ContextInfo context;
						Energistics::Etp::v12::Datatypes::Object::ContextScopeKind scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets;
						boost::optional<int64_t> storeLastWriteFilter;
						bool has_storeLastWriteFilter() const { return storeLastWriteFilter.is_initialized(); }
						int64_t get_storeLastWriteFilter() const { return storeLastWriteFilter.get(); }
						boost::optional<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind> activeStatusFilter;
						bool has_activeStatusFilter() const { return activeStatusFilter.is_initialized(); }
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind get_activeStatusFilter() const { return activeStatusFilter.get(); }
						static constexpr int messageTypeId=1;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DiscoveryQuery);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DiscoveryQuery::FindResources> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DiscoveryQuery::FindResources& v) {
			avro::encode(e, v.context);
			avro::encode(e, v.scope);
			avro::encode(e, v.storeLastWriteFilter);
			avro::encode(e, v.activeStatusFilter);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DiscoveryQuery::FindResources& v) {
			avro::decode(e, v.context);
			avro::decode(e, v.scope);
			avro::decode(e, v.storeLastWriteFilter);
			avro::decode(e, v.activeStatusFilter);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreQuery {
					struct FindDataObjects {
						Energistics::Etp::v12::Datatypes::Object::ContextInfo context;
						Energistics::Etp::v12::Datatypes::Object::ContextScopeKind scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets;
						boost::optional<int64_t> storeLastWriteFilter;
						bool has_storeLastWriteFilter() const { return storeLastWriteFilter.is_initialized(); }
						int64_t get_storeLastWriteFilter() const { return storeLastWriteFilter.get(); }
						boost::optional<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind> activeStatusFilter;
						bool has_activeStatusFilter() const { return activeStatusFilter.is_initialized(); }
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind get_activeStatusFilter() const { return activeStatusFilter.get(); }
						std::string format;
						static constexpr int messageTypeId=1;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreQuery);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreQuery::FindDataObjects> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreQuery::FindDataObjects& v) {
			avro::encode(e, v.context);
			avro::encode(e, v.scope);
			avro::encode(e, v.storeLastWriteFilter);
			avro::encode(e, v.activeStatusFilter);
			avro::encode(e, v.format);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreQuery::FindDataObjects& v) {
			avro::decode(e, v.context);
			avro::decode(e, v.scope);
			avro::decode(e, v.storeLastWriteFilter);
			avro::decode(e, v.activeStatusFilter);
			avro::decode(e, v.format);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct Edge {
						std::string sourceUri;
						std::string targetUri;
						Energistics::Etp::v12::Datatypes::Object::RelationshipKind relationshipKind = Energistics::Etp::v12::Datatypes::Object::RelationshipKind::Primary;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::Edge> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::Edge& v) {
			avro::encode(e, v.sourceUri);
			avro::encode(e, v.targetUri);
			avro::encode(e, v.relationshipKind);
			avro::encode(e, v.customData);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::Edge& v) {
			avro::decode(e, v.sourceUri);
			avro::decode(e, v.targetUri);
			avro::decode(e, v.relationshipKind);
			avro::decode(e, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Discovery {
					struct GetResourcesEdgesResponse {
						std::vector<Energistics::Etp::v12::Datatypes::Object::Edge> edges;
						static constexpr int messageTypeId=7;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Discovery::GetResourcesEdgesResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Discovery::GetResourcesEdgesResponse& v) {
			avro::encode(e, v.edges);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Discovery::GetResourcesEdgesResponse& v) {
			avro::decode(e, v.edges);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct Resource {
						std::string uri;
						std::vector<std::string> alternateUris;
						std::string name;
						boost::optional<int32_t> sourceCount;
						bool has_sourceCount() const { return sourceCount.is_initialized(); }
						int32_t get_sourceCount() const { return sourceCount.get(); }
						boost::optional<int32_t> targetCount;
						bool has_targetCount() const { return targetCount.is_initialized(); }
						int32_t get_targetCount() const { return targetCount.get(); }
						int64_t lastChanged = 0;
						int64_t storeLastWrite = 0;
						int64_t storeCreated = 0;
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind activeStatus = Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind::Inactive;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> customData;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::Resource> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::Resource& v) {
			avro::encode(e, v.uri);
			avro::encode(e, v.alternateUris);
			avro::encode(e, v.name);
			avro::encode(e, v.sourceCount);
			avro::encode(e, v.targetCount);
			avro::encode(e, v.lastChanged);
			avro::encode(e, v.storeLastWrite);
			avro::encode(e, v.storeCreated);
			avro::encode(e, v.activeStatus);
			avro::encode(e, v.customData);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::Resource& v) {
			avro::decode(e, v.uri);
			avro::decode(e, v.alternateUris);
			avro::decode(e, v.name);
			avro::decode(e, v.sourceCount);
			avro::decode(e, v.targetCount);
			avro::decode(e, v.lastChanged);
			avro::decode(e, v.storeLastWrite);
			avro::decode(e, v.storeCreated);
			avro::decode(e, v.activeStatus);
			avro::decode(e, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Discovery {
					struct GetResourcesResponse {
						std::vector<Energistics::Etp::v12::Datatypes::Object::Resource> resources;
						static constexpr int messageTypeId=4;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Discovery::GetResourcesResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Discovery::GetResourcesResponse& v) {
			avro::encode(e, v.resources);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Discovery::GetResourcesResponse& v) {
			avro::decode(e, v.resources);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DiscoveryQuery {
					struct FindResourcesResponse {
						std::vector<Energistics::Etp::v12::Datatypes::Object::Resource> resources;
						std::string serverSortOrder;
						static constexpr int messageTypeId=2;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::DiscoveryQuery);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::DiscoveryQuery::FindResourcesResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::DiscoveryQuery::FindResourcesResponse& v) {
			avro::encode(e, v.resources);
			avro::encode(e, v.serverSortOrder);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::DiscoveryQuery::FindResourcesResponse& v) {
			avro::decode(e, v.resources);
			avro::decode(e, v.serverSortOrder);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct ObjectActiveStatusChanged {
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind activeStatus = Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind::Inactive;
						int64_t changeTime = 0;
						Energistics::Etp::v12::Datatypes::Object::Resource resource;
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;
						static constexpr int messageTypeId=11;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreNotification::ObjectActiveStatusChanged> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreNotification::ObjectActiveStatusChanged& v) {
			avro::encode(e, v.activeStatus);
			avro::encode(e, v.changeTime);
			avro::encode(e, v.resource);
			avro::encode(e, v.requestUuid);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreNotification::ObjectActiveStatusChanged& v) {
			avro::decode(e, v.activeStatus);
			avro::decode(e, v.changeTime);
			avro::decode(e, v.resource);
			avro::decode(e, v.requestUuid);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct DataObject {
						Energistics::Etp::v12::Datatypes::Object::Resource resource;
						std::string format;
						boost::optional<Energistics::Etp::v12::Datatypes::Uuid> blobId;
						bool has_blobId() const { return blobId.is_initialized(); }
						Energistics::Etp::v12::Datatypes::Uuid get_blobId() const { return blobId.get(); }
						std::string data;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::DataObject> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::DataObject& v) {
			avro::encode(e, v.resource);
			avro::encode(e, v.format);
			avro::encode(e, v.blobId);
			avro::encode(e, v.data);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::DataObject& v) {
			avro::decode(e, v.resource);
			avro::decode(e, v.format);
			avro::decode(e, v.blobId);
			avro::decode(e, v.data);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct GetDataObjectsResponse {
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> dataObjects;
						static constexpr int messageTypeId=4;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Store);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Store::GetDataObjectsResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Store::GetDataObjectsResponse& v) {
			avro::encode(e, v.dataObjects);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Store::GetDataObjectsResponse& v) {
			avro::decode(e, v.dataObjects);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct PutDataObjects {
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> dataObjects;
						bool pruneContainedObjects=false;
						static constexpr int messageTypeId=2;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::Store);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::Store::PutDataObjects> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::Store::PutDataObjects& v) {
			avro::encode(e, v.dataObjects);
			avro::encode(e, v.pruneContainedObjects);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::Store::PutDataObjects& v) {
			avro::decode(e, v.dataObjects);
			avro::decode(e, v.pruneContainedObjects);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreQuery {
					struct FindDataObjectsResponse {
						std::vector<Energistics::Etp::v12::Datatypes::Object::DataObject> dataObjects;
						std::string serverSortOrder;
						static constexpr int messageTypeId=2;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreQuery);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreQuery::FindDataObjectsResponse> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreQuery::FindDataObjectsResponse& v) {
			avro::encode(e, v.dataObjects);
			avro::encode(e, v.serverSortOrder);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreQuery::FindDataObjectsResponse& v) {
			avro::decode(e, v.dataObjects);
			avro::decode(e, v.serverSortOrder);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct ObjectChange {
						Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind changeKind = Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind::insert;
						int64_t changeTime = 0;
						Energistics::Etp::v12::Datatypes::Object::DataObject dataObject;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::ObjectChange> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::ObjectChange& v) {
			avro::encode(e, v.changeKind);
			avro::encode(e, v.changeTime);
			avro::encode(e, v.dataObject);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::ObjectChange& v) {
			avro::decode(e, v.changeKind);
			avro::decode(e, v.changeTime);
			avro::decode(e, v.dataObject);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct ObjectChanged {
						Energistics::Etp::v12::Datatypes::Object::ObjectChange change;
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;
						static constexpr int messageTypeId=2;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreNotification::ObjectChanged> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreNotification::ObjectChanged& v) {
			avro::encode(e, v.change);
			avro::encode(e, v.requestUuid);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreNotification::ObjectChanged& v) {
			avro::decode(e, v.change);
			avro::decode(e, v.requestUuid);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct SubscriptionInfo {
						Energistics::Etp::v12::Datatypes::Object::ContextInfo context;
						Energistics::Etp::v12::Datatypes::Object::ContextScopeKind scope= Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets;
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;
						bool includeObjectData=false;
						std::string format;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::SubscriptionInfo> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::SubscriptionInfo& v) {
			avro::encode(e, v.context);
			avro::encode(e, v.scope);
			avro::encode(e, v.requestUuid);
			avro::encode(e, v.includeObjectData);
			avro::encode(e, v.format);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::SubscriptionInfo& v) {
			avro::decode(e, v.context);
			avro::decode(e, v.scope);
			avro::decode(e, v.requestUuid);
			avro::decode(e, v.includeObjectData);
			avro::decode(e, v.format);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct SubscribeNotifications {
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::SubscriptionInfo> request;
						static constexpr int messageTypeId=6;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotifications> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotifications& v) {
			avro::encode(e, v.request);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreNotification::SubscribeNotifications& v) {
			avro::decode(e, v.request);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct UnsolicitedStoreNotifications {
						std::vector<Energistics::Etp::v12::Datatypes::Object::SubscriptionInfo> subscriptions;
						static constexpr int messageTypeId=8;
						static constexpr uint16_t protocolId = static_cast<std::underlying_type<Energistics::Etp::v12::Datatypes::Protocol>::type>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Protocol::StoreNotification::UnsolicitedStoreNotifications> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Protocol::StoreNotification::UnsolicitedStoreNotifications& v) {
			avro::encode(e, v.subscriptions);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Protocol::StoreNotification::UnsolicitedStoreNotifications& v) {
			avro::decode(e, v.subscriptions);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct SupportedType {
						std::string dataObjectType;
						boost::optional<int32_t> objectCount;
						bool has_objectCount() const { return objectCount.is_initialized(); }
						int32_t get_objectCount() const { return objectCount.get(); }
						Energistics::Etp::v12::Datatypes::Object::RelationshipKind relationshipKind = Energistics::Etp::v12::Datatypes::Object::RelationshipKind::Primary;
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::SupportedType> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::SupportedType& v) {
			avro::encode(e, v.dataObjectType);
			avro::encode(e, v.objectCount);
			avro::encode(e, v.relationshipKind);
		}
		static void decode(Decoder& e, Energistics::Etp::v12::Datatypes::Object::SupportedType& v) {
			avro::decode(e, v.dataObjectType);
			avro::decode(e, v.objectCount);
			avro::decode(e, v.relationshipKind);
		}
	};
}
namespace avro {
	template<> struct codec_traits<boost::optional<bool>> {
		static void encode(Encoder& e, boost::optional<bool> v) {
			if (v) {
				e.encodeUnionIndex(1);
				avro::encode(e, v.get());
			}
			else {
				e.encodeUnionIndex(0);
				e.encodeNull();
			}
		}
		static void decode(Decoder& d, boost::optional<bool>& v) {
			size_t n = d.decodeUnionIndex();
			if (n >= 2) { throw avro::Exception("Union index too big"); }
			switch (n) {
				case 0:
					{
						d.decodeNull();
						v = boost::none;
					}
					break;
				case 1:
					{
						bool vv;
						avro::decode(d, vv);
						v.emplace(vv);
					}
					break;
			}
		}
	};
}

namespace avro {
	template<> struct codec_traits<boost::optional<int32_t>> {
		static void encode(Encoder& e, boost::optional<int32_t> v) {
			if (v) {
				e.encodeUnionIndex(1);
				avro::encode(e, v.get());
			}
			else {
				e.encodeUnionIndex(0);
				e.encodeNull();
			}
		}
		static void decode(Decoder& d, boost::optional<int32_t>& v) {
			size_t n = d.decodeUnionIndex();
			if (n >= 2) { throw avro::Exception("Union index too big"); }
			switch (n) {
				case 0:
					{
						d.decodeNull();
						v = boost::none;
					}
					break;
				case 1:
					{
						int32_t vv;
						avro::decode(d, vv);
						v.emplace(vv);
					}
					break;
			}
		}
	};
}

namespace avro {
	template<> struct codec_traits<boost::optional<int64_t>> {
		static void encode(Encoder& e, boost::optional<int64_t> v) {
			if (v) {
				e.encodeUnionIndex(1);
				avro::encode(e, v.get());
			}
			else {
				e.encodeUnionIndex(0);
				e.encodeNull();
			}
		}
		static void decode(Decoder& d, boost::optional<int64_t>& v) {
			size_t n = d.decodeUnionIndex();
			if (n >= 2) { throw avro::Exception("Union index too big"); }
			switch (n) {
				case 0:
					{
						d.decodeNull();
						v = boost::none;
					}
					break;
				case 1:
					{
						int64_t vv;
						avro::decode(d, vv);
						v.emplace(vv);
					}
					break;
			}
		}
	};
}

namespace avro {
	template<> struct codec_traits<boost::optional<Energistics::Etp::v12::Datatypes::Uuid>> {
		static void encode(Encoder& e, boost::optional<Energistics::Etp::v12::Datatypes::Uuid> v) {
			if (v) {
				e.encodeUnionIndex(1);
				avro::encode(e, v.get());
			}
			else {
				e.encodeUnionIndex(0);
				e.encodeNull();
			}
		}
		static void decode(Decoder& d, boost::optional<Energistics::Etp::v12::Datatypes::Uuid>& v) {
			size_t n = d.decodeUnionIndex();
			if (n >= 2) { throw avro::Exception("Union index too big"); }
			switch (n) {
				case 0:
					{
						d.decodeNull();
						v = boost::none;
					}
					break;
				case 1:
					{
						Energistics::Etp::v12::Datatypes::Uuid vv;
						avro::decode(d, vv);
						v.emplace(vv);
					}
					break;
			}
		}
	};
}

namespace avro {
	template<> struct codec_traits<boost::optional<Energistics::Etp::v12::Datatypes::ErrorInfo>> {
		static void encode(Encoder& e, boost::optional<Energistics::Etp::v12::Datatypes::ErrorInfo> v) {
			if (v) {
				e.encodeUnionIndex(1);
				avro::encode(e, v.get());
			}
			else {
				e.encodeUnionIndex(0);
				e.encodeNull();
			}
		}
		static void decode(Decoder& d, boost::optional<Energistics::Etp::v12::Datatypes::ErrorInfo>& v) {
			size_t n = d.decodeUnionIndex();
			if (n >= 2) { throw avro::Exception("Union index too big"); }
			switch (n) {
				case 0:
					{
						d.decodeNull();
						v = boost::none;
					}
					break;
				case 1:
					{
						Energistics::Etp::v12::Datatypes::ErrorInfo vv;
						avro::decode(d, vv);
						v.emplace(vv);
					}
					break;
			}
		}
	};
}

namespace avro {
	template<> struct codec_traits<boost::optional<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind>> {
		static void encode(Encoder& e, boost::optional<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind> v) {
			if (v) {
				e.encodeUnionIndex(1);
				avro::encode(e, v.get());
			}
			else {
				e.encodeUnionIndex(0);
				e.encodeNull();
			}
		}
		static void decode(Decoder& d, boost::optional<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind>& v) {
			size_t n = d.decodeUnionIndex();
			if (n >= 2) { throw avro::Exception("Union index too big"); }
			switch (n) {
				case 0:
					{
						d.decodeNull();
						v = boost::none;
					}
					break;
				case 1:
					{
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind vv;
						avro::decode(d, vv);
						v.emplace(vv);
					}
					break;
			}
		}
	};
}

#endif
