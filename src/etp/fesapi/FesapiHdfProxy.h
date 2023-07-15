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

#include <fesapi/common/HdfProxyFactory.h>

#include "../AbstractSession.h"
#include "../ProtocolHandlers/GetFullDataArrayHandlers.h"

#include <type_traits>

namespace ETP_NS
{
	class FETPAPI_DLL_IMPORT_OR_EXPORT FesapiHdfProxy : public EML2_NS::AbstractHdfProxy
	{
	public:

		/**
		* Serialization context
		*/
		FesapiHdfProxy(AbstractSession* session, COMMON_NS::DataObjectRepository * repo, const std::string & guid, const std::string & title, const std::string & packageDirAbsolutePath, const std::string & externalFilePath, COMMON_NS::DataObjectRepository::openingMode hdfPermissionAccess) :
			EML2_NS::AbstractHdfProxy(packageDirAbsolutePath, externalFilePath, hdfPermissionAccess), session_(session), compressionLevel(0) {
			xmlNs_ = repo->getDefaultEmlVersion() == COMMON_NS::DataObjectRepository::EnergisticsStandard::EML2_0 ? "eml20" : "eml23";
			initGsoapProxy(repo, guid, title, 20);
		}

		/**
		* Deserialization context
		*/
		FesapiHdfProxy(AbstractSession* session, gsoap_resqml2_0_1::_eml20__EpcExternalPartReference* fromGsoap) :
			EML2_NS::AbstractHdfProxy(fromGsoap), session_(session), compressionLevel(0), xmlNs_("eml20") {}

		/**
		* Only for partial transfer
		*/
		FesapiHdfProxy(AbstractSession* session, const COMMON_NS::DataObjectReference& dor) :
			EML2_NS::AbstractHdfProxy(dor), session_(session), compressionLevel(0) {
			std::string ct = dor.getContentType();
			if (ct.find("2.0") != std::string::npos) {
				xmlNs_ = "eml20";
			}
			else if (ct.find("2.3") != std::string::npos) {
				xmlNs_ = "eml23";
			}
			else {
				throw std::invalid_argument("The HDF Proxy DataObjectReference does not look to be eml20 or eml23 neither : " + ct);
			}
		}


		/**
		* Destructor.
		*/
		~FesapiHdfProxy() = default;

		AbstractSession* getSession() { return session_; }

		/**
		* Does nothing since the ETP session must already be opened.
		*/
		void open() final {}

		/**
		* It is opened if the ETP session is opened
		*/
		bool isOpened() const final { return session_ != nullptr && !session_->isWebSocketSessionClosed();  }

		/**
		* Does nothing since the ETP session can be used for other purpose.
		*/
		void close() final {}

		/*
		* Get the used (native) datatype in a dataset
		*/
		COMMON_NS::AbstractObject::numericalDatatypeEnum getNumericalDatatype(const std::string & groupName) final;

		/**
		* Get the used datatype class in a dataset
		* To compare with H5T_INTEGER, H5T_FLOAT , H5T_STRING , etc...
		*/
		int getHdfDatatypeClassInDataset(const std::string & datasetName) final;

		/**
		* Write an itemized list of list into the HDF file by means of a single group containing 2 datasets.
		* @param groupName				The name of the group where to create the itemized list of list.
		*								This name must not contain '/' character and must be directly contained in RESQML group.
		* @param name					The name of the itemized list of list hdf group.
		* @param cumulativeLengthDatatype	The datatype of the cumulative length to write.
		* @param cumulativeLength			1d array of positive integers containing for each list the sum of all the previous list lengths including the current one
		* @param cumulativeLengthSize		Size of the cumulativeLength array.
		* @param elementsDatatype			The datatype of the elements to write.
		* @param elements					1d array of elements containing the aggregation of individual list content.
		* @param elementsSize				Size of the elements array.
		*/
		void writeItemizedListOfList(const std::string & groupName,
			const std::string & name,
			COMMON_NS::AbstractObject::numericalDatatypeEnum cumulativeLengthDatatype,
			const void * cumulativeLength,
			uint64_t cumulativeLengthSize,
			COMMON_NS::AbstractObject::numericalDatatypeEnum elementsDatatype,
			const void * elements,
			uint64_t elementsSize) final;

		/**
		 * Get the number of elements in each dimension in an HDF dataset of the proxy.
		 * @param datasetName	The absolute name of the dataset we want to get the number of elements.
		 */
		std::vector<uint32_t> getElementCountPerDimension(const std::string & datasetName) final;

		/**
		* Set the new compression level which will be used for all data to be written
		* @param compressionLevel				Lower compression levels are faster but result in less compression. Range [0..9] is allowed.
		*/
		void setCompressionLevel(unsigned int newCompressionLevel) final { if (newCompressionLevel > 9) compressionLevel = 9; else compressionLevel = newCompressionLevel; }

		/**
		* Recursively write sub arrays (potentially with 2 dimensions) of a specific datatype into the HDF file by means of a single dataset.
		* @param uri							The uri of the original array.
		* @param pathInResource					The path of the original array.
		* @param totalCounts					The total number of values in each dimension of the original array.
		* @param starts							The starting indices in each dimension of the subarray to be written.
		* @param counts							The number of values in each dimension of the subarray to be written.
		* @param values							1d array of specific datatype ordered firstly by fastest direction.
		*/
		template<typename T>
		void writeSubArrayNd(
			const std::string& uri,
			const std::string& pathInResource,
			std::vector<int64_t>& totalCounts,
			std::vector<int64_t> starts,
			std::vector<int64_t> counts,
			const void* values) {

			// Calculate array size
			size_t totalCount{ 1 };

			for (const auto& count : counts)
				totalCount *= count;

			// [Base Condition] Array size is OK to be transmitted.
			if ((totalCount * sizeof(T)) <= maxArraySize_) {

				// PUT DATA SUBARRAYS
				Energistics::Etp::v12::Protocol::DataArray::PutDataSubarrays pdsa{};
				pdsa.dataSubarrays["0"].uid.uri = uri;
				pdsa.dataSubarrays["0"].uid.pathInResource = pathInResource;
				pdsa.dataSubarrays["0"].starts = starts;
				pdsa.dataSubarrays["0"].counts = counts;

				// Cast values in T values.
				const T* typeValues{ static_cast<const T*>(values) };

				// Create 1D Array for Sub Values.
				T* subValues = new T[totalCount];
				size_t valueIndex{ 0 };

				// Recursively populate subValues starting from first dimension.
				populateSubValuesNd<T>(
					0,
					totalCounts, starts, counts,
					valueIndex, typeValues, subValues);

				// Create AVRO Array
				Energistics::Etp::v12::Datatypes::AnyArray data;
				createAnyArray<T>(data, totalCount, subValues); // Type-specific code is written in specialized template functions for createAnyArray().
				pdsa.dataSubarrays["0"].data = data;

				std::cout << "Writing subarray..." << std::endl;

				// Send putDataSubarrays Message
				session_->sendAndBlock(pdsa, 0, 0x02);

				// Delete Array
				delete[] subValues;
			}
			// [Divide and Conquer Approach] If sub array is still large, partition it into more sub arrays.
			else {
				// Recursively divide all dimensions starting from first dimension.
				createSubArrayNd<T>(
					0,
					uri, pathInResource, totalCounts,
					starts, counts, values);
			}
		};

		/**
		* Recursively populate subValues array from original values array.
		* @param dimensionIndex					The index of dimension in nD array.
		* @param totalCounts					The total number of values in each dimension of the original array.				
		* @param starts							The starting indices in each dimension of the subarray to be written.
		* @param counts							The number of values in each dimension of the subarray to be written.
		* @param valueIndex						The index of subValues array.
		* @param values							1d array of specific datatype ordered firstly by fastest direction.
		* @param subValues						1d subarray to be populated.
		*/
		template<typename T>
		void populateSubValuesNd(
			size_t dimensionIndex,
			std::vector<int64_t>& totalCounts,
			std::vector<int64_t>& starts,
			std::vector<int64_t>& counts,
			size_t& valueIndex,
			const T* values,
			T* subValues) {

			// [Base Condition] If dimensionIndex exceeds last dimension.
			if (dimensionIndex >= starts.size()) {
				// Add value in subValues.
				subValues[valueIndex] =
					values[getRowMajorIndex(0, totalCounts, starts)]; // Convert nD indices to row major order index.

				++valueIndex;
			}
			else {
				// Save starting index.
				int64_t start = starts[dimensionIndex];

				for (size_t i = 0; i < counts[dimensionIndex]; ++i) {
					// Recursively populate subValues for next dimensions.
					populateSubValuesNd<T>(
						dimensionIndex + 1,
						totalCounts,
						starts, counts,
						valueIndex, values, subValues);

					starts[dimensionIndex]++;
				}

				// Restore starting index.
				starts[dimensionIndex] = start;
			}
		}

		/*
			HOW ROW MAJOR INDEX IN CALCULATED FROM N INDICES	
			index = i0 * (dim1 * dim2 * ... * dimn) + i1 * (dim2 * dim3 * ... * dimn) + i2 * (dim3 * dim4 * ... * dimn) + ... + in-1 * dimn + in
			Here:

				1.	index is the calculated index value.
				2.	i0, i1, i2, ..., in-1, in represent the indices of the element in each dimension. 
					For example, i0 is the index in the first dimension, i1 is the index in the second dimension, and so on.
				3.	dim1, dim2, dim3, ..., dimn are the sizes of the array along each dimension. 
					These represent the number of elements in each dimension.
		*/

		/**
		* Recursively calculate row major index from n indices.
		* @param dimensionIndex					The index of dimension in nD array.
		* @param totalCounts					The total number of values in each dimension of the original array.
		* @param starts							The starting indices in each dimension of the subarray to be written.
		*/
		int64_t getRowMajorIndex(
			size_t dimensionIndex,
			std::vector<int64_t>& totalCounts,
			std::vector<int64_t>& starts) {
			// [Base Condition] If dimensionIndex is the last dimension.	
			if (dimensionIndex == (starts.size() - 1)) {
				return starts[dimensionIndex];
			}
			else {
				return starts[dimensionIndex] * getCountsProduct(dimensionIndex + 1, totalCounts) + 
					getRowMajorIndex((dimensionIndex + 1), totalCounts, starts);
			}
		}

		/**
		* Recursively calculate product of total counts for calculating row major index.
		* @param dimensionIndex					The index of dimension in nD array.
		* @param totalCounts					The total number of values in each dimension of the original array.
		*/
		int64_t getCountsProduct(
			size_t dimensionIndex,
			std::vector<int64_t>& totalCounts) {
			// [Base Condition] If dimensionIndex exceeds the last dimension.	
			if (dimensionIndex >= totalCounts.size()) {
				return 1;
			}
			else {
				return totalCounts[dimensionIndex] * 
					getCountsProduct(dimensionIndex + 1, totalCounts);
			}
		}

		/**
		* Recursively divide each dimension into half and create an n-D subbarray.
		* @param dimensionIndex					The index of dimension in nD array.
		* @param uri							The uri of the original array.
		* @param pathInResource					The path of the original array.
		* @param totalCounts					The total number of values in each dimension of the original array.
		* @param starts							The starting indices in each dimension of the subarray to be written.
		* @param counts							The number of values in each dimension of the subarray to be written.
		* @param values							1d array of specific datatype ordered firstly by fastest direction.
		*/
		template<typename T>
		void createSubArrayNd(
			size_t dimensionIndex,
			const std::string& uri,
			const std::string& pathInResource,
			std::vector<int64_t>& totalCounts,
			std::vector<int64_t> starts,
			std::vector<int64_t> counts,
			const void* values) {
			// [Base Condition] If dimensionIndex exceeds the last dimension.
			if (dimensionIndex >= starts.size()) {
				// Recursively Write Subarray.
				writeSubArrayNd<T>(
					uri, pathInResource, totalCounts,
					starts,
					counts,
					values);
			}
			else {
				int64_t numberOfValues = counts[dimensionIndex];

				int64_t firstHalfValues = numberOfValues / 2;
				int64_t secondHalfValues = numberOfValues - firstHalfValues;

				std::vector<int64_t> newCounts{ counts };
				newCounts[dimensionIndex] = firstHalfValues;

				// Recursively divide next dimension.
				createSubArrayNd<T>(
					dimensionIndex + 1,
					uri, pathInResource, totalCounts,
					starts,
					newCounts,
					values);

				std::vector<int64_t> newStarts{ starts };
				newStarts[dimensionIndex] = newStarts[dimensionIndex] + firstHalfValues;
				newCounts[dimensionIndex] = secondHalfValues;

				// Recursively divide next dimension.
				createSubArrayNd<T>(
					dimensionIndex + 1,
					uri, pathInResource, totalCounts,
					newStarts,
					newCounts,
					values);
			}
		}

		/**
		* Create AnyArray from given data array of type T.
		* @param data							The reference to AnyArray to be populated.
		* @param totalCount						Total number of values.
		* @param values							1d array of specific datatype ordered firstly by fastest direction.
		*/
		template<typename T>
		void createAnyArray(
			Energistics::Etp::v12::Datatypes::AnyArray& data,
			size_t totalCount,
			T* values) {
			throw logic_error(
				"Subarrays are implemented for primitive types only: double, float, int64, int32, short, char");
		}

		/**
		* Create AnyArray from given data array of type double.
		* @param data							The reference to AnyArray to be populated.
		* @param totalCount						Total number of values.
		* @param values							1d array of specific datatype ordered firstly by fastest direction.
		*/
		template<>
		void createAnyArray<double>(
			Energistics::Etp::v12::Datatypes::AnyArray& data,
			size_t totalCount,
			double* values) {		
			Energistics::Etp::v12::Datatypes::ArrayOfDouble avroArray;
			avroArray.values = std::vector<double>(values, values + totalCount);
			data.item.set_ArrayOfDouble(avroArray);	
		}

		/**
		* Create AnyArray from given data array of type float.
		* @param data							The reference to AnyArray to be populated.
		* @param totalCount						Total number of values.
		* @param values							1d array of specific datatype ordered firstly by fastest direction.
		*/
		template<>
		void createAnyArray<float>(
			Energistics::Etp::v12::Datatypes::AnyArray& data,
			size_t totalCount,
			float* values) {
			Energistics::Etp::v12::Datatypes::ArrayOfFloat avroArray;
			avroArray.values = std::vector<float>(values, values + totalCount);
			data.item.set_ArrayOfFloat(avroArray);
		}

		/**
		* Create AnyArray from given data array of type int64_t.
		* @param data							The reference to AnyArray to be populated.
		* @param totalCount						Total number of values.
		* @param values							1d array of specific datatype ordered firstly by fastest direction.
		*/
		template<>
		void createAnyArray<int64_t>(
			Energistics::Etp::v12::Datatypes::AnyArray& data,
			size_t totalCount,
			int64_t* values) {
			Energistics::Etp::v12::Datatypes::ArrayOfLong avroArray;
			avroArray.values = std::vector<int64_t>(values, values + totalCount);
			data.item.set_ArrayOfLong(avroArray);
		}

		/**
		* Create AnyArray from given data array of type int32_t.
		* @param data							The reference to AnyArray to be populated.
		* @param totalCount						Total number of values.
		* @param values							1d array of specific datatype ordered firstly by fastest direction.
		*/
		template<>
		void createAnyArray<int32_t>(
			Energistics::Etp::v12::Datatypes::AnyArray& data,
			size_t totalCount,
			int32_t* values) {
			Energistics::Etp::v12::Datatypes::ArrayOfInt avroArray;
			avroArray.values = std::vector<int32_t>(values, values + totalCount);
			data.item.set_ArrayOfInt(avroArray);
		}

		/**
		* Create AnyArray from given data array of type short.
		* @param data							The reference to AnyArray to be populated.
		* @param totalCount						Total number of values.
		* @param values							1d array of specific datatype ordered firstly by fastest direction.
		*/
		template<>
		void createAnyArray<short>(
			Energistics::Etp::v12::Datatypes::AnyArray& data,
			size_t totalCount,
			short* values) {
			Energistics::Etp::v12::Datatypes::ArrayOfInt avroArray;

			for (size_t i = 0; i < totalCount; ++i)
				avroArray.values.push_back(values[i]);

			data.item.set_ArrayOfInt(avroArray);
		}

		/**
		* Create AnyArray from given data array of type char.
		* @param data							The reference to AnyArray to be populated.
		* @param totalCount						Total number of values.
		* @param values							1d array of specific datatype ordered firstly by fastest direction.
		*/
		template<>
		void createAnyArray<char>(
			Energistics::Etp::v12::Datatypes::AnyArray& data,
			size_t totalCount,
			char* values) {
			std::string avroArray{};

			for (size_t i = 0; i < totalCount; ++i)
				avroArray.push_back(values[i]);

			data.item.set_bytes(avroArray);
		}

		/**
		* Write an array (potentially with multi dimensions) of a specific datatype into the HDF file by means of a single dataset.
		* @param groupName						The name of the group where to create the array of values.
		*										This name must not contain '/' character and must be directly contained in RESQML group.
		* @param name							The name of the array (potentially with multi dimensions) of a specific datatype hdf dataset. It must not already exist.
		* @param datatype						The specific datatype of the values to write.
		* @param values							1d array of specific datatype ordered firstly by fastest direction.
		* @param numValuesInEachDimension		Number of values in each dimension of the array to write. They are ordered from fastest index to slowest index.
		* @param numDimensions					The number of the dimensions of the array to write
		*/
		void writeArrayNd(const std::string & groupName,
			const std::string & name,
			COMMON_NS::AbstractObject::numericalDatatypeEnum datatype,
			const void * values,
			const uint64_t * numValuesInEachDimension,
			unsigned int numDimensions) final;

		/**
		* Create an array (potentially with multi dimensions) of a specific datatype into the HDF file. Values are not yet written to this array.
		* @param groupName                      The name of the group where to create the array of double values.
		*                                       This name must not contain '/' character and must be directly contained in RESQML group.
		* @param name                           The name of the array (potentially with multi dimensions) of a specific datatype hdf dataset. It must not exist.
		* @param datatype                       The specific datatype of the values to write.
		* @param numValuesInEachDimension       Number of values in each dimension of the array to write. They are ordered from fastest index to slowest index.
		* @param numDimensions                  The number of the dimensions of the array to write.
		*/
		void createArrayNd(
			const std::string& groupName,
			const std::string& name,
			COMMON_NS::AbstractObject::numericalDatatypeEnum datatype,
			const uint64_t* numValuesInEachDimension,
			unsigned int numDimensions) final;

		/**
		* Find the array associated with @p groupName and @p name and write to it.
		* @param groupName                      The name of the group associated with the array.
		* @param name                           The name of the array (potentially with multi dimensions).
		* @param datatype						The specific datatype of the values to write.
		* @param values                         1d array of specific datatype ordered firstly by fastest direction.
		* @param numValuesInEachDimension       Number of values in each dimension of the array to write. They are ordered from fastest index to slowest index.
		* @param offsetValuesInEachDimension    Offset values in each dimension of the array to write. They are ordered from fastest index to slowest index.
		* @param numDimensions                  The number of the dimensions of the array to write.
		*/
		void writeArrayNdSlab(
			const std::string& groupName,
			const std::string& name,
			COMMON_NS::AbstractObject::numericalDatatypeEnum datatype,
			const void* values,
			const uint64_t* numValuesInEachDimension,
			const uint64_t* offsetValuesInEachDimension,
			unsigned int numDimensions) final;

		/**
		* Write some string attributes into a group
		*/
		void writeGroupAttributes(const std::string&,
			const std::vector<std::string>&,
			const std::vector<std::string>&) final {
			throw std::logic_error("Group attributes are not supported in ETP1.2");
		}

		/**
		* Write a single attribute which contain an array of strings
		*/
		void writeGroupAttribute(const std::string&,
			const std::string&,
			const std::vector<std::string>&) final {
			throw std::logic_error("Group attributes are not supported in ETP1.2");
		}

		/**
		* Write some double attributes into a group
		*/
		void writeGroupAttributes(const std::string&,
			const std::vector<std::string>&,
			const std::vector<double>&) {
			throw std::logic_error("Group attributes are not supported in ETP1.2");
		}

		/**
		* Write some int attributes into a group
		*/
		void writeGroupAttributes(const std::string&,
			const std::vector<std::string>&,
			const std::vector<int>&) final {
			throw std::logic_error("Group attributes are not supported in ETP1.2");
		}

		/**
		* Write some string attributes into a dataset
		*/
		void writeDatasetAttributes(const std::string&,
			const std::vector<std::string>&,
			const std::vector<std::string>&) final {
			throw std::logic_error("Dataset attributes are not supported in ETP1.2");
		}

		/**
		* Write a single attribute which contain an array of strings
		*/
		void writeDatasetAttribute(const std::string&,
			const std::string&,
			const std::vector<std::string>&) final {
			throw std::logic_error("Dataset attributes are not supported in ETP1.2");
		}

		/**
		* Write some double attributes into a dataset
		*/
		void writeDatasetAttributes(const std::string&,
			const std::vector<std::string>&,
			const std::vector<double>&) final {
			throw std::logic_error("Dataset attributes are not supported in ETP1.2");
		}

		/**
		* Write some int attributes into a dataset
		*/
		void writeDatasetAttributes(const std::string&,
			const std::vector<std::string>&,
			const std::vector<int>&) final {
			throw std::logic_error("Dataset attributes are not supported in ETP1.2");
		}

		std::string readStringAttribute(const std::string&,
			const std::string&) const final {
			throw std::logic_error("Attributes are not supported in ETP1.2");
		}

		std::vector<std::string> readStringArrayAttribute(const std::string&,
			const std::string&) const final {
			throw std::logic_error("Attributes are not supported in ETP1.2");
		}

		double readDoubleAttribute(const std::string&,
			const std::string&) const final {
			throw std::logic_error("Attributes are not supported in ETP1.2");
		}

		int64_t readInt64Attribute(const std::string&,
			const std::string&) const final {
			throw std::logic_error("Attributes are not supported in ETP1.2");
		}

		/**
		* Read an array Nd of double values stored in a specific dataset
		* @param datasetName	The absolute dataset name where to read the values
		* @param values 		The values must be pre-allocated.
		*/
		void readArrayNdOfDoubleValues(const std::string & datasetName, double* values) final { readArrayNdOfValues(datasetName, values); }

		/**
		 * Find the array associated with @p datasetName and read from it.
		 * @param datasetName                    The name of the array (potentially with multi dimensions).
		 * @param values                         1d array output of double values ordered firstly by fastest direction.
		 * @param numValuesInEachDimension       Number of values in each dimension of the array to read. They are ordered from fastest index to slowest index.
		 * @param offsetValuesInEachDimension    Offset values in each dimension of the array to read. They are ordered from fastest index to slowest index.
		 * @param numDimensions                  The number of the dimensions of the array to read.
		 */
		void readArrayNdOfDoubleValues(
			const std::string & datasetName,
			double* values,
			uint64_t const * numValuesInEachDimension,
			uint64_t const * offsetInEachDimension,
			unsigned int numDimensions) final;

		/**
		* Find the array associated with @p datasetName and read from it.
		* @param datasetName					The name of the array (potentially with multi dimensions).
		* @param values							1d array output of double values ordered firstly by fastest direction.
		* @param blockCountPerDimension			Number of blocks to select from the dataspace, in each dimension. They are ordered from fastest index to slowest index.
		* @param offsetInEachDimension			Offset values in each dimension of the array to read. They are ordered from fastest index to slowest index.
		* @param strideInEachDimension			Number of elements to move from one block to another in each dimension. They are ordered from fastest index to slowest index.
		* @param blockSizeInEachDimension		Size of selected blocks in each dimension. They are ordered from fastest index to slowest index.
		* @param numDimensions					The number of the dimensions of the array to read.
		*/
		void readArrayNdOfDoubleValues(
			const std::string & datasetName, double* values,
			uint64_t const * blockCountPerDimension,
			uint64_t const * offsetInEachDimension,
			uint64_t const * strideInEachDimension,
			uint64_t const * blockSizeInEachDimension,
			unsigned int numDimensions) final;

		void selectArrayNdOfValues(
			const std::string & datasetName,
			uint64_t const * blockCountPerDimension,
			uint64_t const * offsetInEachDimension,
			uint64_t const * strideInEachDimension,
			uint64_t const * blockSizeInEachDimension,
			unsigned int numDimensions,
			bool newSelection,
			hdf5_hid_t & dataset,
			hdf5_hid_t & filespace) final;

		/**
		* Considering a given dataset, read the double values corresponding to an existing selected region.
		* @param dataset		ID of the dataset to read from.
		* @param filespace		ID of the selected region.
		* @param values			1d array output of double values ordered firstly by fastest direction.
		* @param slabSize		Number of values to read.
		*/
		void readArrayNdOfDoubleValues(
			hdf5_hid_t dataset,
			hdf5_hid_t filespace,
			void* values,
			uint64_t slabSize) final;

		/**
		* Read an array Nd of float values stored in a specific dataset.
		* @param datasetName	The absolute dataset name where to read the values
		* @param values 		The values must be pre-allocated.
		*/
		void readArrayNdOfFloatValues(const std::string & datasetName, float* values) final { readArrayNdOfValues(datasetName, values); }

		/**
		* Find the array associated with @p datasetName and read from it.
		* @param datasetName                    The name of the array (potentially with multi dimensions).
		* @param values                         1d array output of float values ordered firstly by fastest direction.
		* @param numValuesInEachDimension       Number of values in each dimension of the array to read. They are ordered from fastest index to slowest index.
		* @param offsetValuesInEachDimension    Offset values in each dimension of the array to read. They are ordered from fastest index to slowest index.
		* @param numDimensions                  The number of the dimensions of the array to read.
		*/
		void readArrayNdOfFloatValues(
			const std::string & datasetName,
			float* values,
			uint64_t const * numValuesInEachDimension,
			uint64_t const * offsetInEachDimension,
			unsigned int numDimensions) final;

		/**
		* Read an array Nd of int 64 bits values stored in a specific dataset.
		* @param datasetName	The absolute dataset name where to read the values
		* @param values 		The values must be pre-allocated.
		*/
		void readArrayNdOfInt64Values(const std::string & datasetName, int64_t* values) final { readArrayNdOfValues(datasetName, values); }

		/**
		* Find the array associated with datasetName and read from it.
		* @param datasetName                    The name of the array (potentially with multi dimensions).
		* @param values                         1d array output of int 64 bits values ordered firstly by fastest direction.
		* @param numValuesInEachDimension       Number of values in each dimension of the array to read. They are ordered from fastest index to slowest index.
		* @param offsetValuesInEachDimension    Offset values in each dimension of the array to read. They are ordered from fastest index to slowest index.
		* @param numDimensions                  The number of the dimensions of the array to read.
		*/
		void readArrayNdOfInt64Values(
			const std::string & datasetName,
			int64_t* values,
			uint64_t const * numValuesInEachDimension,
			uint64_t const * offsetInEachDimension,
			unsigned int numDimensions)  final;

		/**
		* Read an array Nd of unsigned int 64 bits values stored in a specific dataset.
		* @param datasetName	The absolute dataset name where to read the values
		* @param values 		The values must be pre-allocated.
		*/
		void readArrayNdOfUInt64Values(const std::string & datasetName, uint64_t* values) final { readArrayNdOfValues(datasetName, values); }

		/**
		* Read an array Nd of int values stored in a specific dataset.
		* @param datasetName	The absolute dataset name where to read the values
		* @param values 		The values must be pre-allocated.
		*/
		void readArrayNdOfIntValues(const std::string & datasetName, int* values) final { readArrayNdOfValues(datasetName, values); }

		/**
		* Find the array associated with datasetName and read from it.
		* @param datasetName                    The name of the array (potentially with multi dimensions).
		* @param values                         1d array output of int values ordered firstly by fastest direction.
		* @param numValuesInEachDimension       Number of values in each dimension of the array to read. They are ordered from fastest index to slowest index.
		* @param offsetValuesInEachDimension    Offset values in each dimension of the array to read. They are ordered from fastest index to slowest index.
		* @param numDimensions                  The number of the dimensions of the array to read.
		*/
		void readArrayNdOfIntValues(
			const std::string & datasetName,
			int* values,
			uint64_t const * numValuesInEachDimension,
			uint64_t const * offsetInEachDimension,
			unsigned int numDimensions
		) final;

		/**
		* Read an array Nd of unsigned int values stored in a specific dataset
		* Don"t forget to delete the allocated pointer when no more necessary.
		* @param datasetName	The absolute dataset name where to read the values
		*/
		void readArrayNdOfUIntValues(const std::string & datasetName, unsigned int* values) final { readArrayNdOfValues(datasetName, values); }

		/**
		* Read an array Nd of short values stored in a specific dataset
		* Don"t forget to delete the allocated pointer when no more necessary.
		* @param datasetName	The absolute dataset name where to read the values
		* @param values 		The values must be pre-allocated.
		*/
		void readArrayNdOfShortValues(const std::string & datasetName, short* values) final { readArrayNdOfValues(datasetName, values); }

		/**
		* Read an array Nd of unsigned short values stored in a specific dataset.
		* @param datasetName	The absolute dataset name where to read the values
		* @param values 		The values must be pre-allocated.
		*/
		void readArrayNdOfUShortValues(const std::string & datasetName, unsigned short* values) final { readArrayNdOfValues(datasetName, values); }

		/**
		* Read an array Nd of int 8 bits values stored in a specific dataset.
		* @param datasetName	The absolute dataset name where to read the values
		* @param values 		The values must be pre-allocated.
		*/
		void readArrayNdOfInt8Values(const std::string & datasetName, int8_t* values) final { readArrayNdOfValues(datasetName, values); }

		/**
		* Read an array Nd of int 8 bits values stored in a specific dataset.
		* @param datasetName	The absolute dataset name where to read the values
		*/
		void readArrayNdOfUInt8Values(const std::string & datasetName, uint8_t* values) final { readArrayNdOfValues(datasetName, values); }

		/**
		* Check wether an absolute path exists in the hdf file or not.
		*/
		bool exist(const std::string & absolutePathInHdfFile) const final;

		/**
		* Check wether a dataset is compressed or not.
		*/
		bool isCompressed(const std::string & datasetName) final;
		
		/**
		 * Get the number of elements in each chunk dimension of an HDF5 dataset.
		 * If the dataset is not compressed, then it returns an empty vector.
		 * @param datasetName	The absolute name of the dataset which we want to get the number of elements from.
		 */
		std::vector<uint32_t> getElementCountPerChunkDimension(const std::string&) final { return {}; }

		/**
		* Get the standard XML namespace for serializing this data object.
		*/
		std::string getXmlNamespace() const { return xmlNs_; }

	private:
		AbstractSession* session_;
		unsigned int compressionLevel;
		std::string xmlNs_;
		int maxArraySize_{ 4000000 }; // Bytes

		Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayIdentifier buildDataArrayIdentifier(const std::string & datasetName) const;
		Energistics::Etp::v12::Protocol::DataArray::GetDataArrays buildGetDataArraysMessage(const std::string & datasetName) const;
		Energistics::Etp::v12::Protocol::DataArray::GetDataArrayMetadata buildGetDataArrayMetadataMessage(const std::string & datasetName) const;

		Energistics::Etp::v12::Datatypes::DataArrayTypes::DataArrayMetadata getDataArrayMetadata(const std::string & datasetName) const;

		template<typename T> void readArrayNdOfValues(const std::string & datasetName, T* values)
		{
			// First get metadata about the data array
			std::vector<uint64_t> dimensions;

			const auto daMetadata = getDataArrayMetadata(datasetName);
			size_t valueCount = 1;
			for (auto dim : daMetadata.dimensions) {
				valueCount *= dim;
			}

			size_t valueSize = 1;
			switch (daMetadata.transportArrayType) {
			case Energistics::Etp::v12::Datatypes::AnyArrayType::bytes:
			case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfBoolean: break;
			case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfInt: valueSize = 6; break; // 25% more because of zig zag encoding worst case scenario
			case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfFloat: valueSize = 4; break;
			case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfLong:  valueSize = 10; break; // 25% more because of zig zag encoding worst case scenario
			case Energistics::Etp::v12::Datatypes::AnyArrayType::arrayOfDouble: valueSize = 8; break;
			default: throw std::logic_error("Array of strings are not implemented yet");
			}
			size_t wholeSize = valueCount * valueSize;

			// maxAllowedDataArraySize is the maximum serialized size of the array (including avro extra longs for array blocks)
			const size_t maxAllowedDataArraySize = session_->getMaxWebSocketMessagePayloadSize()
				- sizeof(Energistics::Etp::v12::Datatypes::MessageHeader)
				- (daMetadata.dimensions.size() * 2 + 1) * 8; // *2 because the array can contain a maximum of element count block, +1 for the length of the last block : https://avro.apache.org/docs/1.10.2/spec.html#binary_encode_complex

			// Now get values of the data array
			auto specializedHandler = std::make_shared<GetFullDataArrayHandlers<T>>(session_, values);
			if (wholeSize + (valueCount + 1) * 8 <= maxAllowedDataArraySize) { // There can be valueCount array block and there is the length of the last array block
				// Get all values at once
				const int64_t msgId = session_->sendWithSpecificHandler(
					buildGetDataArraysMessage(datasetName),
					specializedHandler,
					0, 0x02);

				// Blocking loop
				while (session_->isMessageStillProcessing(msgId)) {}
			}
			else {
				// Get all values using several data subarrays allowing more granular streaming
				std::vector<int64_t> counts(daMetadata.dimensions.size(), 1);

				// Compute the dimensions of the subArrays to get
				size_t subArrayValueCount = 1;
				for (int64_t dimIndex = daMetadata.dimensions.size() - 1; dimIndex >= 0; --dimIndex) {
					int64_t maxCountOnDim = daMetadata.preferredSubarrayDimensions.empty()
						? daMetadata.dimensions[dimIndex]
						: daMetadata.preferredSubarrayDimensions[dimIndex];
					subArrayValueCount *= maxCountOnDim;
					int64_t allowedCountOnDim = maxCountOnDim;
					while (subArrayValueCount * valueSize + (subArrayValueCount + 1) * 8 > maxAllowedDataArraySize) {
						subArrayValueCount /= allowedCountOnDim;
						allowedCountOnDim /= 2;
						subArrayValueCount *= allowedCountOnDim;
					}
					counts[dimIndex] = allowedCountOnDim;
					if (allowedCountOnDim != maxCountOnDim) {
						break;
					}
				}

				// Build the message
				Energistics::Etp::v12::Protocol::DataArray::GetDataSubarrays msg;
				size_t subArrayIndex = 0;
				std::vector<int64_t> starts(daMetadata.dimensions.size(), 0);
				std::vector<int64_t> currentCounts = counts;
				bool hasParsedAllArray = false;
				while (!hasParsedAllArray) {
					std::string subArrayIndexStr = std::to_string(subArrayIndex);
					msg.dataSubarrays[subArrayIndexStr].uid = buildDataArrayIdentifier(datasetName);
					msg.dataSubarrays[subArrayIndexStr].counts = currentCounts;
					msg.dataSubarrays[subArrayIndexStr].starts = starts;

					// next sub array to get
					++subArrayIndex;
					hasParsedAllArray = true;
					for (int64_t dimIndex = daMetadata.dimensions.size() - 1; dimIndex >= 0; --dimIndex) {
						if (starts[dimIndex] + currentCounts[dimIndex] < daMetadata.dimensions[dimIndex]) {
							starts[dimIndex] += currentCounts[dimIndex];
							if (starts[dimIndex] + currentCounts[dimIndex] > daMetadata.dimensions[dimIndex]) {
								currentCounts[dimIndex] = daMetadata.dimensions[dimIndex] - starts[dimIndex];
							}

							for (size_t dimIndex2 = dimIndex + 1; dimIndex2 < daMetadata.dimensions.size(); ++dimIndex2) {
								starts[dimIndex2] = 0;
								currentCounts[dimIndex2] = counts[dimIndex2];
							}
							hasParsedAllArray = false;
							break;
						}
					}

					specializedHandler->setDataSubarrays(subArrayIndexStr, msg.dataSubarrays[subArrayIndexStr]);
				}

				// Send message
				const int64_t msgId = session_->sendWithSpecificHandler(
					msg,
					specializedHandler,
					0, 0x02);

				// Blocking loop
				while (session_->isMessageStillProcessing(msgId)) {}
			}
		}
	};

	class FETPAPI_DLL_IMPORT_OR_EXPORT FesapiHdfProxyFactory : public COMMON_NS::HdfProxyFactory
	{
	public:
		FesapiHdfProxyFactory(AbstractSession* session) : session_(session) {}

		/**
		* Only to be used in partial transfer context
		*/
		EML2_NS::AbstractHdfProxy* make(const COMMON_NS::DataObjectReference& dor) {
			FesapiHdfProxy* result = new FesapiHdfProxy(session_, dor);
			return result;
		}

		/**
		* Creates an instance of this class by wrapping a gsoap instance.
		*/
		EML2_NS::AbstractHdfProxy* make(gsoap_resqml2_0_1::_eml20__EpcExternalPartReference* fromGsoap) {
			FesapiHdfProxy* result = new FesapiHdfProxy(session_, fromGsoap);
			return result;
		}

		EML2_NS::AbstractHdfProxy* make(COMMON_NS::DataObjectRepository * repo, const std::string & guid, const std::string & title,
			const std::string & packageDirAbsolutePath, const std::string & externalFilePath,
			COMMON_NS::DataObjectRepository::openingMode hdfPermissionAccess = COMMON_NS::DataObjectRepository::openingMode::READ_ONLY) {
			FesapiHdfProxy* result = new FesapiHdfProxy(session_, repo, guid, title, packageDirAbsolutePath, externalFilePath, hdfPermissionAccess);
			return result;
		}

	private:
		AbstractSession* session_;
	};
}
