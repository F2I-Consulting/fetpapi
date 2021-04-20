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

#include "../ProtocolHandlers/BlockingDataArrayHandlers.h"

using namespace ETP_NS;
using namespace std;

std::string FesapiHdfProxy::getUri() const
{
	const std::string xmlNs = getXmlNamespace();
	return "eml:///" + (xmlNs == "eml20" ? xmlNs + ".obj_" : xmlNs + ".") + "EpcExternalPartReference(" + getUuid() + ")";
}

Energistics::Etp::v12::Protocol::DataArray::GetDataArrays FesapiHdfProxy::buildGetDataArraysMessage(const std::string & datasetName)
{
	Energistics::Etp::v12::Protocol::DataArray::GetDataArrays gda;
	Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier dai;
	dai.uri = getUri();
	dai.pathInResource = datasetName;
	gda.dataArrays["0"] = dai;

	return gda;
}

COMMON_NS::AbstractObject::hdfDatatypeEnum FesapiHdfProxy::getHdfDatatypeInDataset(const std::string & datasetName)
{
	throw logic_error("Not implemented yet");
}

int FesapiHdfProxy::getHdfDatatypeClassInDataset(const std::string & datasetName)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeItemizedListOfList(const string & groupName, const std::string & name,
	hdf5_hid_t cumulativeLengthDatatype,
	const void * cumulativeLength,
	unsigned long long cumulativeLengthSize,
	hdf5_hid_t elementsDatatype,
	const void * elements,
	unsigned long long elementsSize)
{
	throw logic_error("Not implemented yet");
}

unsigned int FesapiHdfProxy::getDimensionCount(const std::string & datasetName)
{
	throw logic_error("Not implemented yet");
}

std::vector<unsigned long long> FesapiHdfProxy::getElementCountPerDimension(const std::string & datasetName)
{
	throw logic_error("Not implemented yet");
}

signed long long FesapiHdfProxy::getElementCount(const std::string & datasetName)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeArrayNdOfFloatValues(const string & groupName,
	const string & name,
	const float * floatValues,
	const unsigned long long * numValuesInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeArrayNdOfDoubleValues(const string & groupName,
	const string & name,
	const double * dblValues,
	const unsigned long long * numValuesInEachDimension,
	unsigned int numDimensions)
{
	if (!isOpened()) {
		open();
	}

	Energistics::Etp::v12::Protocol::DataArray::PutDataArrays pda;
	pda.dataArrays["0"].uid.uri = getUri();
	pda.dataArrays["0"].uid.pathInResource = (groupName.back() == '/' ? groupName : groupName + '/') + name;

	size_t totalCount = 1;
	std::vector<int64_t> dimensions;
	for (size_t i = 0; i < numDimensions; ++i) {
		dimensions.push_back(numValuesInEachDimension[i]);
		totalCount *= numValuesInEachDimension[i];
	}
	pda.dataArrays["0"].array.dimensions = dimensions;

	Energistics::Etp::v12::Datatypes::AnyArray data;
	Energistics::Etp::v12::Datatypes::ArrayOfDouble arrayOfDbl;
	arrayOfDbl.values = std::vector<double>(dblValues, dblValues + totalCount);
	data.item.set_ArrayOfDouble(arrayOfDbl);
	pda.dataArrays["0"].array.data = data;

	session_->send(pda);
}

void FesapiHdfProxy::writeArrayNdOfCharValues(const std::string & groupName,
	const std::string & name,
	const char * intValues,
	const unsigned long long * numValuesInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeArrayNdOfIntValues(const string & groupName,
	const string & name,
	const int * intValues,
	const unsigned long long * numValuesInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeArrayNdOfInt64Values(const std::string & groupName,
	const std::string & name,
	const int64_t * values,
	const unsigned long long * numValuesInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeArrayNdOfUInt64Values(const std::string & groupName,
	const std::string & name,
	const uint64_t * values,
	const unsigned long long * numValuesInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeArrayNd(const std::string & groupName,
	const std::string & name,
	hdf5_hid_t datatype,
	const void * values,
	const unsigned long long * numValuesInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::createArrayNd(
	const std::string& groupName,
	const std::string& datasetName,
	hdf5_hid_t datatype,
	const unsigned long long* numValuesInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}


void FesapiHdfProxy::writeArrayNdSlab(
	const string& groupName,
	const string& datasetName,
	hdf5_hid_t datatype,
	const void* values,
	const unsigned long long* numValuesInEachDimension,
	const unsigned long long* offsetInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfDoubleValues(const std::string & datasetName, double* values)
{
	const int64_t msgId = session_->sendWithSpecificHandler(
		buildGetDataArraysMessage(datasetName),
		std::make_shared<BlockingDataArrayHandlers<double>>(session_, values),
		0, 0x02);

	// Blocking loop
	while (session_->isMessageStillProcessing(msgId)) {}
}

void FesapiHdfProxy::readArrayNdOfDoubleValues(
	const std::string & datasetName, double* values,
	unsigned long long const * numValuesInEachDimension,
	unsigned long long const * offsetInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfDoubleValues(
	const std::string & datasetName, double* values,
	unsigned long long const * blockCountPerDimension,
	unsigned long long const * offsetInEachDimension,
	unsigned long long const * strideInEachDimension,
	unsigned long long const * blockSizeInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::selectArrayNdOfValues(
	const std::string & datasetName,
	unsigned long long const * blockCountPerDimension,
	unsigned long long const * offsetInEachDimension,
	unsigned long long const * strideInEachDimension,
	unsigned long long const * blockSizeInEachDimension,
	unsigned int numDimensions,
	bool newSelection,
	hdf5_hid_t & dataset,
	hdf5_hid_t & filespace)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfDoubleValues(
	hdf5_hid_t dataset,
	hdf5_hid_t filespace,
	void* values,
	unsigned long long slabSize)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfFloatValues(const std::string & datasetName, float* values)
{
	const int64_t msgId = session_->sendWithSpecificHandler(
		buildGetDataArraysMessage(datasetName),
		std::make_shared<BlockingDataArrayHandlers<float>>(session_, values),
		0, 0x02);

	// Blocking loop
	while (session_->isMessageStillProcessing(msgId)) {}
}

void FesapiHdfProxy::readArrayNdOfFloatValues(
	const std::string& datasetName, float* values,
	unsigned long long const * numValuesInEachDimension,
	unsigned long long const * offsetInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfInt64Values(const std::string & datasetName, int64_t* values)
{
	const int64_t msgId = session_->sendWithSpecificHandler(
		buildGetDataArraysMessage(datasetName),
		std::make_shared<BlockingDataArrayHandlers<int64_t>>(session_, values),
		0, 0x02);

	// Blocking loop
	while (session_->isMessageStillProcessing(msgId)) {}
}

void FesapiHdfProxy::readArrayNdOfInt64Values(
	const std::string& datasetName, int64_t* values,
	unsigned long long const * numValuesInEachDimension,
	unsigned long long const * offsetInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfUInt64Values(const std::string & datasetName, uint64_t* values)
{
	const int64_t msgId = session_->sendWithSpecificHandler(
		buildGetDataArraysMessage(datasetName),
		std::make_shared<BlockingDataArrayHandlers<uint64_t>>(session_, values),
		0, 0x02);

	// Blocking loop
	while (session_->isMessageStillProcessing(msgId)) {}
}

void FesapiHdfProxy::readArrayNdOfIntValues(const std::string & datasetName, int* values)
{
	const int64_t msgId = session_->sendWithSpecificHandler(
		buildGetDataArraysMessage(datasetName),
		std::make_shared<BlockingDataArrayHandlers<int32_t>>(session_, values),
		0, 0x02);

	// Blocking loop
	while (session_->isMessageStillProcessing(msgId)) {}
}

void FesapiHdfProxy::readArrayNdOfIntValues(
	const std::string& datasetName, int* values,
	unsigned long long const * numValuesInEachDimension,
	unsigned long long const * offsetInEachDimension,
	unsigned int numDimensions)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::readArrayNdOfUIntValues(const std::string & datasetName, unsigned int* values)
{
	const int64_t msgId = session_->sendWithSpecificHandler(
		buildGetDataArraysMessage(datasetName),
		std::make_shared<BlockingDataArrayHandlers<uint32_t>>(session_, values),
		0, 0x02);

	// Blocking loop
	while (session_->isMessageStillProcessing(msgId)) {}
}

 void FesapiHdfProxy::readArrayNdOfShortValues(const std::string & datasetName, short* values)
{
	 const int64_t msgId = session_->sendWithSpecificHandler(
		 buildGetDataArraysMessage(datasetName),
		 std::make_shared<BlockingDataArrayHandlers<int16_t>>(session_, values),
		 0, 0x02);

	 // Blocking loop
	 while (session_->isMessageStillProcessing(msgId)) {}
}

void FesapiHdfProxy::readArrayNdOfUShortValues(const std::string & datasetName, unsigned short* values)
{
	const int64_t msgId = session_->sendWithSpecificHandler(
		buildGetDataArraysMessage(datasetName),
		std::make_shared<BlockingDataArrayHandlers<uint16_t>>(session_, values),
		0, 0x02);

	// Blocking loop
	while (session_->isMessageStillProcessing(msgId)) {}
}

void FesapiHdfProxy::readArrayNdOfCharValues(const std::string & datasetName, char* values)
{
	const int64_t msgId = session_->sendWithSpecificHandler(
		buildGetDataArraysMessage(datasetName),
		std::make_shared<BlockingDataArrayHandlers<char>>(session_, values),
		0, 0x02);

	// Blocking loop
	while (session_->isMessageStillProcessing(msgId)) {}
}

void FesapiHdfProxy::readArrayNdOfUCharValues(const std::string & datasetName, unsigned char* values)
{
	const int64_t msgId = session_->sendWithSpecificHandler(
		buildGetDataArraysMessage(datasetName),
		std::make_shared<BlockingDataArrayHandlers<unsigned char>>(session_, values),
		0, 0x02);

	// Blocking loop
	while (session_->isMessageStillProcessing(msgId)) {}
}

std::vector<unsigned long long> FesapiHdfProxy::readArrayDimensions(const std::string & datasetName)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeGroupAttributes(const std::string & groupName,
	const std::vector<std::string> & attributeNames,
	const std::vector<std::string> & values)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeGroupAttribute(const std::string & groupName,
	const std::string & attributeName,
	const std::vector<std::string> & values)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeGroupAttributes(const std::string & groupName,
	const std::vector<std::string> & attributeNames,
	const std::vector<double> & values)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeGroupAttributes(const std::string & groupName,
	const std::vector<std::string> & attributeNames,
	const std::vector<int> & values)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeDatasetAttributes(const std::string & datasetName,
	const std::vector<std::string> & attributeNames,
	const std::vector<std::string> & values)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeDatasetAttribute(const std::string & datasetName,
	const std::string & attributeName,
	const std::vector<std::string> & values)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeDatasetAttributes(const std::string & datasetName,
	const std::vector<std::string> & attributeNames,
	const std::vector<double> & values)
{
	throw logic_error("Not implemented yet");
}

void FesapiHdfProxy::writeDatasetAttributes(const std::string & datasetName,
	const std::vector<std::string> & attributeNames,
	const std::vector<int> & values)
{
	throw logic_error("Not implemented yet");
}

std::string FesapiHdfProxy::readStringAttribute(const std::string & obj_name,
	const std::string & attr_name) const
{
	throw logic_error("Not implemented yet");
}

vector<string> FesapiHdfProxy::readStringArrayAttribute(const std::string & obj_name,
	const std::string & attr_name) const
{
	throw logic_error("Not implemented yet");
}

double FesapiHdfProxy::readDoubleAttribute(const std::string & obj_name,
	const std::string & attr_name) const
{
	throw logic_error("Not implemented yet");
}

int64_t FesapiHdfProxy::readLongAttribute(const std::string & obj_name,
	const std::string & attr_name) const
{
	throw logic_error("Not implemented yet");
}

bool FesapiHdfProxy::exist(const std::string & absolutePathInHdfFile) const
{
	throw logic_error("Not implemented yet");
}

bool FesapiHdfProxy::isCompressed(const std::string & datasetName)
{
	throw logic_error("Not implemented yet");
}
