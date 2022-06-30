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
#include  "DataArrayHandlers.h"

#include "../AbstractSession.h"
#include "../EtpHelpers.h"

using namespace ETP_NS;

void DataArrayHandlers::decodeMessageBody(const Energistics::Etp::v12::Datatypes::MessageHeader & mh, avro::DecoderPtr d)
{
	if (mh.protocol != static_cast<int32_t>(Energistics::Etp::v12::Datatypes::Protocol::DataArray)) {
		std::cerr << "Error : This message header does not belong to the protocol DataArray" << std::endl;
		return;
	}

	if (mh.messageType == Energistics::Etp::v12::Protocol::DataArray::GetDataArrays::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataArray::GetDataArrays gda;
		avro::decode(*d, gda);
		session->flushReceivingBuffer();
		on_GetDataArrays(gda, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse gdar;
		avro::decode(*d, gdar);
		session->flushReceivingBuffer();
		on_GetDataArraysResponse(gdar, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataArray::PutDataArrays::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataArray::PutDataArrays pda;
		avro::decode(*d, pda);
		session->flushReceivingBuffer();
		on_PutDataArrays(pda, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataArray::PutDataArraysResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataArray::PutDataArraysResponse msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_PutDataArraysResponse(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_GetDataSubarrays(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataArray::GetDataSubarraysResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataArray::GetDataSubarraysResponse msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_GetDataSubarraysResponse(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_PutDataSubarrays(msg, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataArray::PutDataSubarraysResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataArray::PutDataSubarraysResponse msg;
		avro::decode(*d, msg);
		session->flushReceivingBuffer();
		on_PutDataSubarraysResponse(msg, mh.correlationId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata gdam;
		avro::decode(*d, gdam);
		session->flushReceivingBuffer();
		on_GetDataArrayMetadata(gdam, mh.messageId);
	}
	else if (mh.messageType == Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadataResponse::messageTypeId) {
		Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadataResponse gdamr;
		avro::decode(*d, gdamr);
		session->flushReceivingBuffer();

		// Validation
		bool valid = true;
		for (const auto& element : gdamr.arrayMetadata) {
			const Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata& daMetadata = element.second;
			if (daMetadata.storeLastWrite < daMetadata.storeCreated) {
				session->send(EtpHelpers::buildSingleMessageProtocolException(5, "The data array storeLastWrite cannot be inferior to the data array storeCreated"), mh.messageId, 0x02);
				valid = false;
			}
			if (daMetadata.dimensions.empty()) {
				session->send(EtpHelpers::buildSingleMessageProtocolException(5, "The data array dimensions cannot be empty"), mh.messageId, 0x02);
				valid = false;
			}
			for (auto dimIndex = 0; dimIndex < daMetadata.dimensions.size(); ++dimIndex) {
				if (daMetadata.dimensions[dimIndex] <= 0) {
					session->send(EtpHelpers::buildSingleMessageProtocolException(5, "The data array dimension " + std::to_string(dimIndex) + " cannot be <= 0"), mh.messageId, 0x02);
					valid = false;
				}
			}
			if (!daMetadata.preferredSubarrayDimensions.empty() && daMetadata.preferredSubarrayDimensions.size() != daMetadata.dimensions.size()) {
				session->send(EtpHelpers::buildSingleMessageProtocolException(5, "The data array preferredSubarrayDimensions must be the same count as the data array dimensions"), mh.messageId, 0x02);
				valid = false;
			}
			for (auto dimIndex = 0; dimIndex < daMetadata.preferredSubarrayDimensions.size(); ++dimIndex) {
				if (daMetadata.preferredSubarrayDimensions[dimIndex] <= 0 || daMetadata.preferredSubarrayDimensions[dimIndex] > daMetadata.dimensions[dimIndex]) {
					session->send(EtpHelpers::buildSingleMessageProtocolException(5, "The data array preferredSubarrayDimension " + std::to_string(dimIndex) + " is <=0 or > to the corresponding data array dimension"), mh.messageId, 0x02);
					valid = false;
				}
			}
		}

		if (!valid) {
			return;
		}
		on_GetDataArrayMetadataResponse(gdamr, mh.correlationId);
	}
	else {
		session->flushReceivingBuffer();
		session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(3, "The message type ID " + std::to_string(mh.messageType) + " is invalid for the data array protocol."), mh.messageId, 0x02);
	}
}

void DataArrayHandlers::on_GetDataArrays(const Energistics::Etp::v12::Protocol::DataArray::GetDataArrays &, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataArrayHandlers::on_GetDataArrays method has not been overriden by the agent."), correlationId, 0x02);
}

void DataArrayHandlers::on_GetDataArraysResponse(const Energistics::Etp::v12::Protocol::DataArray::GetDataArraysResponse& msg, int64_t)
{
	for (const auto& element : msg.dataArrays) {
		const Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArray& da = element.second;
		std::cout << "*************************************************" << std::endl;
		std::cout << "Data Array received : " << std::endl;
		std::cout << "Dimension count : " << da.dimensions.size() << std::endl;
		for (auto i = 0; i < da.dimensions.size(); ++i) {
			std::cout << "Dimension " << i << " with count : " << da.dimensions[i] << std::endl;
		}
		std::cout << "*************************************************" << std::endl;
		if (da.data.item.idx() == 0) {
			Energistics::Etp::v12::Datatypes::ArrayOfBoolean avroArray = da.data.item.get_ArrayOfBoolean();
			auto values = avroArray.values;
			for (auto i = 0; i < values.size() && i < 20; ++i) {
				std::cout << "bool value " << i << " == " << values[i] << std::endl;
			}
		}
		else if (da.data.item.idx() == 1) {
			Energistics::Etp::v12::Datatypes::ArrayOfInt avroArray = da.data.item.get_ArrayOfInt();
			auto values = avroArray.values;
			for (auto i = 0; i < values.size() && i < 20; ++i) {
				std::cout << "int value " << i << " == " << values[i] << std::endl;
			}
		}
		else if (da.data.item.idx() == 2) {
			Energistics::Etp::v12::Datatypes::ArrayOfLong avroArray = da.data.item.get_ArrayOfLong();
			auto values = avroArray.values;
			for (auto i = 0; i < values.size() && i < 20; ++i) {
				std::cout << "long value " << i << " == " << values[i] << std::endl;
			}
		}
		else if (da.data.item.idx() == 3) {
			Energistics::Etp::v12::Datatypes::ArrayOfFloat avroArray = da.data.item.get_ArrayOfFloat();
			auto values = avroArray.values;
			for (auto i = 0; i < values.size() && i < 20; ++i) {
				std::cout << "float value " << i << " == " << values[i] << std::endl;
			}
		}
		else if (da.data.item.idx() == 4) {
			Energistics::Etp::v12::Datatypes::ArrayOfDouble avroArray = da.data.item.get_ArrayOfDouble();
			auto values = avroArray.values;
			for (auto i = 0; i < values.size() && i < 20; ++i) {
				std::cout << "double value " << i << " == " << values[i] << std::endl;
			}
		}
		else if (da.data.item.idx() == 5) {
			Energistics::Etp::v12::Datatypes::ArrayOfString avroArray = da.data.item.get_ArrayOfString();
			auto values = avroArray.values;
			for (auto i = 0; i < values.size() && i < 20; ++i) {
				std::cout << "string value " << i << " == " << values[i] << std::endl;
			}
		}
		else if (da.data.item.idx() == 6) {
			std::string values = da.data.item.get_bytes();
			for (auto i = 0; i < values.size() && i < 20; ++i) {
				std::cout << "char value " << i << " == " << (int)values[i] << std::endl;
			}
		}
		std::cout << "*************************************************" << std::endl;
	}
}

void DataArrayHandlers::on_PutDataArrays(const Energistics::Etp::v12::Protocol::DataArray::PutDataArrays&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataArrayHandlers::on_PutDataArrays method has not been overriden by the agent."), correlationId, 0x02);
}

void DataArrayHandlers::on_PutDataArraysResponse(const Energistics::Etp::v12::Protocol::DataArray::PutDataArraysResponse&, int64_t correlationId)
{
	std::cout << "Received PutDataArraysResponse to message id " << correlationId << std::endl;
}

void DataArrayHandlers::on_GetDataSubarrays(const Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataArrayHandlers::on_GetDataArraySlice method has not been overriden by the agent."), correlationId, 0x02);
}

void DataArrayHandlers::on_GetDataSubarraysResponse(const Energistics::Etp::v12::Protocol::DataArray::GetDataSubarraysResponse&, int64_t)
{
	std::cout << "on_GetDataArraySlicesResponse : not implemented yet" << std::endl;
}

void DataArrayHandlers::on_PutDataSubarrays(const Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataArrayHandlers::on_PutDataArraySlices method has not been overriden by the agent."), correlationId, 0x02);
}

void DataArrayHandlers::on_PutDataSubarraysResponse(const Energistics::Etp::v12::Protocol::DataArray::PutDataSubarraysResponse&, int64_t correlationId)
{
	std::cout << "Received PutDataSubarraysResponse to message id " << correlationId << std::endl;
}

void DataArrayHandlers::on_GetDataArrayMetadata(const Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataArrayHandlers::on_GetDataArrayMetadata method has not been overriden by the agent."), correlationId, 0x02);
}

void DataArrayHandlers::on_GetDataArrayMetadataResponse(const Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadataResponse& gdamr, int64_t)
{
	for (const auto& element : gdamr.arrayMetadata) {
		const Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata& daMetadata = element.second;

		std::cout << "*************************************************" << std::endl;
		std::cout << "Data Array Metadata received : " << std::endl;
		std::cout << "Array transport type : ";
		switch (daMetadata.transportArrayType) {
			case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfBoolean : std::cout << "arrayOfBoolean"; break;
			case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfInt: std::cout << "arrayOfInt"; break;
			case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfLong: std::cout << "arrayOfLong"; break;
			case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfFloat: std::cout << "arrayOfFloat"; break;
			case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfDouble: std::cout << "arrayOfDouble"; break;
			case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfString: std::cout << "arrayOfString"; break;
			case Energistics::Etp::v12::Datatypes::AnyArrayType::bytes: std::cout << "bytes"; break;
			default : std::cout << "unrecognized transportArrayType";
		}
		std::cout << "Array logical type : ";
		switch (daMetadata.logicalArrayType) {
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfBoolean : std::cout << "arrayOfBoolean"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfDouble64BE : std::cout << "arrayOfDouble64BE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfDouble64LE : std::cout << "arrayOfDouble64LE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfFloat32BE: std::cout << "arrayOfFloat32BE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfFloat32LE: std::cout << "arrayOfFloat32LE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfInt16BE: std::cout << "arrayOfInt16BE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfInt16LE: std::cout << "arrayOfInt16LE"; break;;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfInt32BE: std::cout << "arrayOfInt32BE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfInt32LE: std::cout << "arrayOfInt32LE"; break;;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfInt64BE: std::cout << "arrayOfInt64BE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfInt64LE: std::cout << "arrayOfInt64LE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfInt8: std::cout << "arrayOfInt8"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfString: std::cout << "arrayOfString"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfUInt16BE: std::cout << "arrayOfUInt16BE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfUInt16LE: std::cout << "arrayOfUInt16LE"; break;;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfUInt32BE: std::cout << "arrayOfUInt32BE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfUInt32LE: std::cout << "arrayOfUInt32LE"; break;;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfUInt64BE: std::cout << "arrayOfUInt64BE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfUInt64LE: std::cout << "arrayOfUInt64LE"; break;
			case Energistics::Etp::v12::Datatypes::AnyLogicalArrayType::arrayOfUInt8: std::cout << "arrayOfUInt8"; break;
			default: std::cout << "unrecognized logicalArrayType";
		}
		std::cout << std::endl;
		for (auto i = 0; i < daMetadata.dimensions.size(); ++i) {
			std::cout << "Dimension " << i << " with count : " << daMetadata.dimensions[i] << std::endl;
		}
		std::cout << "*************************************************" << std::endl;
	}
}

void DataArrayHandlers::on_PutUninitializedDataArrays(const Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArrays&, int64_t correlationId)
{
	session->send(ETP_NS::EtpHelpers::buildSingleMessageProtocolException(7, "The DataArrayHandlers::on_PutUninitializedDataArrays method has not been overriden by the agent."), correlationId, 0x02);
}

void DataArrayHandlers::on_PutUninitializedDataArraysResponse(const Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArraysResponse&, int64_t)
{
	std::cout << "Received PutUninitializedDataArraysResponse." << std::endl;
}
