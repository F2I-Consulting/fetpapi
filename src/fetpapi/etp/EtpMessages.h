#ifndef ETP_MESSAGES__
#define ETP_MESSAGES__

#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include <avro/Exception.hh>

#include "EtpMessage.h"

namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class Protocol : int32_t {
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
					SupportedTypes = 25,
					CoreOSDU = 2400,
					StoreOSDU = 2404,
					DataspaceOSDU = 2424
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Protocol> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Protocol& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Protocol& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::Protocol>(d.decodeEnum());
		}
	};
}

namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct Acknowledge : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1001;

						Acknowledge() {
							this->messageHeader.messageType = messageTypeId;
						}

						void encode(avro::Encoder&) const final {}
						void decode(avro::Decoder&) final {}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct Authorize : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 6;

						Authorize() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Core);
							this->messageHeader.messageType = messageTypeId;
						}
						std::string authorization;
						std::map<std::string, std::string> supplementalAuthorization;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, authorization);
							avro::encode(e, supplementalAuthorization);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, authorization);
							avro::decode(d, supplementalAuthorization);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct AuthorizeResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 7;

						AuthorizeResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Core);
							this->messageHeader.messageType = messageTypeId;
						}
						bool success = false;
						std::vector<std::string> challenges;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, success);
							avro::encode(e, challenges);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, success);
							avro::decode(d, challenges);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct CloseSession : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 5;

						CloseSession() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Core);
							this->messageHeader.messageType = messageTypeId;
						}
						std::string reason;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, reason);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, reason);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct Pong : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 9;

						Pong() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Core);
							this->messageHeader.messageType = messageTypeId;
						}
						int64_t currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

						void encode(avro::Encoder& e) const final {
							avro::encode(e, currentDateTime);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, currentDateTime);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct Ping : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 8;

						Ping() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Core);
							this->messageHeader.messageType = messageTypeId;
						}
						int64_t currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

						void encode(avro::Encoder& e) const final {
							avro::encode(e, currentDateTime);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, currentDateTime);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutDataArraysResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 10;

						PutDataArraysResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> success;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, success);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, success);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutDataSubarraysResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 11;

						PutDataSubarraysResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> success;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, success);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, success);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutUninitializedDataArraysResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 12;

						PutUninitializedDataArraysResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> success;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, success);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, success);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct DeleteDataspaces : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 4;

						DeleteDataspaces() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> uris;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, uris);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, uris);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct DeleteDataspacesResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 5;

						DeleteDataspacesResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> success;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, success);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, success);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct GetDataspaces : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1;

						GetDataspaces() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
							this->messageHeader.messageType = messageTypeId;
						}
						std::optional<int64_t> storeLastWriteFilter;
						bool has_storeLastWriteFilter() const { return storeLastWriteFilter.has_value(); }
						int64_t get_storeLastWriteFilter() const { return storeLastWriteFilter.value(); }

						void encode(avro::Encoder& e) const final {
							avro::encode(e, storeLastWriteFilter);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, storeLastWriteFilter);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct PutDataspacesResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 6;

						PutDataspacesResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> success;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, success);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, success);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Discovery {
					struct GetDeletedResources : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 5;

						GetDeletedResources() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
							this->messageHeader.messageType = messageTypeId;
						}
						std::string dataspaceUri;
						std::optional<int64_t> deleteTimeFilter;
						bool has_deleteTimeFilter() const { return deleteTimeFilter.has_value(); }
						int64_t get_deleteTimeFilter() const { return deleteTimeFilter.value(); }
						std::vector<std::string> dataObjectTypes;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataspaceUri);
							avro::encode(e, deleteTimeFilter);
							avro::encode(e, dataObjectTypes);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataspaceUri);
							avro::decode(d, deleteTimeFilter);
							avro::decode(d, dataObjectTypes);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct SubscribeNotificationsResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 10;

						SubscribeNotificationsResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> success;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, success);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, success);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct DeleteDataObjects : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 3;

						DeleteDataObjects() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Store);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> uris;
						bool pruneContainedObjects = false;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, uris);
							avro::encode(e, pruneContainedObjects);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, uris);
							avro::decode(d, pruneContainedObjects);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct GetDataObjects : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1;

						GetDataObjects() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Store);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> uris;
						std::string format = "xml";

						void encode(avro::Encoder& e) const final {
							avro::encode(e, uris);
							avro::encode(e, format);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, uris);
							avro::decode(d, format);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct StartTransaction : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1;

						StartTransaction() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
							this->messageHeader.messageType = messageTypeId;
						}
						bool readOnly = false;
						std::string message;
						std::vector<std::string> dataspaceUris;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, readOnly);
							avro::encode(e, message);
							avro::encode(e, dataspaceUris);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, readOnly);
							avro::decode(d, message);
							avro::decode(d, dataspaceUris);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class AnyArrayType {
					arrayOfBoolean = 0,
					arrayOfInt = 1,
					arrayOfLong = 2,
					arrayOfFloat = 3,
					arrayOfDouble = 4,
					arrayOfString = 5,
					bytes = 6
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::AnyArrayType> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::AnyArrayType& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::AnyArrayType& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::AnyArrayType>(d.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class AnyLogicalArrayType {
					arrayOfBoolean = 0,
					arrayOfInt8 = 1,
					arrayOfUInt8 = 2,
					arrayOfInt16LE = 3,
					arrayOfInt32LE = 4,
					arrayOfInt64LE = 5,
					arrayOfUInt16LE = 6,
					arrayOfUInt32LE = 7,
					arrayOfUInt64LE = 8,
					arrayOfFloat32LE = 9,
					arrayOfDouble64LE = 10,
					arrayOfInt16BE = 11,
					arrayOfInt32BE = 12,
					arrayOfInt64BE = 13,
					arrayOfUInt16BE = 14,
					arrayOfUInt32BE = 15,
					arrayOfUInt64BE = 16,
					arrayOfFloat32BE = 17,
					arrayOfDouble64BE = 18,
					arrayOfString = 19,
					arrayOfCustom = 20
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::AnyLogicalArrayType> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::AnyLogicalArrayType& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::AnyLogicalArrayType& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::AnyLogicalArrayType>(d.decodeEnum());
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ArrayOfBoolean& v) {
			avro::decode(d, v.values);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ArrayOfBytes& v) {
			avro::decode(d, v.values);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ArrayOfDouble& v) {
			avro::decode(d, v.values);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ArrayOfFloat& v) {
			avro::decode(d, v.values);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ArrayOfInt& v) {
			avro::decode(d, v.values);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ArrayOfLong& v) {
			avro::decode(d, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfNullableBoolean {
					std::vector<std::optional<bool>> values;
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean& v) {
			avro::decode(d, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfNullableInt {
					std::vector<std::optional<int32_t>> values;
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ArrayOfNullableInt& v) {
			avro::decode(d, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct ArrayOfNullableLong {
					std::vector<std::optional<int64_t>> values;
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ArrayOfNullableLong& v) {
			avro::decode(d, v.values);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ArrayOfString& v) {
			avro::decode(d, v.values);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct DeleteDataObjectsResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 10;

						DeleteDataObjectsResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Store);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::ArrayOfString> deletedUris;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, deletedUris);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, deletedUris);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct AnyArrayitem_t {
				private:
					std::variant<
						Energistics::Etp::v12::Datatypes::ArrayOfBoolean,
						Energistics::Etp::v12::Datatypes::ArrayOfInt,
						Energistics::Etp::v12::Datatypes::ArrayOfLong,
						Energistics::Etp::v12::Datatypes::ArrayOfFloat,
						Energistics::Etp::v12::Datatypes::ArrayOfDouble,
						Energistics::Etp::v12::Datatypes::ArrayOfString,
						std::string
					> value_;

				public:
					size_t idx() const { return value_.index(); }
					Energistics::Etp::v12::Datatypes::ArrayOfBoolean const& get_ArrayOfBoolean() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfBoolean>(value_);
					}
					void set_ArrayOfBoolean(const Energistics::Etp::v12::Datatypes::ArrayOfBoolean& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfBoolean>(v);
					}
					void set_ArrayOfBoolean(const Energistics::Etp::v12::Datatypes::ArrayOfBoolean&& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfBoolean>(std::move(v));
					}
					Energistics::Etp::v12::Datatypes::ArrayOfInt const& get_ArrayOfInt() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfInt>(value_);
					}
					void set_ArrayOfInt(const Energistics::Etp::v12::Datatypes::ArrayOfInt& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfInt>(v);
					}
					void set_ArrayOfInt(const Energistics::Etp::v12::Datatypes::ArrayOfInt&& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfInt>(std::move(v));
					}
					Energistics::Etp::v12::Datatypes::ArrayOfLong const& get_ArrayOfLong() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfLong>(value_);
					}
					void set_ArrayOfLong(const Energistics::Etp::v12::Datatypes::ArrayOfLong& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfLong>(v);
					}
					void set_ArrayOfLong(const Energistics::Etp::v12::Datatypes::ArrayOfLong&& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfLong>(std::move(v));
					}
					Energistics::Etp::v12::Datatypes::ArrayOfFloat const& get_ArrayOfFloat() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfFloat>(value_);
					}
					void set_ArrayOfFloat(const Energistics::Etp::v12::Datatypes::ArrayOfFloat& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfFloat>(v);
					}
					void set_ArrayOfFloat(const Energistics::Etp::v12::Datatypes::ArrayOfFloat&& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfFloat>(std::move(v));
					}
					Energistics::Etp::v12::Datatypes::ArrayOfDouble const& get_ArrayOfDouble() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfDouble>(value_);
					}
					void set_ArrayOfDouble(const Energistics::Etp::v12::Datatypes::ArrayOfDouble& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfDouble>(v);
					}
					void set_ArrayOfDouble(const Energistics::Etp::v12::Datatypes::ArrayOfDouble&& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfDouble>(std::move(v));
					}
					Energistics::Etp::v12::Datatypes::ArrayOfString const& get_ArrayOfString() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfString>(value_);
					}
					void set_ArrayOfString(const Energistics::Etp::v12::Datatypes::ArrayOfString& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfString>(v);
					}
					void set_ArrayOfString(const Energistics::Etp::v12::Datatypes::ArrayOfString&& v) {
						value_.emplace<Energistics::Etp::v12::Datatypes::ArrayOfString>(std::move(v));
					}
					std::string const& get_bytes() const {
						return std::get<std::string>(value_);
					}
					void set_bytes(const std::string& v) {
						value_.emplace<std::string>(v);
					}
					void set_bytes(const std::string&& v) {
						value_.emplace<std::string>(std::move(v));
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
				v.set_ArrayOfInt(std::move(vv));
			}
			break;
			case 2:
			{
				Energistics::Etp::v12::Datatypes::ArrayOfLong vv;
				avro::decode(d, vv);
				v.set_ArrayOfLong(std::move(vv));
			}
			break;
			case 3:
			{
				Energistics::Etp::v12::Datatypes::ArrayOfFloat vv;
				avro::decode(d, vv);
				v.set_ArrayOfFloat(std::move(vv));
			}
			break;
			case 4:
			{
				Energistics::Etp::v12::Datatypes::ArrayOfDouble vv;
				avro::decode(d, vv);
				v.set_ArrayOfDouble(std::move(vv));
			}
			break;
			case 5:
			{
				Energistics::Etp::v12::Datatypes::ArrayOfString vv;
				avro::decode(d, vv);
				v.set_ArrayOfString(std::move(vv));
			}
			break;
			case 6:
			{
				std::string vv;
				avro::decode(d, vv);
				v.set_bytes(std::move(vv));
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::AnyArray& v) {
			avro::decode(d, v.item);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::AnySubarray& v) {
			avro::decode(d, v.start);
			avro::decode(d, v.slice);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::AnySparseArray& v) {
			avro::decode(d, v.slices);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Contact& v) {
			avro::decode(d, v.organizationName);
			avro::decode(d, v.contactName);
			avro::decode(d, v.contactPhone);
			avro::decode(d, v.contactEmail);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class DataObjectCapabilityKind {
					ActiveTimeoutPeriod = 0,
					MaxContainedDataObjectCount = 1,
					MaxDataObjectSize = 2,
					OrphanedChildrenPrunedOnDelete = 3,
					SupportsGet = 4,
					SupportsPut = 5,
					SupportsDelete = 6,
					MaxSecondaryIndexCount = 7
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::DataObjectCapabilityKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::DataObjectCapabilityKind& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::DataObjectCapabilityKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::DataObjectCapabilityKind>(d.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct DataValueitem_t {
				private:
					std::variant<
						std::nullptr_t,
						bool,
						int32_t,
						int64_t,
						float,
						double,
						std::string,
						Energistics::Etp::v12::Datatypes::ArrayOfBoolean,
						Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean,
						Energistics::Etp::v12::Datatypes::ArrayOfInt,
						Energistics::Etp::v12::Datatypes::ArrayOfNullableInt,
						Energistics::Etp::v12::Datatypes::ArrayOfLong,
						Energistics::Etp::v12::Datatypes::ArrayOfNullableLong,
						Energistics::Etp::v12::Datatypes::ArrayOfFloat,
						Energistics::Etp::v12::Datatypes::ArrayOfDouble,
						Energistics::Etp::v12::Datatypes::ArrayOfString,
						Energistics::Etp::v12::Datatypes::ArrayOfBytes,
						std::vector<std::byte>,
						Energistics::Etp::v12::Datatypes::AnySparseArray
					> value_;

				public:
					size_t idx() const { return value_.index(); }
					bool is_null() const { return idx() == 0; }
					void set_null() { value_ = nullptr; }
					bool get_boolean() const {
						return std::get<bool>(value_);
					}
					void set_boolean(const bool& v) {
						value_ = v;
					}
					int32_t get_int() const {
						return std::get<int32_t>(value_);
					}
					void set_int(const int32_t& v) {
						value_ = v;
					}
					int64_t get_long() const {
						return std::get<int64_t>(value_);
					}
					void set_long(const int64_t& v) {
						value_ = v;
					}
					float get_float() const {
						return std::get<float>(value_);
					}
					void set_float(const float& v) {
						value_ = v;
					}
					double get_double() const {
						return std::get<double>(value_);
					}
					void set_double(const double& v) {
						value_ = v;
					}
					std::string const& get_string() const {
						return std::get<std::string>(value_);
					}
					void set_string(const std::string& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfBoolean const& get_ArrayOfBoolean() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfBoolean>(value_);
					}
					void set_ArrayOfBoolean(const Energistics::Etp::v12::Datatypes::ArrayOfBoolean& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean const& get_ArrayOfNullableBoolean() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean>(value_);
					}
					void set_ArrayOfNullableBoolean(const Energistics::Etp::v12::Datatypes::ArrayOfNullableBoolean& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfInt const& get_ArrayOfInt() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfInt>(value_);
					}
					void set_ArrayOfInt(const Energistics::Etp::v12::Datatypes::ArrayOfInt& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfNullableInt const& get_ArrayOfNullableInt() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfNullableInt>(value_);
					}
					void set_ArrayOfNullableInt(const Energistics::Etp::v12::Datatypes::ArrayOfNullableInt& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfLong const& get_ArrayOfLong() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfLong>(value_);
					}
					void set_ArrayOfLong(const Energistics::Etp::v12::Datatypes::ArrayOfLong& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfNullableLong const& get_ArrayOfNullableLong() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfNullableLong>(value_);
					}
					void set_ArrayOfNullableLong(const Energistics::Etp::v12::Datatypes::ArrayOfNullableLong& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfFloat const& get_ArrayOfFloat() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfFloat>(value_);
					}
					void set_ArrayOfFloat(const Energistics::Etp::v12::Datatypes::ArrayOfFloat& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfDouble const& get_ArrayOfDouble() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfDouble>(value_);
					}
					void set_ArrayOfDouble(const Energistics::Etp::v12::Datatypes::ArrayOfDouble& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfString const& get_ArrayOfString() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfString>(value_);
					}
					void set_ArrayOfString(const Energistics::Etp::v12::Datatypes::ArrayOfString& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ArrayOfBytes const& get_ArrayOfBytes() const {
						return std::get<Energistics::Etp::v12::Datatypes::ArrayOfBytes>(value_);
					}
					void set_ArrayOfBytes(const Energistics::Etp::v12::Datatypes::ArrayOfBytes& v) {
						value_ = v;
					}
					std::string get_bytes() const {
						const auto& bytes = std::get<std::vector<std::byte>>(value_);
						return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());

					}
					void set_bytes(const std::string& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::AnySparseArray const& get_AnySparseArray() const {
						return std::get<Energistics::Etp::v12::Datatypes::AnySparseArray>(value_);
					}
					void set_AnySparseArray(const Energistics::Etp::v12::Datatypes::AnySparseArray& v) {
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::DataValue& v) {
			avro::decode(d, v.item);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::DataAttribute& v) {
			avro::decode(d, v.attributeId);
			avro::decode(d, v.attributeValue);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class EndpointCapabilityKind {
					ActiveTimeoutPeriod = 0,
					AuthorizationDetails = 1,
					ChangePropagationPeriod = 2,
					ChangeRetentionPeriod = 3,
					MaxConcurrentMultipart = 4,
					MaxDataObjectSize = 5,
					MaxPartSize = 6,
					MaxSessionClientCount = 7,
					MaxSessionGlobalCount = 8,
					MaxWebSocketFramePayloadSize = 9,
					MaxWebSocketMessagePayloadSize = 10,
					MultipartMessageTimeoutPeriod = 11,
					ResponseTimeoutPeriod = 12,
					RequestSessionTimeoutPeriod = 13,
					SessionEstablishmentTimeoutPeriod = 14,
					SupportsAlternateRequestUris = 15,
					SupportsMessageHeaderExtensions = 16
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::EndpointCapabilityKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::EndpointCapabilityKind& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::EndpointCapabilityKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::EndpointCapabilityKind>(d.decodeEnum());
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ErrorInfo& v) {
			avro::decode(d, v.message);
			avro::decode(d, v.code);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct ProtocolException : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1000;

						ProtocolException() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Core);
							this->messageHeader.messageType = messageTypeId;
						}
						std::optional<Energistics::Etp::v12::Datatypes::ErrorInfo> error;
						bool has_error() const { return error.has_value(); }
						Energistics::Etp::v12::Datatypes::ErrorInfo get_error() const { return error.value(); }
						std::map<std::string, Energistics::Etp::v12::Datatypes::ErrorInfo> errors;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, error);
							avro::encode(e, errors);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, error);
							avro::decode(d, errors);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::MessageHeaderExtension& v) {
			avro::decode(d, v.extension);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				enum class ProtocolCapabilityKind {
					FrameChangeDetectionPeriod = 0,
					MaxDataArraySize = 1,
					MaxDataObjectSize = 2,
					MaxFrameResponseRowCount = 3,
					MaxIndexCount = 4,
					MaxRangeChannelCount = 5,
					MaxRangeDataItemCount = 6,
					MaxResponseCount = 7,
					MaxStreamingChannelsSessionCount = 8,
					MaxSubscriptionSessionCount = 9,
					MaxTransactionCount = 10,
					SupportsSecondaryIndexFiltering = 11,
					TransactionTimeoutPeriod = 12
				};
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ProtocolCapabilityKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ProtocolCapabilityKind& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ProtocolCapabilityKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::ProtocolCapabilityKind>(d.decodeEnum());
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::SupportedDataObject& v) {
			avro::decode(d, v.qualifiedType);
			avro::decode(d, v.dataObjectCapabilities);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct Uuid {
					std::array<uint8_t, 16> array{ {} };
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Uuid& v) {
			avro::decode(d, v.array);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct Chunk : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 9;

						Chunk() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Uuid blobId;
						std::string data;
						bool final = false;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, blobId);
							avro::encode(e, data);
							avro::encode(e, final);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, blobId);
							avro::decode(d, data);
							avro::decode(d, final);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct ObjectAccessRevoked : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 5;

						ObjectAccessRevoked() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						std::string uri;
						int64_t changeTime;
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, uri);
							avro::encode(e, changeTime);
							avro::encode(e, requestUuid);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, uri);
							avro::decode(d, changeTime);
							avro::decode(d, requestUuid);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct ObjectDeleted : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 3;

						ObjectDeleted() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						std::string uri;
						int64_t changeTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, uri);
							avro::encode(e, changeTime);
							avro::encode(e, requestUuid);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, uri);
							avro::decode(d, changeTime);
							avro::decode(d, requestUuid);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct SubscriptionEnded : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 7;

						SubscriptionEnded() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						std::string reason;
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, reason);
							avro::encode(e, requestUuid);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, reason);
							avro::decode(d, requestUuid);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct UnsubscribeNotifications : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 4;

						UnsubscribeNotifications() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, requestUuid);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, requestUuid);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct Chunk : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 8;

						Chunk() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Uuid blobId;
						std::string data;
						bool final = false;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, blobId);
							avro::encode(e, data);
							avro::encode(e, final);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, blobId);
							avro::decode(d, data);
							avro::decode(d, final);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreQuery {
					struct Chunk : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 3;

						Chunk() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Uuid blobId;
						std::string data;
						bool final = false;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, blobId);
							avro::encode(e, data);
							avro::encode(e, final);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, blobId);
							avro::decode(d, data);
							avro::decode(d, final);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct CommitTransaction : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 3;

						CommitTransaction() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, transactionUuid);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, transactionUuid);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct CommitTransactionResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 5;

						CommitTransactionResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						bool successful = false;
						std::string failureReason;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, transactionUuid);
							avro::encode(e, successful);
							avro::encode(e, failureReason);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, transactionUuid);
							avro::decode(d, successful);
							avro::decode(d, failureReason);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct RollbackTransaction : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 4;

						RollbackTransaction() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, transactionUuid);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, transactionUuid);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct RollbackTransactionResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 6;

						RollbackTransactionResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						bool successful = false;
						std::string failureReason;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, transactionUuid);
							avro::encode(e, successful);
							avro::encode(e, failureReason);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, transactionUuid);
							avro::decode(d, successful);
							avro::decode(d, failureReason);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Transaction {
					struct StartTransactionResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 2;

						StartTransactionResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Transaction);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Uuid transactionUuid;
						bool successful = false;
						std::string failureReason;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, transactionUuid);
							avro::encode(e, successful);
							avro::encode(e, failureReason);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, transactionUuid);
							avro::decode(d, successful);
							avro::decode(d, failureReason);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Version& v) {
			avro::decode(d, v.major);
			avro::decode(d, v.minor);
			avro::decode(d, v.revision);
			avro::decode(d, v.patch);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::SupportedProtocol& v) {
			avro::decode(d, v.protocol);
			avro::decode(d, v.protocolVersion);
			avro::decode(d, v.role);
			avro::decode(d, v.protocolCapabilities);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct OpenSession : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 2;

						OpenSession() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Core);
							this->messageHeader.messageType = messageTypeId;
						}
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

						void encode(avro::Encoder& e) const final {
							avro::encode(e, applicationName);
							avro::encode(e, applicationVersion);
							avro::encode(e, serverInstanceId);
							avro::encode(e, supportedProtocols);
							avro::encode(e, supportedDataObjects);
							avro::encode(e, supportedCompression);
							avro::encode(e, supportedFormats);
							avro::encode(e, currentDateTime);
							avro::encode(e, earliestRetainedChangeTime);
							avro::encode(e, sessionId);
							avro::encode(e, endpointCapabilities);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, applicationName);
							avro::decode(d, applicationVersion);
							avro::decode(d, serverInstanceId);
							avro::decode(d, supportedProtocols);
							avro::decode(d, supportedDataObjects);
							avro::decode(d, supportedCompression);
							avro::decode(d, supportedFormats);
							avro::decode(d, currentDateTime);
							avro::decode(d, earliestRetainedChangeTime);
							avro::decode(d, sessionId);
							avro::decode(d, endpointCapabilities);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Core {
					struct RequestSession : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1;

						RequestSession() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Core);
							this->messageHeader.messageType = messageTypeId;
						}
						std::string applicationName;
						std::string applicationVersion;
						Energistics::Etp::v12::Datatypes::Uuid clientInstanceId;
						std::vector<Energistics::Etp::v12::Datatypes::SupportedProtocol> requestedProtocols;
						std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> supportedDataObjects;
						std::vector<std::string> supportedCompression;
						std::vector<std::string> supportedFormats = { "xml" };
						int64_t currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						int64_t earliestRetainedChangeTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						bool serverAuthorizationRequired = false;
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataValue> endpointCapabilities;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, applicationName);
							avro::encode(e, applicationVersion);
							avro::encode(e, clientInstanceId);
							avro::encode(e, requestedProtocols);
							avro::encode(e, supportedDataObjects);
							avro::encode(e, supportedCompression);
							avro::encode(e, supportedFormats);
							avro::encode(e, currentDateTime);
							avro::encode(e, earliestRetainedChangeTime);
							avro::encode(e, serverAuthorizationRequired);
							avro::encode(e, endpointCapabilities);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, applicationName);
							avro::decode(d, applicationVersion);
							avro::decode(d, clientInstanceId);
							avro::decode(d, requestedProtocols);
							avro::decode(d, supportedDataObjects);
							avro::decode(d, supportedCompression);
							avro::decode(d, supportedFormats);
							avro::decode(d, currentDateTime);
							avro::decode(d, earliestRetainedChangeTime);
							avro::decode(d, serverAuthorizationRequired);
							avro::decode(d, endpointCapabilities);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ServerCapabilities& v) {
			avro::decode(d, v.applicationName);
			avro::decode(d, v.applicationVersion);
			avro::decode(d, v.contactInformation);
			avro::decode(d, v.supportedCompression);
			avro::decode(d, v.supportedEncodings);
			avro::decode(d, v.supportedFormats);
			avro::decode(d, v.supportedDataObjects);
			avro::decode(d, v.supportedProtocols);
			avro::decode(d, v.endpointCapabilities);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::ChannelChangeRequestInfo& v) {
			avro::decode(d, v.sinceChangeTime);
			avro::decode(d, v.channelIds);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					enum class ChannelDataKind {
						DateTime = 0,
						ElapsedTime = 1,
						MeasuredDepth = 2,
						PassIndexedDepth = 3,
						TrueVerticalDepth = 4,
						typeBoolean = 5,
						typeInt = 6,
						typeLong = 7,
						typeFloat = 8,
						typeDouble = 9,
						typeString = 10,
						typeBytes = 11
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::ChannelData::ChannelDataKind>(d.decodeEnum());
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::AttributeMetadataRecord& v) {
			avro::decode(d, v.attributeId);
			avro::decode(d, v.attributeName);
			avro::decode(d, v.dataKind);
			avro::decode(d, v.uom);
			avro::decode(d, v.depthDatum);
			avro::decode(d, v.attributePropertyKindUri);
			avro::decode(d, v.axisVectorLengths);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					enum class ChannelIndexKind {
						DateTime = 0,
						ElapsedTime = 1,
						MeasuredDepth = 2,
						TrueVerticalDepth = 3,
						PassIndexedDepth = 4,
						Pressure = 5,
						Temperature = 6,
						Scalar = 7
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::ChannelIndexKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::ChannelIndexKind& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::ChannelIndexKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::ChannelData::ChannelIndexKind>(d.decodeEnum());
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::FramePoint& v) {
			avro::decode(d, v.value);
			avro::decode(d, v.valueAttributes);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					enum class IndexDirection {
						Increasing = 0,
						Decreasing = 1,
						Unordered = 2
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::IndexDirection> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::IndexDirection& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::IndexDirection& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::ChannelData::IndexDirection>(d.decodeEnum());
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace ChannelData {
					enum class PassDirection {
						Up = 0,
						HoldingSteady = 1,
						Down = 2
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::ChannelData::PassDirection> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::ChannelData::PassDirection& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::PassDirection& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::ChannelData::PassDirection>(d.decodeEnum());
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth& v) {
			avro::decode(d, v.pass);
			avro::decode(d, v.direction);
			avro::decode(d, v.depth);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				struct IndexValueitem_t {
				private:
					std::variant<
						std::nullptr_t,
						int64_t,
						double,
						Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth
					> value_;

				public:
					size_t idx() const { return value_.index(); }
					bool is_null() const { return idx() == 0; }
					void set_null() { value_ = nullptr; }
					int64_t get_long() const {
						return std::get<int64_t>(value_);
					}
					void set_long(const int64_t& v) {
						value_ = v;
					}
					double get_double() const {
						return std::get<double>(value_);
					}
					void set_double(const double& v) {
						value_ = v;
					}
					Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth const& get_PassIndexedDepth() const {
						return std::get<Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth>(value_);
					}
					void set_PassIndexedDepth(const Energistics::Etp::v12::Datatypes::ChannelData::PassIndexedDepth& v) {
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::IndexValue& v) {
			avro::decode(d, v.item);
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
						bool dataChanges = false;
						std::optional<int32_t> requestLatestIndexCount;
						bool has_requestLatestIndexCount() const { return requestLatestIndexCount.has_value(); }
						int32_t get_requestLatestIndexCount() const { return requestLatestIndexCount.value(); }
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::ChannelSubscribeInfo& v) {
			avro::decode(d, v.channelId);
			avro::decode(d, v.startIndex);
			avro::decode(d, v.dataChanges);
			avro::decode(d, v.requestLatestIndexCount);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::DataItem& v) {
			avro::decode(d, v.channelId);
			avro::decode(d, v.indexes);
			avro::decode(d, v.value);
			avro::decode(d, v.valueAttributes);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::FrameRow& v) {
			avro::decode(d, v.indexes);
			avro::decode(d, v.points);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::TruncateInfo& v) {
			avro::decode(d, v.channelId);
			avro::decode(d, v.newEndIndex);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray& v) {
			avro::decode(d, v.dimensions);
			avro::decode(d, v.data);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataArraysResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1;

						GetDataArraysResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray> dataArrays;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataArrays);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataArrays);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataSubarraysResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 8;

						GetDataSubarraysResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray> dataSubarrays;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataSubarrays);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataSubarrays);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier& v) {
			avro::decode(d, v.uri);
			avro::decode(d, v.pathInResource);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataArrayMetadata : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 6;

						GetDataArrayMetadata() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier> dataArrays;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataArrays);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataArrays);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataArrays : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 2;

						GetDataArrays() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier> dataArrays;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataArrays);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataArrays);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::DataArrayTypes::GetDataSubarraysType& v) {
			avro::decode(d, v.uid);
			avro::decode(d, v.starts);
			avro::decode(d, v.counts);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataSubarrays : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 3;

						GetDataSubarrays() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::GetDataSubarraysType> dataSubarrays;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataSubarrays);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataSubarrays);
						}

						std::string body_to_string() const final {
							std::ostringstream oss;
							oss << "*************************************************\n"
								<< "Message Body put in the queue :\n";
							for (auto keyVal : dataSubarrays) {
								oss << "Key : " << keyVal.first << " Value : Starts ";
								for (auto dim : keyVal.second.starts) {
									oss << dim << " ";
								}
								oss << "Counts ";
								for (auto dim : keyVal.second.counts) {
									oss << dim << " ";
								}
								oss << "\n" << "URI " << keyVal.second.uid.uri << " Path " << keyVal.second.uid.pathInResource << "\n";
							}
							oss << "*************************************************";
							return oss.str();
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata& v) {
			avro::decode(d, v.dimensions);
			avro::decode(d, v.preferredSubarrayDimensions);
			avro::decode(d, v.transportArrayType);
			avro::decode(d, v.logicalArrayType);
			avro::decode(d, v.storeLastWrite);
			avro::decode(d, v.storeCreated);
			avro::decode(d, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct GetDataArrayMetadataResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 7;

						GetDataArrayMetadataResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata> arrayMetadata;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, arrayMetadata);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, arrayMetadata);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataArraysType& v) {
			avro::decode(d, v.uid);
			avro::decode(d, v.array);
			avro::decode(d, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutDataArrays : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 4;

						PutDataArrays() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataArraysType> dataArrays;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataArrays);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataArrays);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataSubarraysType& v) {
			avro::decode(d, v.uid);
			avro::decode(d, v.data);
			avro::decode(d, v.starts);
			avro::decode(d, v.counts);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutDataSubarrays : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 5;

						PutDataSubarrays() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutDataSubarraysType> dataSubarrays;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataSubarrays);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataSubarrays);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutUninitializedDataArrayType& v) {
			avro::decode(d, v.uid);
			avro::decode(d, v.metadata);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataArray {
					struct PutUninitializedDataArrays : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 9;

						PutUninitializedDataArrays() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataArray);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::DataArrayTypes::PutUninitializedDataArrayType> dataArrays;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataArrays);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataArrays);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					enum class ActiveStatusKind {
						Active = 0,
						Inactive = 1
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind>(d.decodeEnum());
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::FrameChannelMetadataRecord& v) {
			avro::decode(d, v.uri);
			avro::decode(d, v.channelName);
			avro::decode(d, v.dataKind);
			avro::decode(d, v.uom);
			avro::decode(d, v.depthDatum);
			avro::decode(d, v.channelPropertyKindUri);
			avro::decode(d, v.status);
			avro::decode(d, v.source);
			avro::decode(d, v.axisVectorLengths);
			avro::decode(d, v.attributeMetadata);
			avro::decode(d, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					enum class ContextScopeKind {
						self = 0,
						sources = 1,
						targets = 2,
						sourcesOrSelf = 3,
						targetsOrSelf = 4
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::ContextScopeKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::ContextScopeKind& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::ContextScopeKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::Object::ContextScopeKind>(d.decodeEnum());
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::Dataspace& v) {
			avro::decode(d, v.uri);
			avro::decode(d, v.path);
			avro::decode(d, v.storeLastWrite);
			avro::decode(d, v.storeCreated);
			avro::decode(d, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct GetDataspacesResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 2;

						GetDataspacesResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
							this->messageHeader.messageType = messageTypeId;
						}
						std::vector<Energistics::Etp::v12::Datatypes::Object::Dataspace> dataspaces;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataspaces);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataspaces);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Dataspace {
					struct PutDataspaces : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 3;

						PutDataspaces() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Dataspace);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::Dataspace> dataspaces;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataspaces);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataspaces);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace CoreOSDU {
					struct ResumeSession : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1;

						ResumeSession() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::CoreOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::string applicationName;
						std::string applicationVersion;
						Energistics::Etp::v12::Datatypes::Uuid clientInstanceId;
						int64_t currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						Energistics::Etp::v12::Datatypes::Uuid sessionId;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, applicationName);
							avro::encode(e, applicationVersion);
							avro::encode(e, clientInstanceId);
							avro::encode(e, currentDateTime);
							avro::encode(e, sessionId);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, applicationName);
							avro::decode(d, applicationVersion);
							avro::decode(d, clientInstanceId);
							avro::decode(d, currentDateTime);
							avro::decode(d, sessionId);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace CoreOSDU {
					struct ResumeSessionResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 2;

						ResumeSessionResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::CoreOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::string applicationName;
						std::string applicationVersion;
						Energistics::Etp::v12::Datatypes::Uuid serverInstanceId;
						int64_t currentDateTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
						Energistics::Etp::v12::Datatypes::Uuid sessionId;
						int64_t earliestRetainedChangeTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

						void encode(avro::Encoder& e) const final {
							avro::encode(e, applicationName);
							avro::encode(e, applicationVersion);
							avro::encode(e, serverInstanceId);
							avro::encode(e, currentDateTime);
							avro::encode(e, sessionId);
							avro::encode(e, earliestRetainedChangeTime);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, applicationName);
							avro::decode(d, applicationVersion);
							avro::decode(d, serverInstanceId);
							avro::decode(d, currentDateTime);
							avro::decode(d, sessionId);
							avro::decode(d, earliestRetainedChangeTime);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreOSDU {
					struct CopyDataObjectsByValue : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1;

						CopyDataObjectsByValue() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::string uri;
						int32_t sourcesDepth = 0;
						std::vector<std::string> dataObjectTypes;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, uri);
							avro::encode(e, sourcesDepth);
							avro::encode(e, dataObjectTypes);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, uri);
							avro::decode(d, sourcesDepth);
							avro::decode(d, dataObjectTypes);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreOSDU {
					struct CopyDataObjectsByValueResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 2;

						CopyDataObjectsByValueResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::vector<std::string> copiedDataObjects;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, copiedDataObjects);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, copiedDataObjects);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataspaceOSDU {
					struct GetDataspaceInfo : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1;

						GetDataspaceInfo() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataspaceOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> uris;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, uris);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, uris);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataspaceOSDU {
					struct GetDataspaceInfoResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 2;

						GetDataspaceInfoResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataspaceOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::Dataspace> dataspaces;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataspaces);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataspaces);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataspaceOSDU {
					struct CopyDataspacesContent : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 3;

						CopyDataspacesContent() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataspaceOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> dataspaces;
						std::string targetDataspace;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataspaces);
							avro::encode(e, targetDataspace);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataspaces);
							avro::decode(d, targetDataspace);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataspaceOSDU {
					struct CopyDataspacesContentResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 4;

						CopyDataspacesContentResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataspaceOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> success;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, success);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, success);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataspaceOSDU {
					struct LockDataspaces : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 5;

						LockDataspaces() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataspaceOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> uris;
						bool lock;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, uris);
							avro::encode(e, lock);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, uris);
							avro::decode(d, lock);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataspaceOSDU {
					struct LockDataspacesResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 6;

						LockDataspacesResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataspaceOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> success;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, success);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, success);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataspaceOSDU {
					struct CopyToDataspace : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 7;

						CopyToDataspace() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataspaceOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> uris;
						std::string dataspaceUri;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, uris);
							avro::encode(e, dataspaceUri);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, uris);
							avro::decode(d, dataspaceUri);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DataspaceOSDU {
					struct CopyToDataspaceResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 8;

						CopyToDataspaceResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DataspaceOSDU);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, std::string> success;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, success);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, success);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::DeletedResource& v) {
			avro::decode(d, v.uri);
			avro::decode(d, v.deletedTime);
			avro::decode(d, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Discovery {
					struct GetDeletedResourcesResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 6;

						GetDeletedResourcesResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
							this->messageHeader.messageType = messageTypeId;
						}
						std::vector<Energistics::Etp::v12::Datatypes::Object::DeletedResource> deletedResources;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, deletedResources);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, deletedResources);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::IndexInterval& v) {
			avro::decode(d, v.startIndex);
			avro::decode(d, v.endIndex);
			avro::decode(d, v.uom);
			avro::decode(d, v.depthDatum);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::ChannelRangeInfo& v) {
			avro::decode(d, v.channelIds);
			avro::decode(d, v.interval);
			avro::decode(d, v.secondaryIntervals);
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
						bool filterable = false;
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::IndexMetadataRecord& v) {
			avro::decode(d, v.indexKind);
			avro::decode(d, v.interval);
			avro::decode(d, v.direction);
			avro::decode(d, v.name);
			avro::decode(d, v.uom);
			avro::decode(d, v.depthDatum);
			avro::decode(d, v.indexPropertyKindUri);
			avro::decode(d, v.filterable);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::ChannelMetadataRecord& v) {
			avro::decode(d, v.uri);
			avro::decode(d, v.id);
			avro::decode(d, v.indexes);
			avro::decode(d, v.channelName);
			avro::decode(d, v.dataKind);
			avro::decode(d, v.uom);
			avro::decode(d, v.depthDatum);
			avro::decode(d, v.channelClassUri);
			avro::decode(d, v.status);
			avro::decode(d, v.source);
			avro::decode(d, v.axisVectorLengths);
			avro::decode(d, v.attributeMetadata);
			avro::decode(d, v.customData);
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
						bool preferRealtime = false;
						bool dataChanges = false;
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::ChannelData::OpenChannelInfo& v) {
			avro::decode(d, v.metadata);
			avro::decode(d, v.preferRealtime);
			avro::decode(d, v.dataChanges);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::ChangeAnnotation& v) {
			avro::decode(d, v.changeTime);
			avro::decode(d, v.interval);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::ChangeResponseInfo& v) {
			avro::decode(d, v.responseTimestamp);
			avro::decode(d, v.changes);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					enum class ObjectChangeKind {
						insert = 0,
						update = 1,
						authorized = 2,
						joined = 3,
						unjoined = 4,
						joinedSubscription = 5,
						unjoinedSubscription = 6
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::Object::ObjectChangeKind>(d.decodeEnum());
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::ObjectPart& v) {
			avro::decode(d, v.uid);
			avro::decode(d, v.data);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::PartsMetadataInfo& v) {
			avro::decode(d, v.uri);
			avro::decode(d, v.name);
			avro::decode(d, v.index);
			avro::decode(d, v.customData);
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::PutResponse& v) {
			avro::decode(d, v.createdContainedObjectUris);
			avro::decode(d, v.deletedContainedObjectUris);
			avro::decode(d, v.joinedContainedObjectUris);
			avro::decode(d, v.unjoinedContainedObjectUris);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct PutDataObjectsResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 9;

						PutDataObjectsResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Store);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::PutResponse> success;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, success);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, success);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					enum class RelationshipKind {
						Primary = 0,
						Secondary = 1,
						Both = 2
					};
				}
			}
		}
	}
}
namespace avro {
	template<> struct codec_traits<Energistics::Etp::v12::Datatypes::Object::RelationshipKind> {
		static void encode(Encoder& e, const Energistics::Etp::v12::Datatypes::Object::RelationshipKind& v) {
			e.encodeEnum(static_cast<std::size_t>(v));
		}
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::RelationshipKind& v) {
			v = static_cast<Energistics::Etp::v12::Datatypes::Object::RelationshipKind>(d.decodeEnum());
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
						bool includeSecondaryTargets = false;
						bool includeSecondarySources = false;
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::ContextInfo& v) {
			avro::decode(d, v.uri);
			avro::decode(d, v.depth);
			avro::decode(d, v.dataObjectTypes);
			avro::decode(d, v.navigableEdges);
			avro::decode(d, v.includeSecondaryTargets);
			avro::decode(d, v.includeSecondarySources);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Discovery {
					struct GetResources : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1;

						GetResources() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Object::ContextInfo context;
						Energistics::Etp::v12::Datatypes::Object::ContextScopeKind scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets;
						bool countObjects = false;
						std::optional<int64_t> storeLastWriteFilter;
						bool has_storeLastWriteFilter() const { return storeLastWriteFilter.has_value(); }
						int64_t get_storeLastWriteFilter() const { return storeLastWriteFilter.value(); }
						std::optional<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind> activeStatusFilter;
						bool has_activeStatusFilter() const { return activeStatusFilter.has_value(); }
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind get_activeStatusFilter() const { return activeStatusFilter.value(); }
						bool includeEdges = false;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, context);
							avro::encode(e, scope);
							avro::encode(e, countObjects);
							avro::encode(e, storeLastWriteFilter);
							avro::encode(e, activeStatusFilter);
							avro::encode(e, includeEdges);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, context);
							avro::decode(d, scope);
							avro::decode(d, countObjects);
							avro::decode(d, storeLastWriteFilter);
							avro::decode(d, activeStatusFilter);
							avro::decode(d, includeEdges);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DiscoveryQuery {
					struct FindResources : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1;

						FindResources() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DiscoveryQuery);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Object::ContextInfo context;
						Energistics::Etp::v12::Datatypes::Object::ContextScopeKind scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets;
						std::optional<int64_t> storeLastWriteFilter;
						bool has_storeLastWriteFilter() const { return storeLastWriteFilter.has_value(); }
						int64_t get_storeLastWriteFilter() const { return storeLastWriteFilter.value(); }
						std::optional<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind> activeStatusFilter;
						bool has_activeStatusFilter() const { return activeStatusFilter.has_value(); }
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind get_activeStatusFilter() const { return activeStatusFilter.value(); }

						void encode(avro::Encoder& e) const final {
							avro::encode(e, context);
							avro::encode(e, scope);
							avro::encode(e, storeLastWriteFilter);
							avro::encode(e, activeStatusFilter);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, context);
							avro::decode(d, scope);
							avro::decode(d, storeLastWriteFilter);
							avro::decode(d, activeStatusFilter);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreQuery {
					struct FindDataObjects : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 1;

						FindDataObjects() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreQuery);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Object::ContextInfo context;
						Energistics::Etp::v12::Datatypes::Object::ContextScopeKind scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets;
						std::optional<int64_t> storeLastWriteFilter;
						bool has_storeLastWriteFilter() const { return storeLastWriteFilter.has_value(); }
						int64_t get_storeLastWriteFilter() const { return storeLastWriteFilter.value(); }
						std::optional<Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind> activeStatusFilter;
						bool has_activeStatusFilter() const { return activeStatusFilter.has_value(); }
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind get_activeStatusFilter() const { return activeStatusFilter.value(); }
						std::string format;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, context);
							avro::encode(e, scope);
							avro::encode(e, storeLastWriteFilter);
							avro::encode(e, activeStatusFilter);
							avro::encode(e, format);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, context);
							avro::decode(d, scope);
							avro::decode(d, storeLastWriteFilter);
							avro::decode(d, activeStatusFilter);
							avro::decode(d, format);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::Edge& v) {
			avro::decode(d, v.sourceUri);
			avro::decode(d, v.targetUri);
			avro::decode(d, v.relationshipKind);
			avro::decode(d, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Discovery {
					struct GetResourcesEdgesResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 7;

						GetResourcesEdgesResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
							this->messageHeader.messageType = messageTypeId;
						}
						std::vector<Energistics::Etp::v12::Datatypes::Object::Edge> edges;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, edges);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, edges);
						}
					};
				}
			}
		}
	}
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
						std::optional<int32_t> sourceCount;
						bool has_sourceCount() const { return sourceCount.has_value(); }
						int32_t get_sourceCount() const { return sourceCount.value(); }
						std::optional<int32_t> targetCount;
						bool has_targetCount() const { return targetCount.has_value(); }
						int32_t get_targetCount() const { return targetCount.value(); }
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::Resource& v) {
			avro::decode(d, v.uri);
			avro::decode(d, v.alternateUris);
			avro::decode(d, v.name);
			avro::decode(d, v.sourceCount);
			avro::decode(d, v.targetCount);
			avro::decode(d, v.lastChanged);
			avro::decode(d, v.storeLastWrite);
			avro::decode(d, v.storeCreated);
			avro::decode(d, v.activeStatus);
			avro::decode(d, v.customData);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Discovery {
					struct GetResourcesResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 4;

						GetResourcesResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Discovery);
							this->messageHeader.messageType = messageTypeId;
						}
						std::vector<Energistics::Etp::v12::Datatypes::Object::Resource> resources;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, resources);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, resources);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace DiscoveryQuery {
					struct FindResourcesResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 2;

						FindResourcesResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::DiscoveryQuery);
							this->messageHeader.messageType = messageTypeId;
						}
						std::vector<Energistics::Etp::v12::Datatypes::Object::Resource> resources;
						std::string serverSortOrder;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, resources);
							avro::encode(e, serverSortOrder);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, resources);
							avro::decode(d, serverSortOrder);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct ObjectActiveStatusChanged : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 11;

						ObjectActiveStatusChanged() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind activeStatus = Energistics::Etp::v12::Datatypes::Object::ActiveStatusKind::Inactive;
						int64_t changeTime = 0;
						Energistics::Etp::v12::Datatypes::Object::Resource resource;
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, activeStatus);
							avro::encode(e, changeTime);
							avro::encode(e, resource);
							avro::encode(e, requestUuid);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, activeStatus);
							avro::decode(d, changeTime);
							avro::decode(d, resource);
							avro::decode(d, requestUuid);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct DataObject {
						Energistics::Etp::v12::Datatypes::Object::Resource resource;
						std::string format;
						std::optional<Energistics::Etp::v12::Datatypes::Uuid> blobId;
						bool has_blobId() const { return blobId.has_value(); }
						Energistics::Etp::v12::Datatypes::Uuid get_blobId() const { return blobId.value(); }
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::DataObject& v) {
			avro::decode(d, v.resource);
			avro::decode(d, v.format);
			avro::decode(d, v.blobId);
			avro::decode(d, v.data);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct GetDataObjectsResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 4;

						GetDataObjectsResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Store);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> dataObjects;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataObjects);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataObjects);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace Store {
					struct PutDataObjects : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 2;

						PutDataObjects() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::Store);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::DataObject> dataObjects;
						bool pruneContainedObjects = false;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataObjects);
							avro::encode(e, pruneContainedObjects);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataObjects);
							avro::decode(d, pruneContainedObjects);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreQuery {
					struct FindDataObjectsResponse : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 2;

						FindDataObjectsResponse() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreQuery);
							this->messageHeader.messageType = messageTypeId;
						}
						std::vector<Energistics::Etp::v12::Datatypes::Object::DataObject> dataObjects;
						std::string serverSortOrder;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, dataObjects);
							avro::encode(e, serverSortOrder);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, dataObjects);
							avro::decode(d, serverSortOrder);
						}
					};
				}
			}
		}
	}
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::ObjectChange& v) {
			avro::decode(d, v.changeKind);
			avro::decode(d, v.changeTime);
			avro::decode(d, v.dataObject);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct ObjectChanged : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 2;

						ObjectChanged() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						Energistics::Etp::v12::Datatypes::Object::ObjectChange change;
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, change);
							avro::encode(e, requestUuid);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, change);
							avro::decode(d, requestUuid);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct SubscriptionInfo {
						Energistics::Etp::v12::Datatypes::Object::ContextInfo context;
						Energistics::Etp::v12::Datatypes::Object::ContextScopeKind scope = Energistics::Etp::v12::Datatypes::Object::ContextScopeKind::targets;
						Energistics::Etp::v12::Datatypes::Uuid requestUuid;
						bool includeObjectData = false;
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::SubscriptionInfo& v) {
			avro::decode(d, v.context);
			avro::decode(d, v.scope);
			avro::decode(d, v.requestUuid);
			avro::decode(d, v.includeObjectData);
			avro::decode(d, v.format);
		}
	};
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct SubscribeNotifications : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 6;

						SubscribeNotifications() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						std::map<std::string, Energistics::Etp::v12::Datatypes::Object::SubscriptionInfo> request;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, request);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, request);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Protocol {
				namespace StoreNotification {
					struct UnsolicitedStoreNotifications : public ETP_NS::EtpMessage {
						static constexpr int32_t messageTypeId = 8;

						UnsolicitedStoreNotifications() {
							this->messageHeader.protocol = static_cast<std::underlying_type_t<Energistics::Etp::v12::Datatypes::Protocol>>(Energistics::Etp::v12::Datatypes::Protocol::StoreNotification);
							this->messageHeader.messageType = messageTypeId;
						}
						std::vector<Energistics::Etp::v12::Datatypes::Object::SubscriptionInfo> subscriptions;

						void encode(avro::Encoder& e) const final {
							avro::encode(e, subscriptions);
						}
						void decode(avro::Decoder& d) final {
							avro::decode(d, subscriptions);
						}
					};
				}
			}
		}
	}
}
namespace Energistics {
	namespace Etp {
		namespace v12 {
			namespace Datatypes {
				namespace Object {
					struct SupportedType {
						std::string dataObjectType;
						std::optional<int32_t> objectCount;
						bool has_objectCount() const { return objectCount.has_value(); }
						int32_t get_objectCount() const { return objectCount.value(); }
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
		static void decode(Decoder& d, Energistics::Etp::v12::Datatypes::Object::SupportedType& v) {
			avro::decode(d, v.dataObjectType);
			avro::decode(d, v.objectCount);
			avro::decode(d, v.relationshipKind);
		}
	};
}

namespace avro {
	/**
	 * codec_traits for Avro optional assumming that the schema is ["null", T].
	 */
	template<typename T>
	struct codec_traits<std::optional<T>> {
		/**
		 * Encodes a given value.
		 */
		static void encode(Encoder& e, const std::optional<T>& b) {
			if (b) {
				e.encodeUnionIndex(1);
				avro::encode(e, b.value());
			}
			else {
				e.encodeUnionIndex(0);
				e.encodeNull();
			}
		}

		/**
		 * Decodes into a given value.
		 */
		static void decode(Decoder& d, std::optional<T>& s) {
			size_t n = d.decodeUnionIndex();
			if (n >= 2) { throw avro::Exception("Union index too big for optional (expected 0 or 1, got " + std::to_string(n) + ")"); }
			switch (n) {
			case 0:
			{
				d.decodeNull();
				s.reset();
			}
			break;
			case 1:
			{
				s.emplace();
				avro::decode(d, *s);
			}
			break;
			}
		}
	};
}

#endif