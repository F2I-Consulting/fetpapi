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
#include "FesapiHdfProxy.h"

#include <stdexcept>

using namespace ETP_NS;
using namespace std;

Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier FesapiHdfProxy::buildDataArrayIdentifier(const std::string & datasetName) const
{
	Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier dai;
	dai.uri = buildEtp12Uri();
	dai.pathInResource = datasetName;

	return dai;
}

Energistics::Etp::v12::Protocol::DataArray::GetDataArrays FesapiHdfProxy::buildGetDataArraysMessage(const std::string & datasetName) const
{
	Energistics::Etp::v12::Protocol::DataArray::GetDataArrays msg;
	msg.dataArrays["0"] = buildDataArrayIdentifier(datasetName);

	return msg;
}

Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata FesapiHdfProxy::buildGetDataArrayMetadataMessage(const std::string & datasetName) const
{
	Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata msg;
	msg.dataArrays["0"] = buildDataArrayIdentifier(datasetName);

	return msg;
}

Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata FesapiHdfProxy::getDataArrayMetadata(const std::string & datasetName) const
{
	// We don't care about the template parameter in this particular case
	auto handlers = std::make_shared<GetFullDataArrayHandlers<int64_t>>(session_, nullptr);

	const int64_t msgId = session_->sendWithSpecificHandler(
		buildGetDataArrayMetadataMessage(datasetName),
		handlers,
		0, 0x02);

	// Blocking loop
	auto t_start = std::chrono::high_resolution_clock::now();
	// Use timeOut value for session.
	auto timeOut = session_->getTimeOut();
	while (session_->isMessageStillProcessing(msgId)) {
		if (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() > timeOut) {
			throw std::runtime_error("Time out waiting for a response of message id " + std::to_string(msgId));
		}
	}

	return handlers->getDataArrayMetadata();
}

COMMON_NS::AbstractObject::numericalDatatypeEnum  FesapiHdfProxy::getNumericalDatatype(const std::string & datasetName)
{
	const auto daMetadata = getDataArrayMetadata(datasetName);
	switch (daMetadata.transportArrayType) {
	case Energistics::Etp::v12::Datatypes::AnyArrayType::bytes:
	case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfBoolean: return COMMON_NS::AbstractObject::numericalDatatypeEnum::INT8;
	case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfInt: return COMMON_NS::AbstractObject::numericalDatatypeEnum::INT32;
	case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfLong: return COMMON_NS::AbstractObject::numericalDatatypeEnum::INT64;
	case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfFloat: return COMMON_NS::AbstractObject::numericalDatatypeEnum::FLOAT;
	case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfDouble: return COMMON_NS::AbstractObject::numericalDatatypeEnum::DOUBLE;
	default: return COMMON_NS::AbstractObject::numericalDatatypeEnum::UNKNOWN;
	}
}

int FesapiHdfProxy::getHdfDatatypeClassInDataset(const std::string&)
{
	// Hard to implement because it would need to include H5TPublic.h which could create a dependency on HDF5 which we try to avoid...
	throw logic_error("getHdfDatatypeClassInDataset Not implemented yet");
}

void FesapiHdfProxy::writeItemizedListOfList(const string & groupName, const std::string & name,
	COMMON_NS::AbstractObject::numericalDatatypeEnum cumulativeLengthDatatype,
	const void * cumulativeLength,
	uint64_t cumulativeLengthSize,
	COMMON_NS::AbstractObject::numericalDatatypeEnum elementsDatatype,
	const void * elements,
	uint64_t elementsSize)
{
	writeArrayNd(groupName + '/' + name, CUMULATIVE_LENGTH_DS_NAME, cumulativeLengthDatatype, cumulativeLength, &cumulativeLengthSize, 1);
	writeArrayNd(groupName + '/' + name, ELEMENTS_DS_NAME, elementsDatatype, elements, &elementsSize, 1);
}

std::vector<uint32_t> FesapiHdfProxy::getElementCountPerDimension(const std::string & datasetName)
{
	std::vector<uint32_t> result;

	const auto daMetadata = getDataArrayMetadata(datasetName);
	for (auto dim : daMetadata.dimensions) {
		result.push_back(dim);
	}

	return result;
}

void FesapiHdfProxy::writeArrayNd(const std::string & groupName,
	const std::string & name,
	COMMON_NS::AbstractObject::numericalDatatypeEnum datatype,
	const void * values,
	const uint64_t * numValuesInEachDimension,
	unsigned int numDimensions)
{
	if (!isOpened())
		open();

	// URI AND PATH
	std::string uri{ buildEtp12Uri() };

	std::string pathInResource{ (groupName.back() == '/' ? 
		groupName : groupName + '/') + name };

	// Create Dimensions and Total Count
	size_t totalCount{ 1 };
	std::vector<int64_t> dimensions{};

	for (size_t i = 0; i < numDimensions; ++i)
	{
		dimensions.push_back(numValuesInEachDimension[i]);
		totalCount *= numValuesInEachDimension[i];
	}

	// Determine Value Size (bytes)
	int valueSize{ 1 };

	if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::DOUBLE)
	{
		valueSize = sizeof(double);
	}
	else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::FLOAT)
	{
		valueSize = sizeof(float);
	}
	else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT64 ||
		datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT64)
	{
		valueSize = sizeof(int64_t);
	}
	else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT32 ||
		datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT32)
	{
		valueSize = sizeof(int32_t);
	}
	else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT16 ||
		datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT16)
	{
		valueSize = sizeof(short);
	}
	else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT8 ||
		datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT8)
	{
		valueSize = sizeof(char);
	}
	else
	{
		throw logic_error(
			"You need to give a COMMON_NS::AbstractObject::numericalDatatypeEnum as the datatype");
	}

	if (totalCount * valueSize <= maxArraySize_)
	{
		// PUT DATA ARRAYS
		Energistics::Etp::v12::Protocol::DataArray::PutDataArrays pda{};
		pda.dataArrays["0"].uid.uri = uri;
		pda.dataArrays["0"].uid.pathInResource = pathInResource;
		pda.dataArrays["0"].array.dimensions = dimensions;

		// Create AVRO Array
		Energistics::Etp::v12::Datatypes::AnyArray data;
		if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::DOUBLE) 
		{
			Energistics::Etp::v12::Datatypes::ArrayOfDouble avroArray;

			avroArray.values = std::vector<double>(
				static_cast<const double*>(values), 
				static_cast<const double*>(values) + totalCount);

			data.item.set_ArrayOfDouble(avroArray);
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::FLOAT) 
		{
			Energistics::Etp::v12::Datatypes::ArrayOfFloat avroArray;

			avroArray.values = std::vector<float>(
				static_cast<const float*>(values), 
				static_cast<const float*>(values) + totalCount);

			data.item.set_ArrayOfFloat(avroArray);
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT64 || 
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT64) {
			Energistics::Etp::v12::Datatypes::ArrayOfLong avroArray;

			avroArray.values = std::vector<int64_t>(
				static_cast<const int64_t*>(values), 
				static_cast<const int64_t*>(values) + totalCount);

			data.item.set_ArrayOfLong(avroArray);
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT32 || 
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT32) {
			Energistics::Etp::v12::Datatypes::ArrayOfInt avroArray;

			avroArray.values = std::vector<int32_t>(
				static_cast<const int32_t*>(values), 
				static_cast<const int32_t*>(values) + totalCount);

			data.item.set_ArrayOfInt(avroArray);
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT16 || 
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT16) {
			Energistics::Etp::v12::Datatypes::ArrayOfInt avroArray;

			for (size_t i = 0; i < totalCount; ++i) 
			{
				avroArray.values.push_back(static_cast<const short*>(values)[i]);
			}

			data.item.set_ArrayOfInt(avroArray);
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT8 || 
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT8) {
			std::string avroArray;

			for (size_t i = 0; i < totalCount; ++i)
			{
				avroArray.push_back(static_cast<const char*>(values)[i]);
			}

			data.item.set_bytes(avroArray);
		}
		else {
			throw logic_error(
				"You need to give a COMMON_NS::AbstractObject::numericalDatatypeEnum as the datatype");
		}

		pda.dataArrays["0"].array.data = data;

		// Send Data Arrays
		session_->send(pda, 0, 0x02);
	}
	else 
	{
		// PUT UNINITIALIZED DATA ARRAYS
		Energistics::Etp::v12::Protocol::DataArray::PutUninitializedDataArrays puda;
		puda.dataArrays["0"].uid.uri = uri;
		puda.dataArrays["0"].uid.pathInResource = pathInResource;
		puda.dataArrays["0"].metadata.dimensions = dimensions;

		// Assign Data Type
		if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::DOUBLE)
		{
			puda.dataArrays["0"].metadata.transportArrayType =
				Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfDouble;
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::FLOAT)
		{
			puda.dataArrays["0"].metadata.transportArrayType =
				Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfFloat;
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT64 ||
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT64)
		{
			puda.dataArrays["0"].metadata.transportArrayType =
				Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfLong;
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT32 ||
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT32)
		{
			puda.dataArrays["0"].metadata.transportArrayType =
				Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfInt;
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT16 ||
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT16)
		{
			puda.dataArrays["0"].metadata.transportArrayType =
				Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfInt;
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT8 ||
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT8)
		{
			puda.dataArrays["0"].metadata.transportArrayType =
				Energistics::Etp::v12::Datatypes::AnyArrayType::bytes;
		}
		else
		{
			throw logic_error(
				"You need to give a COMMON_NS::AbstractObject::numericalDatatypeEnum as the datatype");
		}

		// Send Uninitialized Data Arrays
		session_->sendAndBlock(puda, 0, 0x02);

		// SEND MULTIPLE PUT DATA SUBARRAYS MESSAGES
		std::cout << "Writing Subarrays: This may take some time." << std::endl;
		std::cout << "Please wait..." << std::endl;

		// Initial Starts and Counts
		std::vector<int64_t> starts{};
		std::vector<int64_t> counts{};

		for (size_t i = 0; i < numDimensions; ++i) {
			starts.push_back(0);
			counts.push_back(numValuesInEachDimension[i]);
		}

		// Recursively Write Subarrays
		if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::DOUBLE) {
			writeSubArrayNd<double>(uri, pathInResource, counts,
				starts,
				counts,
				values);
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::FLOAT) {
			writeSubArrayNd<float>(uri, pathInResource, counts,
				starts,
				counts,
				values);
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT64 || 
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT64) {
			writeSubArrayNd<int64_t>(uri, pathInResource, counts,
				starts,
				counts,
				values);
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT32 || 
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT32) {
			writeSubArrayNd<int32_t>(uri, pathInResource, counts,
				starts,
				counts,
				values);
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT16 || 
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT16) {
			writeSubArrayNd<short>(
				uri, pathInResource, counts,
				starts,
				counts,
				values);
		}
		else if (datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::INT8 || 
			datatype == COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT8) {
			writeSubArrayNd<char>(
				uri, pathInResource, counts,
				starts,
				counts,
				values);
		}
		else {
			throw logic_error(
				"You need to give a COMMON_NS::AbstractObject::numericalDatatypeEnum as the datatype");
		}
	}
}

void FesapiHdfProxy::createArrayNd(
	const std::string& groupName,
	const std::string& datasetName,
	COMMON_NS::AbstractObject::numericalDatatypeEnum datatype,
	const uint64_t* numValuesInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("createArrayNdNot implemented yet");
}

void FesapiHdfProxy::writeArrayNdSlab(
	const string& groupName,
	const string& datasetName,
	COMMON_NS::AbstractObject::numericalDatatypeEnum datatype,
	const void* values,
	const uint64_t* numValuesInEachDimension,
	const uint64_t* offsetInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("writeArrayNdSlab Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfDoubleValues(
	const std::string & datasetName, double* values,
	uint64_t const * numValuesInEachDimension,
	uint64_t const * offsetInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("readArrayNdOfDoubleValues Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfDoubleValues(
	const std::string & datasetName, double* values,
	uint64_t const * blockCountPerDimension,
	uint64_t const * offsetInEachDimension,
	uint64_t const * strideInEachDimension,
	uint64_t const * blockSizeInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("readArrayNdOfDoubleValues Not implemented yet");
}

void FesapiHdfProxy::selectArrayNdOfValues(
	const std::string & datasetName,
	uint64_t const * blockCountPerDimension,
	uint64_t const * offsetInEachDimension,
	uint64_t const * strideInEachDimension,
	uint64_t const * blockSizeInEachDimension,
	unsigned int numDimensions,
	bool newSelection,
	hdf5_hid_t & dataset,
	hdf5_hid_t & filespace)
{
	throw logic_error("selectArrayNdOfValues Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfDoubleValues(
	hdf5_hid_t dataset,
	hdf5_hid_t filespace,
	void* values,
	uint64_t slabSize)
{
	throw logic_error("readArrayNdOfDoubleValues Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfFloatValues(
	const std::string& datasetName, float* values,
	uint64_t const * numValuesInEachDimension,
	uint64_t const * offsetInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("readArrayNdOfFloatValues Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfInt64Values(
	const std::string& datasetName, int64_t* values,
	uint64_t const * numValuesInEachDimension,
	uint64_t const * offsetInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("readArrayNdOfInt64Values Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfIntValues(
	const std::string& datasetName, int* values,
	uint64_t const * numValuesInEachDimension,
	uint64_t const * offsetInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("readArrayNdOfIntValues Not implemented yet");
}

bool FesapiHdfProxy::exist(const std::string & absolutePathInHdfFile) const
{
	throw logic_error("exist Not implemented yet");
}

bool FesapiHdfProxy::isCompressed(const std::string & datasetName)
{
	throw logic_error("isCompressed Not implemented yet");
}
